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

vector<Cliente> cargarClientesReporte() {
    vector<Cliente> clientes;
    ifstream f("clientes.bin", ios::binary);
    if (f) {
        Cliente c;
        while (f.read((char*)&c, sizeof(Cliente))) {
            clientes.push_back(c);
        }
        f.close();
    }
    return clientes;
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

//NUEVO

Cliente obtenerCliente(int id) {
    ifstream f("clientes.bin", ios::binary);
    Cliente c = {-1, 0, 0, ""};
    if (f) {
        while (f.read((char*)&c, sizeof(Cliente))) {
            if (c.id == id) {
                f.close();
                return c;
            }
        }
        f.close();
    }
    c.id = -1;
    return c;
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
//  REPORTES DE CLIENTES (SIN CAMBIOS)
// ======================================================

void reporteListarClientesAsc() {
    vector<Cliente> v = cargarClientesReporte();
    if(v.empty()) { cout << "No hay clientes.\n"; return; }
    sort(v.begin(), v.end(), [](const Cliente& a, const Cliente& b) {
        return strcmp(a.nombre, b.nombre) < 0;
    });
    cout << "\n--- LISTA DE CLIENTES (ASCENDENTE) ---\n";
    cout << left << setw(5) << "ID" << setw(15) << "CI" << setw(15) << "Telefono" << setw(20) << "Nombre" << endl;
    cout << string(55, '-') << endl;
    for (const auto& c : v) {
        cout << left << setw(5) << c.id << setw(15) << c.ci << setw(15) << c.telefono << setw(20) << c.nombre << endl;
    }
}

void reporteListarClientesDesc() {
    vector<Cliente> v = cargarClientesReporte();
    if(v.empty()) { cout << "No hay clientes.\n"; return; }
    sort(v.begin(), v.end(), [](const Cliente& a, const Cliente& b) {
        return strcmp(a.nombre, b.nombre) > 0;
    });
    cout << "\n--- LISTA DE CLIENTES (DESCENDENTE) ---\n";
    cout << left << setw(5) << "ID" << setw(15) << "CI" << setw(15) << "Telefono" << setw(20) << "Nombre" << endl;
    cout << string(55, '-') << endl;
    for (const auto& c : v) {
        cout << left << setw(5) << c.id << setw(15) << c.ci << setw(15) << c.telefono << setw(20) << c.nombre << endl;
    }
}

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

// Compras por cliente
void reporteComprasPorCliente() {
    vector<Venta> ventas = cargarVentasReporte();
    vector<DetalleVenta> detalles = cargarDetallesReporte();

    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    listarClientes(); // Mostrar lista de clientes antes de pedir ID
    int clienteId = leerEntero("Ingrese ID del Cliente a consultar: ");

    Cliente cliente = obtenerCliente(clienteId);
    if (cliente.id == -1) {
        cout << "Error: Cliente con ID " << clienteId << " no encontrado.\n";
        return;
    }

    cout << "\n--- REPORTE DE COMPRAS DE CLIENTE ---\n";
    cout << "ID Cliente: " << cliente.id << endl;
    cout << "Nombre: " << cliente.nombre << endl;
    cout << "CI: " << cliente.ci << endl;
    cout << "Telefono: " << cliente.telefono << endl;
    cout << string(40, '=') << endl;

    double totalAcumulado = 0.0;
    bool tieneCompras = false;

    cout << "\nLISTA DE COMPRAS:\n";

    for (const auto& v : ventas) {
        if (v.clienteId == clienteId) {
            tieneCompras = true;
            cout << left << string(50, '-') << endl;
            cout << "Venta ID: " << v.id << " | Fecha: " << v.fecha << " | Total Venta: "
                 << fixed << setprecision(2) << v.total << endl;
            cout << left << setw(10) << "Prod ID" << setw(40) << "Producto" << setw(10) << "Cant" << setw(15) << "P. Unit" << setw(10) << "Subtotal" << endl;
            cout << left << string(85, '-') << endl;

            for (const auto& d : detalles) {
                if (d.ventaId == v.id) {
                    double subtotal = d.cantidad * d.precioUnitario;
                    cout << left << setw(10) << d.productoId
                         << setw(40) << obtenerNombreProducto(d.productoId)
                         << setw(10) << d.cantidad
                         << setw(15) << fixed << setprecision(2) << d.precioUnitario
                         << setw(10) << fixed << setprecision(2) << subtotal << endl;
                }
            }
            totalAcumulado += v.total;
        }
    }

    if (!tieneCompras) {
        cout << "El cliente no tiene ventas registradas.\n";
    } else {
        cout << string(85, '=') << endl;
        cout << "TOTAL ACUMULADO GASTADO POR EL CLIENTE: " << fixed << setprecision(2) << totalAcumulado << endl;
    }
}



void menuReporteClientes() {
    int op;
    do {
        system("cls");
        cout << "\n===== REPORTE DE CLIENTES =====\n";
        cout << "1. Listar Clientes (Ascendente)\n";
        cout << "2. Listar Clientes (Descendente)\n";
        cout << "3. Cliente con mas compras (Cantidad)\n";
        cout << "4. Cliente con mas gasto (Dinero)\n";
        cout << "5. Compras por Cliente\n";
        cout << "0. Volver\n";
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: reporteListarClientesAsc(); pausa(); break;
            case 2: reporteListarClientesDesc(); pausa(); break;
            case 3: reporteClienteMasProductos(); pausa(); break;
            case 4: reporteClienteMasGasto(); pausa(); break;
            case 5: reporteComprasPorCliente(); pausa(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while (op != 0);
}

// ======================================================
//  REPORTES DE PRODUCTOS
// ======================================================

void reporteListarProductosAsc() {
    vector<Producto> v = cargarProductosReporte();
    if(v.empty()) { cout << "No hay productos.\n"; return; }
    sort(v.begin(), v.end(), [](const Producto& a, const Producto& b) {
        return strcmp(a.nombre, b.nombre) < 0;
    });
    cout << "\n--- LISTA DE PRODUCTOS (ASCENDENTE) ---\n";
    cout << left << setw(5) << "ID" << setw(20) << "Nombre" << setw(30) << "Descripcion" << setw(10) << "Precio" << setw(10) << "Stock" << endl;
    cout << string(75, '-') << endl;
    for (const auto& p : v) {
        cout << left << setw(5) << p.id << setw(20) << p.nombre << setw(30) << p.descripcion << setw(10) << p.precio << setw(10) << p.stock << endl;
    }
}

void reporteListarProductosDesc() {
    vector<Producto> v = cargarProductosReporte();
    if(v.empty()) { cout << "No hay productos.\n"; return; }
    sort(v.begin(), v.end(), [](const Producto& a, const Producto& b) {
        return strcmp(a.nombre, b.nombre) > 0;
    });
    cout << "\n--- LISTA DE PRODUCTOS (DESCENDENTE) ---\n";
    cout << left << setw(5) << "ID" << setw(20) << "Nombre" << setw(30) << "Descripcion" << setw(10) << "Precio" << setw(10) << "Stock" << endl;
    cout << string(75, '-') << endl;
    for (const auto& p : v) {
        cout << left << setw(5) << p.id << setw(20) << p.nombre << setw(30) << p.descripcion << setw(10) << p.precio << setw(10) << p.stock << endl;
    }
}

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

// Top 5 Productos mas vendidos
void graficoTop5Productos() {
    vector<DetalleVenta> detalles = cargarDetallesReporte();
    vector<Producto> productos = cargarProductosReporte();

    if (productos.empty()) {
        cout << "No hay productos.\n";
        return;
    }

    map<int, int> conteo;
    for (const auto& d : detalles) {
        conteo[d.productoId] += d.cantidad;
    }

    // Vector de pares para ordenar
    vector<pair<int, int>> lista;
    for (const auto& pair : conteo) {
        lista.push_back({pair.first, pair.second}); // Id, Cantidad
    }

    // Ordenar de mayor a menor
    sort(lista.begin(), lista.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;
    });

    vector<double> topCantidades;
    vector<string> topNombres;

    int limite = min((int)lista.size(), 5);
    for (int i = 0; i < limite; ++i) {
        topCantidades.push_back((double)lista[i].second);
        topNombres.push_back(obtenerNombreProducto(lista[i].first));
    }

    if (topNombres.empty()) {
        cout << "No hay ventas registradas para generar el Top 5.\n";
        return;
    }

    Gnuplot gp;
    gp.plot_bar(topNombres, topCantidades, "Top 5 Productos Mas Vendidos", "Productos", "Cantidad");
}

// Ventas por dia (Grafico)
void graficoVentasPorDia() {
    vector<Venta> ventas = cargarVentasReporte();
    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    // Mapa: Fecha (string) -> Total (double)
    // Usamos map para que se ordenen automaticamente por fecha si el string es consistente o usamos un custom comparator?
    // Las fechas son DD/MM/AAAA, orden alfabetico NO es cronologico.
    // Usaremos un map con llave long long (YYYYMMDD) para ordenar cronologicamente
    map<long long, double> ventasDia; // FechaInt -> Monto
    map<long long, string> fechaStr;  // FechaInt -> FechaString

    for (const auto& v : ventas) {
        long long f = fechaAEntero(v.fecha);
        if (f != 0) {
            ventasDia[f] += v.total;
            if (fechaStr.find(f) == fechaStr.end()) {
                fechaStr[f] = string(v.fecha);
            }
        }
    }

    vector<double> montos;
    vector<string> fechas;

    for (const auto& pair : ventasDia) {
        fechas.push_back(fechaStr[pair.first]);
        montos.push_back(pair.second);
    }

    if (fechas.empty()) {
        cout << "No se pudieron procesar las fechas.\n";
        return;
    }

    Gnuplot gp;
    gp.plot_bar(fechas, montos, "Ventas Totales por Dia", "Fecha", "Monto Total (Bs)");
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

void reporteVentasMontoAsc() {
    vector<Venta> ventas = cargarVentasReporte();
    if(ventas.empty()) { cout << "No hay ventas.\n"; return; }
    
    // Ordered by Amount Ascending
    sort(ventas.begin(), ventas.end(), [](const Venta& a, const Venta& b) {
        return a.total < b.total;
    });

    cout << "\n--- VENTAS POR MONTO (ASCENDENTE) ---\n";
    cout << left << setw(10) << "ID Venta" << setw(15) << "Fecha" << setw(30) << "Cliente" << setw(15) << "Total" << endl;
    cout << string(70, '-') << endl;

    for (const auto& v : ventas) {
        cout << left << setw(10) << v.id
             << setw(15) << v.fecha
             << setw(30) << obtenerNombreCliente(v.clienteId)
             << setw(15) << fixed << setprecision(2) << v.total << endl;
    }
}

void reporteVentasMontoDesc() {
    vector<Venta> ventas = cargarVentasReporte();
    if(ventas.empty()) { cout << "No hay ventas.\n"; return; }
    
    // Ordered by Amount Descending
    sort(ventas.begin(), ventas.end(), [](const Venta& a, const Venta& b) {
        return a.total > b.total;
    });

    cout << "\n--- VENTAS POR MONTO (DESCENDENTE) ---\n";
    cout << left << setw(10) << "ID Venta" << setw(15) << "Fecha" << setw(30) << "Cliente" << setw(15) << "Total" << endl;
    cout << string(70, '-') << endl;

    for (const auto& v : ventas) {
        cout << left << setw(10) << v.id
             << setw(15) << v.fecha
             << setw(30) << obtenerNombreCliente(v.clienteId)
             << setw(15) << fixed << setprecision(2) << v.total << endl;
    }
}




void reporteVentasRango() {
    char fechaIni[20], fechaFin[20];
    cout << "\nIngrese fecha inicial:\n";
    leerFecha("Fecha (DD/MM/AAAA): ", fechaIni);
    cout << "\nIngrese fecha final:\n";
    leerFecha("Fecha (DD/MM/AAAA): ", fechaFin);

    long long ini = fechaAEntero(fechaIni);
    long long fin = fechaAEntero(fechaFin);

    vector<Venta> ventas = cargarVentasReporte();
    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    double totalRango = 0.0;
    bool hayVentas = false;

    cout << "\n--- VENTAS EN EL RANGO (" << fechaIni << " - " << fechaFin << ") ---\n";
    cout << left << setw(10) << "ID Venta" << setw(15) << "Fecha" << setw(30) << "Cliente" << setw(15) << "Total" << endl;
    cout << string(70, '-') << endl;

    for (const auto& v : ventas) {
        long long f = fechaAEntero(v.fecha);
        if (f >= ini && f <= fin) {
            hayVentas = true;
            cout << left << setw(10) << v.id
                 << setw(15) << v.fecha
                 << setw(30) << obtenerNombreCliente(v.clienteId)
                 << setw(15) << fixed << setprecision(2) << v.total << endl;
            totalRango += v.total;
        }
    }

    if (!hayVentas) {
        cout << "No se encontraron ventas en este rango.\n";
    } else {
        cout << string(70, '-') << endl;
        cout << "TOTAL EN RANGO: " << fixed << setprecision(2) << totalRango << endl;
    }
}


void reporteVentasPorProductoEspecifico() {
    vector<Venta> ventas = cargarVentasReporte();
    vector<DetalleVenta> detalles = cargarDetallesReporte();

    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    listarProductos(); // Mostrar lista de productos antes de pedir ID
    int productoId = leerEntero("Ingrese ID del Producto a consultar: ");
    string nombreProducto = obtenerNombreProducto(productoId);

    if (nombreProducto == "Desconocido") {
        cout << "Error: Producto con ID " << productoId << " no encontrado.\n";
        return;
    }

    cout << "\n--- VENTAS PARA EL PRODUCTO: " << nombreProducto << " ---\n";
    cout << string(80, '=') << endl;

    map<int, Venta> mapaVentas;
    for(const auto& v : ventas) {
        mapaVentas[v.id] = v;
    }

    double totalProducto = 0.0;
    bool tieneVentas = false;

    cout << left << setw(10) << "Venta ID" << setw(15) << "Fecha" << setw(30) << "Cliente"
         << setw(10) << "Cant" << setw(15) << "Subtotal" << endl;
    cout << string(80, '-') << endl;

    for (const auto& d : detalles) {
        if (d.productoId == productoId && mapaVentas.count(d.ventaId)) {
            tieneVentas = true;
            const Venta& v = mapaVentas.at(d.ventaId);
            string nombreCliente = obtenerNombreCliente(v.clienteId);
            double subtotal = d.cantidad * d.precioUnitario;

            cout << left << setw(10) << d.ventaId
                 << setw(15) << v.fecha
                 << setw(30) << nombreCliente
                 << setw(10) << d.cantidad
                 << setw(15) << fixed << setprecision(2) << subtotal << endl;

            totalProducto += subtotal;
        }
    }

    if (!tieneVentas) {
        cout << "Este producto no tiene ventas registradas.\n";
    } else {
        cout << string(80, '=') << endl;
        cout << "TOTAL GENERADO POR EL PRODUCTO '" << nombreProducto << "': "
             << fixed << setprecision(2) << totalProducto << endl;
    }
}


void menuReporteProductos() {
    int op;
    do {
        system("cls");
        cout << "\n===== REPORTE DE PRODUCTOS =====\n";
        cout << "1. Listar Productos (Ascendente)\n";
        cout << "2. Listar Productos (Descendente)\n";
        cout << "3. Producto mas vendido (Rango)\n";
        cout << "4. Producto menos vendido (Rango)\n";
        cout << "5. Ventas por Producto\n";
        cout << "0. Volver\n";
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: reporteListarProductosAsc(); pausa(); break;
            case 2: reporteListarProductosDesc(); pausa(); break;
            case 3: reporteProductoMasVendidoRango(); pausa(); break;
            case 4: reporteProductoMenosVendidoRango(); pausa(); break;
            case 5: reporteVentasPorProductoEspecifico(); pausa(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while (op != 0);
}

void menuReporteVentas() {
    int op;
    do {
        system("cls");
        cout << "\n===== REPORTE DE VENTAS =====\n";
        cout << "1. Listar Ventas (Ascendente)\n";
        cout << "2. Listar Ventas (Descendente)\n";
        cout << "3. Ventas por Monto (Ascendente)\n";
        cout << "4. Ventas por Monto (Descendente)\n";
        cout << "5. Ventas (Rango)\n";
        cout << "6. Ganancia Total\n";
        cout << "0. Volver\n";
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: listarVentasOrdenadas(true); pausa(); break;
            case 2: listarVentasOrdenadas(false); pausa(); break;
            case 3: reporteVentasMontoAsc(); pausa(); break;
            case 4: reporteVentasMontoDesc(); pausa(); break;
            case 5: reporteVentasRango(); pausa(); break;
            case 6: reporteGananciaTotal(); pausa(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while(op != 0);
}

// ======================================================
//  GRAFICOS NUEVOS Y MENU DE GRAFICOS
// ======================================================

// Top 5 Clientes que mas dinero gastaron
void graficoTop5Clientes() {
    vector<Venta> ventas = cargarVentasReporte();
    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    map<int, double> clienteGasto;
    for (const auto& v : ventas) {
        clienteGasto[v.clienteId] += v.total;
    }

    vector<pair<int, double>> lista;
    for (const auto& pair : clienteGasto) {
        lista.push_back({pair.first, pair.second});
    }

    sort(lista.begin(), lista.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });

    vector<double> topMontos;
    vector<string> topNombres;

    int limite = min((int)lista.size(), 5);
    for (int i = 0; i < limite; ++i) {
        topMontos.push_back(lista[i].second);
        topNombres.push_back(obtenerNombreCliente(lista[i].first));
    }

    if (topNombres.empty()) {
        cout << "No hay datos para generar el reporte.\n";
        return;
    }

    Gnuplot gp;
    gp.plot_bar(topNombres, topMontos, "Top 5 Clientes (Mayor Gasto)", "Clientes", "Monto Total (Bs)");
}

// Top 10 Productos con mayor stock
void graficoTop10Stock() {
    vector<Producto> productos = cargarProductosReporte();
    if (productos.empty()) {
        cout << "No hay productos.\n";
        return;
    }

    sort(productos.begin(), productos.end(), [](const Producto& a, const Producto& b) {
        return a.stock > b.stock;
    });

    vector<double> stocks;
    vector<string> nombres;

    int limite = min((int)productos.size(), 10);
    for (int i = 0; i < limite; ++i) {
        stocks.push_back((double)productos[i].stock);
        nombres.push_back(productos[i].nombre);
    }

    Gnuplot gp;
    gp.plot_bar(nombres, stocks, "Top 10 Productos con Mayor Stock", "Productos", "Stock Disponible");
}


void menuGraficos() {
    int op;
    do {
        system("cls");
        cout << "\n===== SECCION DE GRAFICOS =====\n";
        cout << "1. Ventas Totales por Dia\n";
        cout << "2. Histograma de Stock (Todos)\n";
        cout << "3. Cantidad de Productos Vendidos (Todos)\n";
        cout << "4. Top 5 Productos Mas Vendidos\n";
        cout << "5. Top 5 Clientes (Mayor Gasto)\n";
        cout << "6. Top 10 Productos con Mayor Stock\n";
        cout << "0. Volver\n";
        
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: graficoVentasPorDia(); pausa(); break;
            case 2: graficoHistogramaStock(); pausa(); break;
            case 3: graficoVentasProductos(); pausa(); break;
            case 4: graficoTop5Productos(); pausa(); break;
            case 5: graficoTop5Clientes(); pausa(); break;
            case 6: graficoTop10Stock(); pausa(); break;
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
        cout << "3. Reporte de Ventas\n";
        cout << "4. SECCION DE GRAFICOS\n";
        cout << "0. Volver\n";

        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: menuReporteClientes(); break;
            case 2: menuReporteProductos(); break;
            case 3: menuReporteVentas(); break;
            case 4: menuGraficos(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while(op != 0);
}

#endif
