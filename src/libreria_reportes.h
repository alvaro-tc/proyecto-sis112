#ifndef LIBRERIA_REPORTES_H
#define LIBRERIA_REPORTES_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Funciones auxiliares para cargar datos (similares a las de otras librerias pero aqui para lectura)
// Nota: Idealmente deberian estar en un modulo compartido, pero las duplicare aqui por simplicidad y desacoplamiento rapido.

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

// 1. Producto mas vendido
void reporteProductoMasVendido() {
    vector<DetalleVenta> detalles = cargarDetallesReporte();
    if (detalles.empty()) {
        cout << "No hay ventas registradas para generar este reporte.\n";
        return;
    }

    map<int, int> conteo;
    for (const auto& d : detalles) {
        conteo[d.productoId] += d.cantidad;
    }

    int maxId = -1;
    int maxCant = -1;

    for (const auto& pair : conteo) {
        if (pair.second > maxCant) {
            maxCant = pair.second;
            maxId = pair.first;
        }
    }

    if (maxId != -1) {
        cout << "\n--- PRODUCTO MAS VENDIDO ---\n";
        cout << "Producto: " << obtenerNombreProducto(maxId) << endl;
        cout << "Cantidad Total Vendida: " << maxCant << endl;
    }
}

// 2. Cliente con mas compras (cantidad de ventas, no monto)
// O podria ser monto, pero el requerimiento dice "compro mas cosas", asumire cantidad de items o veces que compro.
// "que persona compro mas cosas" -> puede ser cantidad de productos total.
void reporteClienteMasCompras() {
    vector<Venta> ventas = cargarVentasReporte();
    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    // Contaremos cantidad de ventas por cliente
    map<int, int> conteo;
    for (const auto& v : ventas) {
        conteo[v.clienteId]++;
    }

    int maxId = -1;
    int maxVentas = -1;

    for (const auto& pair : conteo) {
        if (pair.second > maxVentas) {
            maxVentas = pair.second;
            maxId = pair.first;
        }
    }

    if (maxId != -1) {
        cout << "\n--- CLIENTE CON MAS COMPRAS (Frecuencia) ---\n";
        cout << "Cliente: " << obtenerNombreCliente(maxId) << endl;
        cout << "Ventas Realizadas: " << maxVentas << endl;
    }
}

// 3. Venta mas grande (Monto total)
void reporteVentaMasGrande() {
    vector<Venta> ventas = cargarVentasReporte();
    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    Venta maxVenta = ventas[0];
    for (const auto& v : ventas) {
        if (v.total > maxVenta.total) {
            maxVenta = v;
        }
    }

    cout << "\n--- VENTA MAS GRANDE (Monto) ---\n";
    cout << "ID Venta: " << maxVenta.id << endl;
    cout << "Cliente: " << obtenerNombreCliente(maxVenta.clienteId) << endl;
    cout << "Monto Total: " << maxVenta.total << endl;
    cout << "Fecha: " << maxVenta.fecha << endl;
}

// 4. Venta con mas productos (Cantidad de items)
void reporteVentaMasProductos() {
    vector<DetalleVenta> detalles = cargarDetallesReporte();
    if (detalles.empty()) {
        cout << "No hay detalles de ventas.\n";
        return;
    }

    map<int, int> conteoItems; // VentaId -> CantidadItems
    for (const auto& d : detalles) {
        conteoItems[d.ventaId] += d.cantidad;
    }

    int maxVentaId = -1;
    int maxItems = -1;

    for (const auto& pair : conteoItems) {
        if (pair.second > maxItems) {
            maxItems = pair.second;
            maxVentaId = pair.first;
        }
    }

    if (maxVentaId != -1) {
        cout << "\n--- VENTA CON MAS PRODUCTOS ---\n";
        cout << "ID Venta: " << maxVentaId << endl;
        cout << "Cantidad de Productos: " << maxItems << endl;
        // Podriamos buscar la venta para mostrar mas detalles si fuera necesario
    }
}

// 5. Producto de mayor precio
void reporteProductoMayorPrecio() {
    ifstream f("productos.bin", ios::binary);
    if (!f) {
        cout << "No hay productos registrados.\n";
        return;
    }

    Producto p;
    Producto maxP;
    bool primero = true;

    while (f.read((char*)&p, sizeof(Producto))) {
        if (primero) {
            maxP = p;
            primero = false;
        } else {
            if (p.precio > maxP.precio) {
                maxP = p;
            }
        }
    }
    f.close();

    if (!primero) {
        cout << "\n--- PRODUCTO DE MAYOR PRECIO ---\n";
        cout << "ID: " << maxP.id << endl;
        cout << "Nombre: " << maxP.nombre << endl;
        cout << "Precio: " << maxP.precio << endl;
    } else {
        cout << "No hay productos.\n";
    }
}

// 6. Producto de menor precio
void reporteProductoMenorPrecio() {
    ifstream f("productos.bin", ios::binary);
    if (!f) {
        cout << "No hay productos registrados.\n";
        return;
    }

    Producto p;
    Producto minP;
    bool primero = true;

    while (f.read((char*)&p, sizeof(Producto))) {
        if (primero) {
            minP = p;
            primero = false;
        } else {
            if (p.precio < minP.precio) {
                minP = p;
            }
        }
    }
    f.close();

    if (!primero) {
        cout << "\n--- PRODUCTO DE MENOR PRECIO ---\n";
        cout << "ID: " << minP.id << endl;
        cout << "Nombre: " << minP.nombre << endl;
        cout << "Precio: " << minP.precio << endl;
    } else {
        cout << "No hay productos.\n";
    }
}

// 7. Ganancia Total
void reporteGananciaTotal() {
    vector<Venta> ventas = cargarVentasReporte();
    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    double totalGanancia = 0;
    for (const auto& v : ventas) {
        totalGanancia += v.total;
    }

    cout << "\n--- GANANCIA TOTAL ---\n";
    cout << "Suma de todas las ventas: " << totalGanancia << endl;
}

// 8. Cantidad Total de Productos Vendidos
void reporteCantidadTotalProductos() {
    vector<DetalleVenta> detalles = cargarDetallesReporte();
    if (detalles.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    int totalProductos = 0;
    for (const auto& d : detalles) {
        totalProductos += d.cantidad;
    }

    cout << "\n--- CANTIDAD TOTAL DE PRODUCTOS VENDIDOS ---\n";
    cout << "Total de items vendidos: " << totalProductos << endl;
}

// 9. Venta con Menor Total
void reporteVentaMenorTotal() {
    vector<Venta> ventas = cargarVentasReporte();
    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    Venta minVenta = ventas[0];
    for (const auto& v : ventas) {
        if (v.total < minVenta.total) {
            minVenta = v;
        }
    }

    cout << "\n--- VENTA CON MENOR TOTAL ---\n";
    cout << "ID Venta: " << minVenta.id << endl;
    cout << "Cliente: " << obtenerNombreCliente(minVenta.clienteId) << endl;
    cout << "Monto Total: " << minVenta.total << endl;
    cout << "Fecha: " << minVenta.fecha << endl;
}

void menuReportes() {
    int op;
    do {
        system("cls");
        cout << "\n===== REPORTES =====\n";
        cout << "1. Producto mas vendido\n";
        cout << "2. Cliente con mas compras\n";
        cout << "3. Venta mas grande (Monto)\n";
        cout << "4. Venta con mas productos\n";
        cout << "5. Producto de mayor precio\n";
        cout << "6. Producto de menor precio\n";
        cout << "7. Ganancia Total\n";
        cout << "8. Cantidad Total de Productos Vendidos\n";
        cout << "9. Venta con Menor Total\n";
        cout << "0. Volver\n";
        
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: reporteProductoMasVendido(); pausa(); break;
            case 2: reporteClienteMasCompras(); pausa(); break;
            case 3: reporteVentaMasGrande(); pausa(); break;
            case 4: reporteVentaMasProductos(); pausa(); break;
            case 5: reporteProductoMayorPrecio(); pausa(); break;
            case 6: reporteProductoMenorPrecio(); pausa(); break;
            case 7: reporteGananciaTotal(); pausa(); break;
            case 8: reporteCantidadTotalProductos(); pausa(); break;
            case 9: reporteVentaMenorTotal(); pausa(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while(op != 0);
}

#endif


