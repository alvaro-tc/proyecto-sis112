#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iomanip>

using namespace std;

// ======================================================
//  MODELOS 
// ======================================================

struct Producto {
    int id;
    char nombre[50];
    char descripcion[100];
    double precio;
    int stock;
};

struct Cliente {
    int id;
    int ci;
    int telefono;
    char nombre[50];
};


struct VentaArchivo {
    int id;
    char fecha[20];
    int clienteId;
    double total;
};

struct DetalleVentaArchivo {
    int ventaId;
    int productoId;
    int cantidad;
    double precioUnitario;
};


struct DetalleVenta {
    int productoId;
    int cantidad;
    double precioUnitario;
};

struct Venta {
    int id;
    string fecha;
    int clienteId;
    vector<DetalleVenta> items;
    double total;
};
