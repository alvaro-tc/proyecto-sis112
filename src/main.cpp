#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iomanip>

#include "modelos.h"

using namespace std;

void pausa() {
    cout << "\nPresione Enter para continuar...";
    cin.ignore();
    cin.get();
}

#include "libreria_productos.h"
#include "libreria_clientes.h"
#include "libreria_ventas.h"

int main() {
    int op;
    do {
        system("cls");
        cout << "\n===== SISTEMA DE VENTAS =====\n";
        cout << "1. Productos\n";
        cout << "2. Clientes\n";
        cout << "3. Ventas\n";
        cout << "0. Salir\n";
        cout << "Opcion: ";
        cin >> op;
        switch(op) {
            case 1: menuProductos(); break;
            case 2: menuClientes(); break;
            case 3: menuVentas(); break;
            case 0: cout << "Saliendo...\n"; break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while(op != 0);
    return 0;
}