// Modelos: sólo estructuras

struct Producto {
    int id;
    string nombre;
    string descripcion;
    double precio;
    int stock;
};

struct Cliente {
    int id;
    int ci;
    int telefono;
    string nombre;
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

struct Proveedor {
    int id;
    string nombre;
    string telefono;
    string email;
};
