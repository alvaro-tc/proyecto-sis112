#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <map>

#include "modelos.h"

using namespace std;

void pausa() {
    cout << "\nPresione Enter para continuar...";
    // cin.ignore(); // Removed to fix double enter issue
    cin.get();
}

#include "utilidades.h"
#include "libreria_productos.h"
#include "libreria_clientes.h"
#include "libreria_ventas.h"
#include "libreria_reportes.h"

int main() {
    int op;

    system("cls");
    cout << "======================================================\n";
    cout << "         SISTEMA DE INVENTARIADO DE FARMACIAS         \n";
    cout << "======================================================\n\n";
    cout << "Este es un sistema de inventariado hecho en C++ para la\n";
    cout << "gestion de productos y ventas de una farmacia desarrollado por:\n";
    cout << "Alvaro Torrez Calle\n\n";
    pausa();

    do {
        system("cls");
        cout << "\n===== SISTEMA DE VENTAS =====\n";
        cout << "1. Productos\n";
        cout << "2. Clientes\n";
        cout << "3. Ventas\n";
        cout << "4. Reportes\n";
        cout << "0. Salir\n";
        
        op = leerEntero("Opcion: ");

        switch(op) {
            case 1: menuProductos(); break;
            case 2: menuClientes(); break;
            case 3: menuVentas(); break;
            case 4: menuReportes(); break;
            case 0: cout << "Saliendo...\n"; break;
            default: cout << "Opcion invalida.\n"; pausa(); break;
        }
    } while(op != 0);
    return 0;
}