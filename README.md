# Proyecto: Sistema de Farmacia

Este repositorio contiene un sistema de gestión para una farmacia desarrollado en C++. Permite administrar productos, clientes y ventas, generando reportes detallados y gráficos estadísticos.

## Características Principales

- **Gestión de Stock**: Altas, bajas, modificaciones y listado de productos.
- **Gestión de Clientes**: Registro y administración de información de clientes.
- **Sistema de Ventas**: Registro de transacciones con cálculo automático de totales y control de stock.
- **Reportes Avanzados**:
  - Listados ordenados (Ascendente/Descendente) de productos, clientes y ventas.
  - Reportes financieros (Ganancia Total, Ventas por Rango de Fecha).
  - Estadísticas de productos más/menos vendidos.
- **Gráficos Estadísticos (Gnuplot)**:
  - Top 5 Clientes y Productos.
  - Histogramas de Stock y Ventas Diarias.

## Requisitos Previos

Para ejecutar este proyecto necesitas:

1.  **Compilador de C++**: Se recomienda `g++` (MinGW o similar).
2.  **Gnuplot**: Necesario para generar los gráficos.
    - Descargar e instalar desde [gnuplot.info](http://www.gnuplot.info/).
    - **Importante**: El programa busca el ejecutable en `C:\Program Files\gnuplot\bin\gnuplot.exe`. Si lo instalas en otra ruta, asegúrate de agregar esa ruta a tus variables de entorno PATH.

## Instrucciones de Instalación y Ejecución

1.  **Clonar el repositorio**:
    ```bash
    git clone https://github.com/tu-usuario/proyecto-sis112.git
    cd proyecto-sis112
    ```

2.  **Compilar el código**:
    Navega a la carpeta `src` y ejecuta el comand de compilación:
    ```bash
    cd src
    g++ main.cpp -o main.exe
    ```

3.  **Ejecutar el programa**:
    ```bash
    .\main.exe
    ```

## Estructura del Proyecto

- `src/`: Código fuente del sistema.
  - `main.cpp`: Punto de entrada de la aplicación.
  - `libreria_productos.h`: Lógica de gestión de inventario.
  - `libreria_clientes.h`: Lógica de administración de clientes.
  - `libreria_ventas.h`: Lógica de transacciones.
  - `libreria_reportes.h`: Generación de reportes y menús de estadística.
  - `gnuplot.h`: Interfaz para la comunicación con Gnuplot.
  - `modelos.h`: Definición de estructuras de datos.
  - `utilidades.h`: Funciones auxiliares generales.
- `Informe/`: Documentación adicional del proyecto.
