
// ======================================================
//  VENTAS
// ======================================================
const char* ARCHIVO_VENTAS = "ventas.bin";
const char* ARCHIVO_DETALLES = "detalles.bin";


int obtenerSiguienteIdVenta() {
    ifstream f(ARCHIVO_VENTAS, ios::binary);
    if (!f) return 1;
    VentaArchivo v;
    int maxId = 0;
    while (f.read((char*)&v, sizeof(VentaArchivo))) {
        if (v.id > maxId) maxId = v.id;
    }
    f.close();
    return maxId + 1;
}

void nuevaVenta() {
    vector<Producto> productos = cargarProductos();
    vector<Cliente> clientes = cargarClientes();

    if (productos.empty() || clientes.empty()) {
        cout << "Necesitas productos y clientes para vender.\n";
        return;
    }

    cout << "\n--- NUEVA VENTA ---\n";
    
    VentaArchivo nuevaVenta;

    vector<int> idsVentas;
    ifstream f(ARCHIVO_VENTAS, ios::binary);
    if (f) {
        VentaArchivo v;
        while (f.read((char*)&v, sizeof(VentaArchivo))) {
            idsVentas.push_back(v.id);
        }
        f.close();
    }

    bool idExiste;
    do {
        idExiste = false;
        cout << "Ingrese ID Venta: ";
        cin >> nuevaVenta.id;
        
        for (int id : idsVentas) {
            if (id == nuevaVenta.id) {
                cout << "Error: El ID de venta ya existe. Intente con otro.\n";
                idExiste = true;
                break;
            }
        }
    } while (idExiste);
    
    cout << "Fecha (DD/MM/AAAA): ";
    cin.ignore();
    cin.getline(nuevaVenta.fecha, 20);


    listarClientes();
    cout << "Ingrese ID del Cliente: ";
    cin >> nuevaVenta.clienteId;

    bool clienteValido = false;
    for (const auto& c : clientes) {
        if (c.id == nuevaVenta.clienteId) {
            clienteValido = true;
            break;
        }
    }
    if (!clienteValido) {
        cout << "Cliente no valido.\n";
        return;
    }


    vector<DetalleVentaArchivo> detalles;
    double totalVenta = 0;
    char continuar = 's';

    while (continuar == 's' || continuar == 'S') {
        listarProductos();
        int prodId, cantidad;
        cout << "Ingrese ID Producto: ";
        cin >> prodId;

        Producto* prodSeleccionado = nullptr;
        for (auto& p : productos) {
            if (p.id == prodId) {
                prodSeleccionado = &p;
                break;
            }
        }

        if (prodSeleccionado) {
            cout << "Stock disponible: " << prodSeleccionado->stock << endl;
            cout << "Cantidad a vender: ";
            cin >> cantidad;

            if (cantidad > 0 && cantidad <= prodSeleccionado->stock) {
                prodSeleccionado->stock -= cantidad; // Reducir stock en memoria
                
                DetalleVentaArchivo detalle;
                detalle.ventaId = nuevaVenta.id;
                detalle.productoId = prodId;
                detalle.cantidad = cantidad;
                detalle.precioUnitario = prodSeleccionado->precio;
                
                detalles.push_back(detalle);
                totalVenta += (cantidad * prodSeleccionado->precio);
                cout << "Producto agregado a la venta.\n";
            } else {
                cout << "Stock insuficiente o cantidad invalida.\n";
            }
        } else {
            cout << "Producto no encontrado.\n";
        }

        cout << "Agregar otro producto? (s/n): ";
        cin >> continuar;
    }

    if (detalles.empty()) {
        cout << "Venta cancelada (sin productos).\n";
        return;
    }

    nuevaVenta.total = totalVenta;


    ofstream fVenta(ARCHIVO_VENTAS, ios::binary | ios::app);
    fVenta.write((char*)&nuevaVenta, sizeof(VentaArchivo));
    fVenta.close();


    ofstream fDetalle(ARCHIVO_DETALLES, ios::binary | ios::app);
    for (const auto& d : detalles) {
        fDetalle.write((char*)&d, sizeof(DetalleVentaArchivo));
    }
    fDetalle.close();

    guardarProductos(productos);

    cout << "Venta registrada con exito. Total: " << totalVenta << endl;
}

void listarVentas() {
    ifstream fVenta(ARCHIVO_VENTAS, ios::binary);
    if (!fVenta) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    vector<DetalleVentaArchivo> todosDetalles;
    ifstream fDetalle(ARCHIVO_DETALLES, ios::binary);
    if (fDetalle) {
        DetalleVentaArchivo d;
        while (fDetalle.read((char*)&d, sizeof(DetalleVentaArchivo))) {
            todosDetalles.push_back(d);
        }
        fDetalle.close();
    }

    VentaArchivo v;
    cout << "\n--- HISTORIAL DE VENTAS ---\n";
    while (fVenta.read((char*)&v, sizeof(VentaArchivo))) {
        cout << "ID Venta: " << v.id << " | Fecha: " << v.fecha 
             << " | ID Cliente: " << v.clienteId 
             << " | Total: " << v.total << endl;
        
        cout << "   Detalles:\n";
        for (const auto& d : todosDetalles) {
            if (d.ventaId == v.id) {
                cout << "   - ProdID: " << d.productoId 
                     << " | Cant: " << d.cantidad 
                     << " | P.Unit: " << d.precioUnitario 
                     << " | Subtotal: " << (d.cantidad * d.precioUnitario) << endl;
            }
        }
        cout << string(40, '-') << endl;
    }
    fVenta.close();
}

void menuVentas() {
    int op;
    do {
        system("cls");
        cout << "\n--- GESTION DE VENTAS ---\n";
        cout << "1. Nueva Venta\n";
        cout << "2. Listar Ventas\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op;
        switch(op) {
            case 1: nuevaVenta(); pausa(); break;
            case 2: listarVentas(); pausa(); break;
        }
    } while(op != 0);
}
