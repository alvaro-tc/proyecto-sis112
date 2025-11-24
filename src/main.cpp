#include "librerias.h"
#include "librerias.h"
#include "modelos.h"
#include <fstream>
#include <cstdlib>
#include <cstdint>

void writeProducto(ofstream &out, const Producto &p) {
    out.write((char*)&p.id, sizeof(p.id));
    uint32_t nl = (uint32_t)p.nombre.size();
    out.write((char*)&nl, sizeof(nl));
    out.write(p.nombre.data(), nl);
    uint32_t dl = (uint32_t)p.descripcion.size();
    out.write((char*)&dl, sizeof(dl));
    out.write(p.descripcion.data(), dl);
    out.write((char*)&p.precio, sizeof(p.precio));
    out.write((char*)&p.stock, sizeof(p.stock));
}

bool readProducto(ifstream &in, Producto &p) {
    if (!in.read((char*)&p.id, sizeof(p.id))) return false;
    uint32_t nl = 0;
    if (!in.read((char*)&nl, sizeof(nl))) return false;
    p.nombre.resize(nl);
    if (nl>0) in.read(&p.nombre[0], nl);
    uint32_t dl = 0;
    if (!in.read((char*)&dl, sizeof(dl))) return false;
    p.descripcion.resize(dl);
    if (dl>0) in.read(&p.descripcion[0], dl);
    if (!in.read((char*)&p.precio, sizeof(p.precio))) return false;
    if (!in.read((char*)&p.stock, sizeof(p.stock))) return false;
    return true;
}

int obtenerNextId(const string &nf) {
    ifstream in(nf, ios::binary);
    if (!in) return 1;
    Producto p;
    int maxId = 0;
    while (readProducto(in, p)) {
        if (p.id > maxId) maxId = p.id;
    }
    return maxId + 1;
}

void escribe(const string &nf) {
    ofstream out(nf, ios::binary | ios::app);
    if (!out) { cout << "No se puede abrir archivo para escribir\n"; return; }
    int n;
    cout << "Cuantos productos almacenara: ";
    cin >> n;
    int nextId = obtenerNextId(nf);
    for (int i = 0; i < n; ++i) {
        Producto p;
        p.id = nextId++;
        cout << "Nombre: "; cin >> p.nombre;
        cout << "Descripcion: "; cin >> p.descripcion;
        cout << "Precio: "; cin >> p.precio;
        cout << "Stock: "; cin >> p.stock;
        writeProducto(out, p);
        cout << "Producto agregado con ID " << p.id << "\n";
    }
    out.close();
}

void lee(const string &nf) {
    ifstream in(nf, ios::binary);
    if (!in) { cout << "Archivo no encontrado\n"; return; }
    Producto p;
    while (readProducto(in, p)) {
        cout << "ID: " << p.id << " | " << p.nombre << " | " << p.descripcion << " | Precio: " << p.precio << " | Stock: " << p.stock << "\n";
    }
    in.close();
}

void buscar(const string &nf) {
    string nombre;
    cout << "Ingresa el nombre del producto a buscar: "; cin >> nombre;
    bool flag = false;
    ifstream in(nf, ios::binary);
    if (!in) { cout << "Archivo no encontrado\n"; return; }
    Producto p;
    while (readProducto(in, p)) {
        if (p.nombre == nombre) { flag = true; }
    }
    in.close();
    if (flag) cout << "El producto: " << nombre << " esta en el archivo\n";
    else cout << "El producto: " << nombre << " no esta en el archivo\n";
}

void frecuencia(const string &nf) {
    string nombre;
    cout << "Ingresa el nombre del producto a contar: "; cin >> nombre;
    int contador = 0;
    ifstream in(nf, ios::binary);
    if (!in) { cout << "Archivo no encontrado\n"; return; }
    Producto p;
    while (readProducto(in, p)) {
        if (p.nombre == nombre) contador++;
    }
    in.close();
    cout << "El producto: " << nombre << " se repite: " << contador << " veces\n";
}

void eliminar(const string &nf) {
    ifstream in(nf, ios::binary);
    if (!in) { cout << "No existe el archivo\n"; return; }
    ofstream out("temporal.bin", ios::binary | ios::trunc);
    string nombre;
    cout << "Ingresar el nombre del producto a eliminar: "; cin >> nombre;
    Producto p;
    while (readProducto(in, p)) {
        if (p.nombre != nombre) writeProducto(out, p);
    }
    in.close(); out.close();
    remove(nf.c_str());
    rename("temporal.bin", nf.c_str());
    cout << "Eliminacion completada.\n";
}

void editar(const string &nf) {
    string nombre;
    cout << "Ingresa el nombre del producto a buscar: "; cin >> nombre;
    ifstream in(nf, ios::binary);
    if (!in) { cout << "Archivo no encontrado\n"; return; }
    vector<Producto> lista;
    Producto p;
    bool flag = false;
    while (readProducto(in, p)) {
        if (p.nombre == nombre) flag = true;
        lista.push_back(p);
    }
    in.close();
    if (!flag) { cout << "El producto: " << nombre << " no esta en el archivo por lo que no se puede editar\n"; return; }
    string nuevoNombre, nuevaDesc;
    double precio; int stock;
    cout << "Nuevo nombre: "; cin >> nuevoNombre;
    cout << "Nueva descripcion: "; cin >> nuevaDesc;
    cout << "Nuevo precio: "; cin >> precio;
    cout << "Nuevo stock: "; cin >> stock;
    for (auto &it : lista) {
        if (it.nombre == nombre) {
            it.nombre = nuevoNombre;
            it.descripcion = nuevaDesc;
            it.precio = precio;
            it.stock = stock;
        }
    }
    ofstream out("temporal.bin", ios::binary | ios::trunc);
    for (auto &it : lista) writeProducto(out, it);
    out.close();
    remove(nf.c_str());
    rename("temporal.bin", nf.c_str());
    cout << "Edicion completada.\n";
}

int main() {
    string nf = "productos.bin";
    int opcion = -1;
    while (opcion != 0) {
        cout << "----------------- MENU DE OPCIONES ---------------------- \n";
        cout << "1. Agregar productos \n";
        cout << "2. Listar productos \n";
        cout << "3. Buscar producto \n";
        cout << "4. Frecuencia de un producto \n";
        cout << "5. Borrar un producto \n";
        cout << "6. Editar un producto \n";
        cout << "0. Salir \n";
        cout << "--------------------------------------------------------- \n";
        cout << "Ingresa la opcion: "; cin >> opcion;
        switch (opcion) {
            case 1: escribe(nf); system("cls"); break;
            case 2: lee(nf); system("pause"); system("cls"); break;
            case 3: buscar(nf); system("pause"); system("cls"); break;
            case 4: frecuencia(nf); system("pause"); system("cls"); break;
            case 5: eliminar(nf); system("cls"); break;
            case 6: editar(nf); system("cls"); break;
            case 0: system("cls"); break;
            default: cout << "Opcion no valida\n"; break;
        }
    }
    return 0;
}