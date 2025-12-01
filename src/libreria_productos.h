
// ======================================================
//  PRODUCTOS
// ======================================================
const char* ARCHIVO_PRODUCTOS = "productos.dat";

vector<Producto> cargarProductos() {
    vector<Producto> v;
    ifstream f(ARCHIVO_PRODUCTOS, ios::binary);
    if (!f) return v;
    Producto p;
    while (f.read((char*)&p, sizeof(Producto))) {
        v.push_back(p);
    }
    f.close();
    return v;
}

void guardarProductos(const vector<Producto>& v) {
    ofstream f(ARCHIVO_PRODUCTOS, ios::binary | ios::trunc);
    for (const auto& p : v) {
        f.write((char*)&p, sizeof(Producto));
    }
    f.close();
}

void agregarProducto() {
    vector<Producto> v = cargarProductos();
    Producto p;
    
    cout << "\n--- AGREGAR PRODUCTO ---\n";
    
    bool idExiste;
    do {
        idExiste = false;
        cout << "Ingrese ID: ";
        cin >> p.id;
        
        for (const auto& prod : v) {
            if (prod.id == p.id) {
                cout << "Error: El ID ya existe. Intente con otro.\n";
                idExiste = true;
                break;
            }
        }
    } while (idExiste);
    
    cout << "Nombre: ";
    cin.ignore();
    cin.getline(p.nombre, 50);
    
    cout << "Descripcion: ";
    cin.getline(p.descripcion, 100);
    
    cout << "Precio: ";
    cin >> p.precio;
    
    cout << "Stock: ";
    cin >> p.stock;

    v.push_back(p);
    guardarProductos(v);
    cout << "Producto agregado correctamente.\n";
}

void listarProductos() {
    vector<Producto> v = cargarProductos();
    if (v.empty()) {
        cout << "No hay productos registrados.\n";
        return;
    }
    cout << "\n--- LISTA DE PRODUCTOS ---\n";
    cout << left << setw(5) << "ID" << setw(20) << "Nombre" << setw(30) << "Descripcion" << setw(10) << "Precio" << setw(10) << "Stock" << endl;
    cout << string(75, '-') << endl;
    for (const auto& p : v) {
        cout << left << setw(5) << p.id 
             << setw(20) << p.nombre 
             << setw(30) << p.descripcion 
             << setw(10) << p.precio 
             << setw(10) << p.stock << endl;
    }
}

void eliminarProducto() {
    vector<Producto> v = cargarProductos();
    if (v.empty()) {
        cout << "No hay productos para eliminar.\n";
        return;
    }
    int id;
    cout << "Ingrese ID del producto a eliminar: ";
    cin >> id;

    auto it = remove_if(v.begin(), v.end(), [id](const Producto& p){ return p.id == id; });
    
    if (it != v.end()) {
        v.erase(it, v.end());
        guardarProductos(v);
        cout << "Producto eliminado.\n";
    } else {
        cout << "Producto no encontrado.\n";
    }
}

void editarProducto() {
    vector<Producto> v = cargarProductos();
    if (v.empty()) {
        cout << "No hay productos para editar.\n";
        return;
    }
    int id;
    cout << "Ingrese ID del producto a editar: ";
    cin >> id;

    bool encontrado = false;
    for (auto& p : v) {
        if (p.id == id) {
            cout << "Editando producto: " << p.nombre << endl;
            cout << "Nuevo nombre: ";
            cin.ignore();
            cin.getline(p.nombre, 50);
            cout << "Nueva descripcion: ";
            cin.getline(p.descripcion, 100);
            cout << "Nuevo precio: ";
            cin >> p.precio;
            cout << "Nuevo stock: ";
            cin >> p.stock;
            encontrado = true;
            break;
        }
    }

    if (encontrado) {
        guardarProductos(v);
        cout << "Producto actualizado.\n";
    } else {
        cout << "Producto no encontrado.\n";
    }
}

void buscarProducto() {
     vector<Producto> v = cargarProductos();
     int id;
     cout << "Ingrese ID a buscar: ";
     cin >> id;
     bool found = false;
     for(const auto& p : v) {
         if(p.id == id) {
             cout << "\nEncontrado:\n";
             cout << "ID: " << p.id << "\nNombre: " << p.nombre 
                  << "\nDesc: " << p.descripcion << "\nPrecio: " << p.precio 
                  << "\nStock: " << p.stock << endl;
             found = true;
             break;
         }
     }
     if(!found) cout << "No encontrado.\n";
}

void menuProductos() {
    int op;
    do {
        system("cls");
        cout << "\n--- GESTION DE PRODUCTOS ---\n";
        cout << "1. Agregar Producto\n";
        cout << "2. Listar Productos\n";
        cout << "3. Eliminar Producto\n";
        cout << "4. Editar Producto\n";
        cout << "5. Buscar Producto\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op;
        switch(op) {
            case 1: agregarProducto(); pausa(); break;
            case 2: listarProductos(); pausa(); break;
            case 3: eliminarProducto(); pausa(); break;
            case 4: editarProducto(); pausa(); break;
            case 5: buscarProducto(); pausa(); break;
        }
    } while(op != 0);
}
