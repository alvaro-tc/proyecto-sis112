
// ======================================================
//  CLIENTES
// ======================================================
const char* ARCHIVO_CLIENTES = "clientes.bin";


vector<Cliente> cargarClientes() {
    vector<Cliente> v;
    ifstream f(ARCHIVO_CLIENTES, ios::binary);
    if (!f) return v;
    Cliente c;
    while (f.read((char*)&c, sizeof(Cliente))) {
        v.push_back(c);
    }
    f.close();
    return v;
}

void guardarClientes(const vector<Cliente>& v) {
    ofstream f(ARCHIVO_CLIENTES, ios::binary | ios::trunc);
    for (const auto& c : v) {
        f.write((char*)&c, sizeof(Cliente));
    }
    f.close();
}

void agregarCliente() {
    vector<Cliente> v = cargarClientes();
    Cliente c;
    cout << "\n--- AGREGAR CLIENTE ---\n";
    
    bool idExiste;
    do {
        idExiste = false;
        cout << "Ingrese ID: ";
        cin >> c.id;
        
        for (const auto& cli : v) {
            if (cli.id == c.id) {
                cout << "Error: El ID ya existe. Intente con otro.\n";
                idExiste = true;
                break;
            }
        }
    } while (idExiste);
    cout << "CI: "; cin >> c.ci;
    cout << "Telefono: "; cin >> c.telefono;
    cout << "Nombre: ";
    cin.ignore();
    cin.getline(c.nombre, 50);

    v.push_back(c);
    guardarClientes(v);
    cout << "Cliente agregado correctamente.\n";
}

void listarClientes() {
    vector<Cliente> v = cargarClientes();
    if (v.empty()) {
        cout << "No hay clientes registrados.\n";
        return;
    }
    cout << "\n--- LISTA DE CLIENTES ---\n";
    cout << left << setw(5) << "ID" << setw(15) << "CI" << setw(15) << "Telefono" << setw(20) << "Nombre" << endl;
    cout << string(55, '-') << endl;
    for (const auto& c : v) {
        cout << left << setw(5) << c.id 
             << setw(15) << c.ci 
             << setw(15) << c.telefono 
             << setw(20) << c.nombre << endl;
    }
}

void eliminarCliente() {
    vector<Cliente> v = cargarClientes();
    if (v.empty()) {
        cout << "No hay clientes para eliminar.\n";
        return;
    }
    int id;
    cout << "Ingrese ID del cliente a eliminar: ";
    cin >> id;
    
    auto it = remove_if(v.begin(), v.end(), [id](const Cliente& c){ return c.id == id; });
    if (it != v.end()) {
        v.erase(it, v.end());
        guardarClientes(v);
        cout << "Cliente eliminado.\n";
    } else {
        cout << "Cliente no encontrado.\n";
    }
}

void editarCliente() {
    vector<Cliente> v = cargarClientes();
    if (v.empty()) {
        cout << "No hay clientes para editar.\n";
        return;
    }
    int id;
    cout << "Ingrese ID del cliente a editar: ";
    cin >> id;

    bool encontrado = false;
    for (auto& c : v) {
        if (c.id == id) {
            cout << "Editando cliente: " << c.nombre << endl;
            cout << "Nuevo CI: "; cin >> c.ci;
            cout << "Nuevo Telefono: "; cin >> c.telefono;
            cout << "Nuevo Nombre: ";
            cin.ignore();
            cin.getline(c.nombre, 50);
            encontrado = true;
            break;
        }
    }
    if (encontrado) {
        guardarClientes(v);
        cout << "Cliente actualizado.\n";
    } else {
        cout << "Cliente no encontrado.\n";
    }
}

void buscarCliente() {
    vector<Cliente> v = cargarClientes();
    int id;
    cout << "Ingrese ID a buscar: ";
    cin >> id;
    bool found = false;
    for(const auto& c : v) {
        if(c.id == id) {
            cout << "\nEncontrado:\n";
            cout << "ID: " << c.id << "\nCI: " << c.ci 
                 << "\nTel: " << c.telefono << "\nNombre: " << c.nombre << endl;
            found = true;
            break;
        }
    }
    if(!found) cout << "No encontrado.\n";
}

void menuClientes() {
    int op;
    do {
        system("cls");
        cout << "\n--- GESTION DE CLIENTES ---\n";
        cout << "1. Agregar Cliente\n";
        cout << "2. Listar Clientes\n";
        cout << "3. Eliminar Cliente\n";
        cout << "4. Editar Cliente\n";
        cout << "5. Buscar Cliente\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op;
        switch(op) {
            case 1: agregarCliente(); pausa(); break;
            case 2: listarClientes(); pausa(); break;
            case 3: eliminarCliente(); pausa(); break;
            case 4: editarCliente(); pausa(); break;
            case 5: buscarCliente(); pausa(); break;
        }
    } while(op != 0);
}
