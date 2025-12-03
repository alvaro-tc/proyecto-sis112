#ifndef LIBRERIA_VENTAS_H
#define LIBRERIA_VENTAS_H

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
        nuevaVenta.id = leerEntero("Ingrese ID Venta: ");
        
        for (int id : idsVentas) {
            if (id == nuevaVenta.id) {
                cout << "Error: El ID de venta ya existe. Intente con otro.\n";
                idExiste = true;
                break;
            }
        }
    } while (idExiste);
    
    leerFecha("Fecha (DD/MM/AAAA): ", nuevaVenta.fecha);


    listarClientes();
    nuevaVenta.clienteId = leerEntero("Ingrese ID del Cliente: ");

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
        int prodId = leerEntero("Ingrese ID Producto: ");

        Producto* prodSeleccionado = nullptr;
        for (auto& p : productos) {
            if (p.id == prodId) {
                prodSeleccionado = &p;
                break;
            }
        }

        if (prodSeleccionado) {
            cout << "Stock disponible: " << prodSeleccionado->stock << endl;
            int cantidad = leerEntero("Cantidad a vender: ");

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
        cin.ignore(); // Limpiar buffer despues de leer char
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

vector<VentaArchivo> cargarVentas() {
    vector<VentaArchivo> v;
    ifstream f(ARCHIVO_VENTAS, ios::binary);
    if (!f) return v;
    VentaArchivo venta;
    while (f.read((char*)&venta, sizeof(VentaArchivo))) {
        v.push_back(venta);
    }
    f.close();
    return v;
}

void guardarVentas(const vector<VentaArchivo>& v) {
    ofstream f(ARCHIVO_VENTAS, ios::binary | ios::trunc);
    for (const auto& venta : v) {
        f.write((char*)&venta, sizeof(VentaArchivo));
    }
    f.close();
}

vector<DetalleVentaArchivo> cargarDetalles() {
    vector<DetalleVentaArchivo> v;
    ifstream f(ARCHIVO_DETALLES, ios::binary);
    if (!f) return v;
    DetalleVentaArchivo d;
    while (f.read((char*)&d, sizeof(DetalleVentaArchivo))) {
        v.push_back(d);
    }
    f.close();
    return v;
}

void guardarDetalles(const vector<DetalleVentaArchivo>& v) {
    ofstream f(ARCHIVO_DETALLES, ios::binary | ios::trunc);
    for (const auto& d : v) {
        f.write((char*)&d, sizeof(DetalleVentaArchivo));
    }
    f.close();
}

void eliminarVenta() {
    vector<VentaArchivo> ventas = cargarVentas();
    if (ventas.empty()) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    int id = leerEntero("Ingrese ID de la Venta a eliminar: ");
    bool encontrado = false;
    int indiceVenta = -1;

    for (int i = 0; i < ventas.size(); i++) {
        if (ventas[i].id == id) {
            indiceVenta = i;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Venta no encontrada.\n";
        return;
    }

    // Restaurar Stock
    vector<DetalleVentaArchivo> detalles = cargarDetalles();
    vector<DetalleVentaArchivo> detallesConservar;
    
    // Mapa para acumular stock a restaurar: ProductoID -> Cantidad
    map<int, int> stockRestaurar;

    for (const auto& d : detalles) {
        if (d.ventaId == id) {
            stockRestaurar[d.productoId] += d.cantidad;
        } else {
            detallesConservar.push_back(d);
        }
    }

    // Actualizar archivo de productos
    vector<Producto> productos = cargarProductos();
    bool stockActualizado = false;
    for (auto& p : productos) {
        if (stockRestaurar.count(p.id)) {
            p.stock += stockRestaurar[p.id];
            stockActualizado = true;
        }
    }
    if (stockActualizado) {
        guardarProductos(productos);
    }

    // Eliminar venta del vector
    ventas.erase(ventas.begin() + indiceVenta);

    // Guardar cambios
    guardarVentas(ventas);
    guardarDetalles(detallesConservar);

    cout << "Venta eliminada y stock restaurado correctamente.\n";
}

void editarVenta() {
    int id = leerEntero("Ingrese ID de la Venta a editar: ");
    
    fstream f("ventas.bin", ios::binary | ios::in | ios::out);
    if (!f) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    VentaArchivo v;
    bool encontrado = false;

    while (f.read((char*)&v, sizeof(VentaArchivo))) {
        if (v.id == id) {
            encontrado = true;
            cout << "Datos actuales:\n";
            cout << "Fecha: " << v.fecha << endl;
            cout << "Cliente ID: " << v.clienteId << endl;
            cout << "Total: " << v.total << endl;
            
            cout << "\nIngrese nuevos datos (Solo cabecera):\n";
            leerFecha("Nueva Fecha (DD/MM/AAAA): ", v.fecha);
            
            listarClientes();
            v.clienteId = leerEntero("Nuevo ID Cliente: ");

            long pos = (long)f.tellg() - sizeof(VentaArchivo);
            f.seekp(pos);
            f.write((char*)&v, sizeof(VentaArchivo));
            break;
        }
    }
    f.close();

    if (encontrado) {
        cout << "Venta editada correctamente.\n";
    } else {
        cout << "Venta no encontrada.\n";
    }
}

void buscarVenta() {
    int id = leerEntero("Ingrese ID de la Venta a buscar: ");
    
    ifstream fVentas("ventas.bin", ios::binary);
    if (!fVentas) {
        cout << "No hay ventas registradas.\n";
        return;
    }

    VentaArchivo v;
    bool encontrado = false;

    while (fVentas.read((char*)&v, sizeof(VentaArchivo))) {
        if (v.id == id) {
            encontrado = true;
            cout << "\n--- DETALLES DE LA VENTA ---\n";
            cout << "ID: " << v.id << endl;
            cout << "Fecha: " << v.fecha << endl;
            cout << "Cliente ID: " << v.clienteId << endl;
            cout << "Total: " << v.total << endl;
            cout << "Productos:\n";
            
            ifstream fDetalles("detalles.bin", ios::binary);
            DetalleVentaArchivo d;
            while (fDetalles.read((char*)&d, sizeof(DetalleVentaArchivo))) {
                if (d.ventaId == id) {
                    cout << " - Producto ID: " << d.productoId << " | Cant: " << d.cantidad << " | Subtotal: " << d.precioUnitario * d.cantidad << endl;
                }
            }
            fDetalles.close();
            break;
        }
    }
    fVentas.close();

    if (!encontrado) {
        cout << "Venta no encontrada.\n";
    }
}

void menuVentas() {
    int op;
    do {
        system("cls");
        cout << "\n===== GESTION DE VENTAS =====\n";
        cout << "1. Nueva Venta\n";
        cout << "2. Listar Ventas\n";
        cout << "3. Eliminar Venta\n";
        cout << "4. Editar Venta\n";
        cout << "5. Buscar Venta\n";
        cout << "0. Volver\n";
        
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: nuevaVenta(); pausa(); break;
            case 2: listarVentas(); pausa(); break;
            case 3: eliminarVenta(); pausa(); break;
            case 4: editarVenta(); pausa(); break;
            case 5: buscarVenta(); pausa(); break;
            case 0: break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while(op != 0);
}

#endif
