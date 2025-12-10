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
        // Se usa ruta absoluta para evitar conflictos con ejecutables corruptos locales
        pipe = GNUPLOT_POPEN("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persistent", "w");
        if (!pipe) {
            // Fallback: intentar comando global si falla la ruta absoluta
             pipe = GNUPLOT_POPEN("gnuplot -persistent", "w");
             if (!pipe) {
                 cerr << "Error: No se pudo encontrar gnuplot. Asegurese de que este instalado y en el PATH." << endl;
             }
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

    // Graficar un histograma simple (Barras) con valores encima
    // x: etiquetas (nombres), y: alturas (valores)
    void plot_bar(const vector<string>& x, const vector<double>& y, const string& title, const string& xlabel, const string& ylabel) {
        if (!pipe) return;

        if (x.size() != y.size()) {
            cerr << "Error: Los vectores X e Y deben tener el mismo tamanio." << endl;
            return;
        }

        // 1. Escribir datos a un archivo temporal para poder reusarlos (barras + labels)
        ofstream dataFile("data.tmp");
        for (size_t i = 0; i < x.size(); ++i) {
            dataFile << "\"" << x[i] << "\" " << y[i] << endl;
        }
        dataFile.close();

        // 2. Enviar comandos a Gnuplot
        // Configuracion de Fuentes (Pequenas)
        *this << "set title font \",10 bold\"";
        *this << "set xlabel font \",8\"";
        *this << "set ylabel font \",8\"";
        *this << "set xtics font \",7\""; // Fuente mas pequena para ejes
        *this << "set ytics font \",7\"";
        *this << "set key font \",7\"";   // Fuente para la leyenda

        *this << "set title '" + title + "'";
        *this << "set xlabel '" + xlabel + "'";
        *this << "set ylabel '" + ylabel + "'";
        
        // Estilos
        *this << "set style data histograms";
        *this << "set style fill solid 0.6 border -1"; // Un poco mas solido
        *this << "set boxwidth 0.6 relative"; // Barras un poco mas delgadas
        
        // Margenes y rangos
        *this << "set bmargin 8"; // Margen inferior aumentado
        *this << "set lmargin 8"; // Margen izquierdo
        *this << "set xtics rotate by -45 scale 0"; 
        *this << "set grid ytics lc rgb \"#bbbbbb\" lw 1 lt 0"; // Grid mas sutil
        *this << "set yrange [0:*]"; 
        *this << "set offset 0,0,1,0"; 

        // 3. Plotear usando el archivo temporal
        // Color azul suave (#4da6ff)
        fprintf(pipe, "plot 'data.tmp' using 2:xtic(1) title 'Monto/Cant' lc rgb '#4da6ff', '' using 0:2:2 with labels font \",7\" offset 0,0.5 notitle\n");
        fflush(pipe);
    }
};

#endif
