# Proyecto: Sistema de farmacia (ejemplo en C++)

Descripción
Este repositorio contiene un ejemplo didáctico de un sistema básico de venta y control de almacen para una farmacia. El objetivo es demostrar el uso de funciones, tipos de variables, arreglos/dinámicos y vectores, estructuras (struct) y manejo simple de archivos en C++. Incluyendo histogramas de productos con gnuplot.


Características principales
- Control básico de almacen: añadir, modificar y listar productos.  
- Registro de ventas: almacenar el nombre del comprador y la cantidad comprada.  
- Persistencia simple en archivos locales.  
- Interfaz de consola.  



Estructura del proyecto
- Informe/                Documentación (PDF)  
- src/                    Código fuente
  - main.cpp              Archivo principal donde corre todo el programa
  - modelos.h             Aquí están las estructuras (structs) de los datos
  - libreria_productos.h  Lógica para manejar los productos (agregar, listar, etc.)
  - libreria_clientes.h   Lógica para manejar los clientes
  - libreria_ventas.h     Lógica para las ventas y sus detalles
