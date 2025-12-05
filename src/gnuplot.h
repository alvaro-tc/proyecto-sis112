#ifndef GNUPLOT_H
#define GNUPLOT_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <fstream>
#include <sstream>

// Detectar sistema operativo
#ifdef _WIN32
    #define GNUPLOT_POPEN _popen
    #define GNUPLOT_PCLOSE _pclose
#else
    #define GNUPLOT_POPEN popen
    #define GNUPLOT_PCLOSE pclose
#endif

using namespace std;

class Gnuplot {
private:
    FILE* pipe;

public:
    Gnuplot() {
        // Se intenta abrir gnuplot. Asegurese de que gnuplot este en el PATH.
        // -persistent mantiene la ventana abierta despues de que el programa termina.
        pipe = GNUPLOT_POPEN("gnuplot -persistent", "w");
        if (!pipe) {
            cerr << "Error: No se pudo encontrar gnuplot. Asegurese de que este instalado y en el PATH." << endl;
        }
    }

    ~Gnuplot() {
        if (pipe) {
            GNUPLOT_PCLOSE(pipe);
        }
    }

    // Enviar comandos a gnuplot
    void operator<<(const string& command) {
        if (pipe) {
            fprintf(pipe, "%s\n", command.c_str());
            fflush(pipe);
        }
    }

    // Graficar un histograma simple (Barras)
    // x: etiquetas (nombres), y: alturas (valores)
    void plot_bar(const vector<string>& x, const vector<double>& y, const string& title, const string& xlabel, const string& ylabel) {
        if (!pipe) return;

        if (x.size() != y.size()) {
            cerr << "Error: Los vectores X e Y deben tener el mismo tamanio." << endl;
            return;
        }

        // Configuracion basica
        *this << "set title '" + title + "'";
        *this << "set xlabel '" + xlabel + "'";
        *this << "set ylabel '" + ylabel + "'";
        *this << "set style data histograms";
        *this << "set style fill solid 1.0 border -1";
        *this << "set boxwidth 0.8 relative";
        *this << "set xtics rotate by -45"; // Rotar etiquetas si son largas
        *this << "set grid ytics";
        
        // Escribir datos en linea
        fprintf(pipe, "plot '-' using 2:xtic(1) notitle\n");
        
        for (size_t i = 0; i < x.size(); ++i) {
            fprintf(pipe, "\"%s\" %f\n", x[i].c_str(), y[i]);
        }
        fprintf(pipe, "e\n"); // Fin de datos
        fflush(pipe);
    }
};

#endif
