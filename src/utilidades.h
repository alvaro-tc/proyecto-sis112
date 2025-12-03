#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Funcion para leer un entero con validacion
int leerEntero(string mensaje) {
    int valor;
    string entrada;
    while (true) {
        cout << mensaje;
        getline(cin, entrada);
        try {
            valor = stoi(entrada);
            return valor;
        } catch (const invalid_argument& e) {
            cout << "Error: Debe ingresar un numero entero valido.\n";
        } catch (const out_of_range& e) {
            cout << "Error: El numero es demasiado grande.\n";
        }
    }
}

// Funcion para leer un double con validacion
double leerDouble(string mensaje) {
    double valor;
    string entrada;
    while (true) {
        cout << mensaje;
        getline(cin, entrada);
        try {
            valor = stod(entrada);
            return valor;
        } catch (const invalid_argument& e) {
            cout << "Error: Debe ingresar un numero valido.\n";
        } catch (const out_of_range& e) {
            cout << "Error: El numero es demasiado grande.\n";
        }
    }
}

// Funcion para validar si una cadena es numerica
bool esNumero(const string& str) {
    for (char const &c : str) {
        if (isdigit(c) == 0) return false;
    }
    return true;
}

// Funcion para leer una fecha con validacion (DD/MM/AAAA)
void leerFecha(string mensaje, char* fechaSalida) {
    string entrada;
    while (true) {
        cout << mensaje;
        getline(cin, entrada);

        if (entrada.length() != 10) {
            cout << "Error: Formato incorrecto. Use DD/MM/AAAA.\n";
            continue;
        }

        if (entrada[2] != '/' || entrada[5] != '/') {
            cout << "Error: Formato incorrecto. Use DD/MM/AAAA.\n";
            continue;
        }

        string diaStr = entrada.substr(0, 2);
        string mesStr = entrada.substr(3, 2);
        string anioStr = entrada.substr(6, 4);

        if (!esNumero(diaStr) || !esNumero(mesStr) || !esNumero(anioStr)) {
            cout << "Error: La fecha debe contener numeros.\n";
            continue;
        }

        int dia = stoi(diaStr);
        int mes = stoi(mesStr);
        int anio = stoi(anioStr);

        if (anio < 1900 || anio > 2100) {
            cout << "Error: Anio fuera de rango valido.\n";
            continue;
        }

        if (mes < 1 || mes > 12) {
            cout << "Error: Mes invalido (1-12).\n";
            continue;
        }

        int diasEnMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        // Bisiesto
        if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) {
            diasEnMes[2] = 29;
        }

        if (dia < 1 || dia > diasEnMes[mes]) {
            cout << "Error: Dia invalido para el mes especificado.\n";
            continue;
        }

        // Si pasa todas las validaciones
        strcpy(fechaSalida, entrada.c_str());
        break;
    }
}

#endif


