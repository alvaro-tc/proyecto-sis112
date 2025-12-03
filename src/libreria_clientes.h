#ifndef LIBRERIA_CLIENTES_H
#define LIBRERIA_CLIENTES_H

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
        c.id = leerEntero("Ingrese ID: ");
        
        for (const auto& cli : v) {
            if (cli.id == c.id) {
                cout << "Error: El ID ya existe. Intente con otro.\n";
                idExiste = true;
                break;
            }
        }
    } while (idExiste);
    c.ci = leerEntero("CI: ");
    c.telefono = leerEntero("Telefono: ");
    cout << "Nombre: ";
    // cin.ignore(); // No necesario si venimos de leerEntero
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

    int id = leerEntero("Ingrese ID del cliente a eliminar: ");

    // Verificacion de integridad: Verificar si el cliente tiene ventas
    ifstream fVentas("ventas.bin", ios::binary);
    if (fVentas) {
        VentaArchivo venta;
        while (fVentas.read((char*)&venta, sizeof(VentaArchivo))) {
            if (venta.clienteId == id) {
                cout << "Error: No se puede eliminar el cliente porque tiene ventas registradas.\n";
                fVentas.close();
                return;
            }
        }
        fVentas.close();
    }

    bool encontrado = false;
    for (int i = 0; i < v.size(); i++) {
        if (v[i].id == id) {
            v.erase(v.begin() + i);
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Cliente no encontrado.\n";
        return;
    }
    guardarClientes(v);
    cout << "Cliente eliminado correctamente.\n";
}

void editarCliente() {
    vector<Cliente> v = cargarClientes();
    if (v.empty()) {
        cout << "No hay clientes para editar.\n";
        return;
    }
    int id = leerEntero("Ingrese ID del cliente a editar: ");

    bool encontrado = false;
    for (auto& c : v) {
        if (c.id == id) {
            cout << "Editando cliente: " << c.nombre << endl;
            c.ci = leerEntero("Nuevo CI: ");
            c.telefono = leerEntero("Nuevo Telefono: ");
            cout << "Nuevo Nombre: ";
            // cin.ignore(); // No necesario
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
    int id = leerEntero("Ingrese ID a buscar: ");
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
        
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: agregarCliente(); pausa(); break;
            case 2: listarClientes(); pausa(); break;
            case 3: eliminarCliente(); pausa(); break;
            case 4: editarCliente(); pausa(); break;
            case 5: buscarCliente(); pausa(); break;
        }
    } while(op != 0);
}

#endif
