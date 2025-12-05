#ifndef LIBRERIA_REPORTES_H
#define LIBRERIA_REPORTES_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iomanip>
#include <set>

// Se asume que el entorno tiene Gnuplot instalado y en el PATH
#include "gnuplot.h"

using namespace std;

// ======================================================
//  FUNCIONES DE CARGA DE DATOS (AUXILIARES)
// ======================================================

vector<Venta> cargarVentasReporte() {
    vector<Venta> ventas;
    ifstream f("ventas.bin", ios::binary);
    if (f) {
        Venta v;
        while (f.read((char*)&v, sizeof(Venta))) {
            ventas.push_back(v);
        }
        f.close();
    }
    return ventas;
}

vector<DetalleVenta> cargarDetallesReporte() {
    vector<DetalleVenta> detalles;
    ifstream f("detalles.bin", ios::binary);
    if (f) {
        DetalleVenta d;
        while (f.read((char*)&d, sizeof(DetalleVenta))) {
            detalles.push_back(d);
        }
        f.close();
    }
    return detalles;
}

vector<Producto> cargarProductosReporte() {
    vector<Producto> productos;
    ifstream f("productos.bin", ios::binary);
    if (f) {
        Producto p;
        while (f.read((char*)&p, sizeof(Producto))) {
            productos.push_back(p);
        }
        f.close();
    }
    return productos;
}

string obtenerNombreProducto(int id) {
    ifstream f("productos.bin", ios::binary);
    if (f) {
        Producto p;
        while (f.read((char*)&p, sizeof(Producto))) {
            if (p.id == id) {
                return string(p.nombre);
            }
        }
        f.close();
    }
    return "Desconocido";
}

string obtenerNombreCliente(int id) {
    ifstream f("clientes.bin", ios::binary);
    if (f) {
        Cliente c;
        while (f.read((char*)&c, sizeof(Cliente))) {
            if (c.id == id) {
                return string(c.nombre);
            }
        }
        f.close();
    }
    return "Desconocido";
}

// ======================================================
//  UTILIDADES DE FECHA
// ======================================================

// Convierte "DD/MM/AAAA" a entero YYYYMMDD para comparacion
long long fechaAEntero(string fecha) {
    if (fecha.length() != 10) return 0;
    string d = fecha.substr(0, 2);
    string m = fecha.substr(3, 2);
    string a = fecha.substr(6, 4);
    try {
        return stoll(a + m + d);
    } catch (...) {
        return 0;
    }
}

// ======================================================
//  REPORTES DE CLIENTES
// ======================================================

// Cliente con mas compras (basado en la cantidad de productos)
void reporteClienteMasProductos() {
    vector<Venta> ventas = cargarVentasReporte();
    vector<DetalleVenta> detalles = cargarDetallesReporte();

    if (ventas.empty() || detalles.empty()) {
        cout << "No hay suficientes datos para generar el reporte.\n";
        return;
    }

    // Mapa: VentaId -> ClienteId
    map<int, int> ventaCliente;
    for (const auto& v : ventas) {
        ventaCliente[v.id] = v.clienteId;
    }

    // Mapa: ClienteId -> CantidadTotalProductos
    map<int, int> clienteProductos;
    for (const auto& d : detalles) {
        if (ventaCliente.count(d.ventaId)) {
            int clienteId = ventaCliente[d.ventaId];
            clienteProductos[clienteId] += d.cantidad;
        }
    }

    int maxId = -1;
    int maxCant = -1;

    for (const auto& pair : clienteProductos) {
        if (pair.second > maxCant) {
            maxCant = pair.second;
            maxId = pair.first;
        }
    }

    cout << "\n--- CLIENTE CON MAS COMPRAS (CANTIDAD DE PRODUCTOS) ---\n";
    if (maxId != -1) {
        cout << "Cliente: " << obtenerNombreCliente(maxId) << endl;
        cout << "Cantidad de productos comprados: " << maxCant << endl;
    } else {
        cout << "No se encontraron datos.\n";
    }
}

// Cliente que mas dinero gasto
void reporteClienteMasGasto() {
    vector<Venta> ventas = cargarVentasReporte();

    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    map<int, double> clienteGasto;
    for (const auto& v : ventas) {
        clienteGasto[v.clienteId] += v.total;
    }

    int maxId = -1;
    double maxGasto = -1.0;

    for (const auto& pair : clienteGasto) {
        if (pair.second > maxGasto) {
            maxGasto = pair.second;
            maxId = pair.first;
        }
    }

    cout << "\n--- CLIENTE QUE MAS DINERO GASTO ---\n";
    if (maxId != -1) {
        cout << "Cliente: " << obtenerNombreCliente(maxId) << endl;
        cout << "Total gastado: " << fixed << setprecision(2) << maxGasto << endl;
    } else {
        cout << "No se encontraron datos.\n";
    }
}

void menuReporteClientes() {
    int op;
    do {
        system("cls");
        cout << "\n===== REPORTE DE CLIENTES =====\n";
        cout << "1. Cliente con mas compras (Cantidad de productos)\n";
        cout << "2. Cliente que mas dinero gasto\n";
        cout << "0. Volver\n";
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: reporteClienteMasProductos(); pausa(); break;
            case 2: reporteClienteMasGasto(); pausa(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while (op != 0);
}

// ======================================================
//  REPORTES DE PRODUCTOS
// ======================================================

// Producto mas vendido (en base a fecha inicial y final)
void reporteProductoMasVendidoRango() {
    char fechaIni[20], fechaFin[20];
    cout << "\nIngrese fecha inicial:\n";
    leerFecha("Fecha (DD/MM/AAAA): ", fechaIni);
    cout << "\nIngrese fecha final:\n";
    leerFecha("Fecha (DD/MM/AAAA): ", fechaFin);

    long long ini = fechaAEntero(fechaIni);
    long long fin = fechaAEntero(fechaFin);

    vector<Venta> ventas = cargarVentasReporte();
    vector<DetalleVenta> detalles = cargarDetallesReporte();

    // Filtrar ventas por fecha
    set<int> ventasValidas;
    for (const auto& v : ventas) {
        long long f = fechaAEntero(v.fecha);
        if (f >= ini && f <= fin) {
            ventasValidas.insert(v.id);
        }
    }

    if (ventasValidas.empty()) {
        cout << "No hay ventas en el rango de fechas seleccionado.\n";
        return;
    }

    map<int, int> productoCant;
    for (const auto& d : detalles) {
        if (ventasValidas.count(d.ventaId)) {
            productoCant[d.productoId] += d.cantidad;
        }
    }

    int maxId = -1;
    int maxCant = -1;

    for (const auto& pair : productoCant) {
        if (pair.second > maxCant) {
            maxCant = pair.second;
            maxId = pair.first;
        }
    }

    cout << "\n--- PRODUCTO MAS VENDIDO (" << fechaIni << " - " << fechaFin << ") ---\n";
    if (maxId != -1) {
        cout << "Producto: " << obtenerNombreProducto(maxId) << endl;
        cout << "Cantidad vendida: " << maxCant << endl;
    } else {
        cout << "No se vendieron productos en este rango.\n";
    }
}

// Producto menos vendido (en base a fecha inicial y final)
void reporteProductoMenosVendidoRango() {
    char fechaIni[20], fechaFin[20];
    cout << "\nIngrese fecha inicial:\n";
    leerFecha("Fecha (DD/MM/AAAA): ", fechaIni);
    cout << "\nIngrese fecha final:\n";
    leerFecha("Fecha (DD/MM/AAAA): ", fechaFin);

    long long ini = fechaAEntero(fechaIni);
    long long fin = fechaAEntero(fechaFin);

    vector<Venta> ventas = cargarVentasReporte();
    vector<DetalleVenta> detalles = cargarDetallesReporte();
    vector<Producto> productos = cargarProductosReporte();

    if (productos.empty()) {
        cout << "No hay productos registrados.\n";
        return;
    }

    // Inicializar conteo en 0 para todos los productos
    map<int, int> productoCant;
    for (const auto& p : productos) {
        productoCant[p.id] = 0;
    }

    // Filtrar ventas por fecha
    set<int> ventasValidas;
    for (const auto& v : ventas) {
        long long f = fechaAEntero(v.fecha);
        if (f >= ini && f <= fin) {
            ventasValidas.insert(v.id);
        }
    }

    // Contar ventas
    for (const auto& d : detalles) {
        if (ventasValidas.count(d.ventaId)) {
            productoCant[d.productoId] += d.cantidad;
        }
    }

    int minId = -1;
    int minCant = 2147483647; // Max int

    for (const auto& pair : productoCant) {
        if (pair.second < minCant) {
            minCant = pair.second;
            minId = pair.first;
        }
    }

    cout << "\n--- PRODUCTO MENOS VENDIDO (" << fechaIni << " - " << fechaFin << ") ---\n";
    if (minId != -1) {
        cout << "Producto: " << obtenerNombreProducto(minId) << endl;
        cout << "Cantidad vendida: " << minCant << endl;
    } else {
        cout << "Error al calcular.\n";
    }
}

// Histograma de productos (stock) usando Gnuplot
void graficoHistogramaStock() {
    vector<Producto> productos = cargarProductosReporte();
    if (productos.empty()) {
        cout << "No hay productos para graficar.\n";
        return;
    }

    vector<double> stocks;
    vector<string> nombres;

    for (const auto& p : productos) {
        stocks.push_back((double)p.stock);
        nombres.push_back(p.nombre);
    }

    Gnuplot gp;
    gp.plot_bar(nombres, stocks, "Histograma de Stock de Productos", "Productos", "Stock");
}

// Grafico de productos y cantidad vendidos usando Gnuplot
void graficoVentasProductos() {
    vector<DetalleVenta> detalles = cargarDetallesReporte();
    vector<Producto> productos = cargarProductosReporte();

    if (productos.empty()) {
        cout << "No hay productos.\n";
        return;
    }

    map<int, int> conteo;
    // Inicializar
    for (const auto& p : productos) {
        conteo[p.id] = 0;
    }

    for (const auto& d : detalles) {
        conteo[d.productoId] += d.cantidad;
    }

    vector<double> cantidades;
    vector<string> nombres;

    for (const auto& p : productos) {
        cantidades.push_back((double)conteo[p.id]);
        nombres.push_back(p.nombre);
    }

    Gnuplot gp;
    gp.plot_bar(nombres, cantidades, "Cantidad de Productos Vendidos", "Productos", "Cantidad Vendida");
}

// Ganancia Total (Tabla y Total)
void reporteGananciaTotal() {
    vector<Venta> ventas = cargarVentasReporte();
    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    double totalGeneral = 0;

    cout << "\n--- REPORTE DE GANANCIA TOTAL ---\n";
    cout << left << setw(10) << "ID Venta" << setw(15) << "Fecha" << setw(30) << "Cliente" << setw(15) << "Total" << endl;
    cout << string(70, '-') << endl;

    for (const auto& v : ventas) {
        cout << left << setw(10) << v.id 
             << setw(15) << v.fecha 
             << setw(30) << obtenerNombreCliente(v.clienteId) 
             << setw(15) << fixed << setprecision(2) << v.total << endl;
        totalGeneral += v.total;
    }
    cout << string(70, '-') << endl;
    cout << "GANANCIA TOTAL ACUMULADA: " << fixed << setprecision(2) << totalGeneral << endl;
}

void menuReporteProductos() {
    int op;
    do {
        system("cls");
        cout << "\n===== REPORTE DE PRODUCTOS =====\n";
        cout << "1. Producto mas vendido (Rango de fechas)\n";
        cout << "2. Producto menos vendido (Rango de fechas)\n";
        cout << "3. Histograma de Stock (Grafico)\n";
        cout << "4. Grafico de Ventas (Cantidad)\n";
        cout << "5. Ganancia Total (Tabla)\n";
        cout << "0. Volver\n";
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: reporteProductoMasVendidoRango(); pausa(); break;
            case 2: reporteProductoMenosVendidoRango(); pausa(); break;
            case 3: graficoHistogramaStock(); pausa(); break;
            case 4: graficoVentasProductos(); pausa(); break;
            case 5: reporteGananciaTotal(); pausa(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while (op != 0);
}

// ======================================================
//  MENU PRINCIPAL DE REPORTES
// ======================================================

void menuReportes() {
    int op;
    do {
        system("cls");
        cout << "\n===== REPORTES =====\n";
        cout << "1. Reporte de Clientes\n";
        cout << "2. Reporte de Productos\n";
        cout << "0. Volver\n";
        
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: menuReporteClientes(); break;
            case 2: menuReporteProductos(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while(op != 0);
}

#endif
