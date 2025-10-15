--- 
geometry: margin=2.5cm
fontsize: 12pt
mainfont: "Calibri"
sansfont: "Calibri"
monofont: "Consolas"
header-includes: |
  \usepackage{fontspec}
  \usepackage{pdfpages}
  \usepackage{booktabs}
  \usepackage{longtable}
  \usepackage{caption}
  \captionsetup[table]{labelfont=bf,textfont=sf}
  \usepackage{parskip}
  \usepackage{tocloft}
  \renewcommand{\cftsecfont}{\sffamily\bfseries}
  \renewcommand{\cftsecpagefont}{\sffamily\bfseries}
  \renewcommand{\cftsubsecfont}{\sffamily\bfseries}
  \renewcommand{\cftsubsecpagefont}{\sffamily\bfseries}
  \renewcommand{\cftsubsubsecfont}{\sffamily\bfseries}
  \renewcommand{\cftsubsubsecpagefont}{\sffamily\bfseries}
  \setlength{\cftbeforesecskip}{6pt}
  \setlength{\cftbeforesubsecskip}{4pt}
  \setlength{\cftbeforesubsubsecskip}{2pt}
  \setlength{\cftsecindent}{0pt}
  \setlength{\cftsubsecindent}{1.5em}
  \setlength{\cftsubsubsecindent}{3.0em}
  \setlength{\cftsecnumwidth}{2.5em}
  \setlength{\cftsubsecnumwidth}{3.5em}
  \setlength{\cftsubsubsecnumwidth}{4.5em}
  \usepackage{titlesec}
  \titleformat{\section}{\sffamily\Large\bfseries}{\thesection}{1em}{}
  \titleformat{\subsection}{\sffamily\large\bfseries}{\thesubsection}{1em}{}
  \titleformat{\subsubsection}{\sffamily\normalsize\bfseries}{\thesubsubsection}{1em}{}
  \titlespacing*{\section}{0pt}{1.2\baselineskip}{0.6\baselineskip}
  \titlespacing*{\subsection}{0pt}{1.0\baselineskip}{0.4\baselineskip}
  \titlespacing*{\subsubsection}{0pt}{0.8\baselineskip}{0.3\baselineskip}
  \renewcommand{\familydefault}{\sfdefault}
  \let\maketitle\relax
--- 

\includepdf[pages=1]{caratula/Caratula.pdf}

\newpage
\pagenumbering{roman}

\begin{center}
  {\sffamily\Large\bfseries Índice}
\end{center}

\begingroup
  \renewcommand{\contentsname}{} 
  \tableofcontents
\endgroup

\newpage
\pagenumbering{arabic}

# Introducción

Este proyecto propone un sistema de farmacia sencillo para comercios pequeños que automatiza el control de inventario y el registro de ventas. El sistema almacenará el nombre y precio de cada producto, registrará las personas que compran y la cantidad de artículos adquiridos, permitiendo consultas rápidas del stock y de las ventas realizadas.

# Problemas identificados

1. Control manual del inventario que genera desajustes.  
2. Falta de registro estructurado de ventas y clientes.  
3. Dificultad para conocer rápidamente productos agotados o movimientos de venta.

# Objetivo general

Construir una aplicación básica en C++ que permita controlar el inventario y registrar ventas con la información mínima necesaria: nombre y precio del producto, nombre del comprador y cantidad comprada, para facilitar la gestión diaria del comercio.

# Funcionalidades principales

- Registrar productos con nombre y precio.  
- Actualizar stock al realizar ventas.  
- Registrar ventas indicando nombre del comprador y cantidad comprada.  
- Consultar inventario y listar ventas realizadas.  

# Alcances y limitaciones

## Alcances
- El sistema gestionará el inventario y las ventas de una sola farmacia
- Se podra agregar, modificar y eliminar productos
- Se podra agregar, modificar y eliminar una venta
- Se podra agregar, modificar y eliminar un usuario
- Se podra consultar historial de ventas. 


## Limitaciones
- No contempla facturación fiscal
- No sera multiusuario
- No sera multiplataforma
- No tendra pasarela de pago
- No tendra interfaz grafica
- No tendra seguridad de autenticacion


# Herramientas y tecnologías

Lenguaje: C++  
Entorno de desarrollo: Visual Studio Code  
Almacenamiento: archivos locales   
Control de versiones: Git
