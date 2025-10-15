# Informe: compilación y requisitos

Requisitos mínimos
- Pandoc (>= 2.0)  
- Una distribución TeX con xelatex (MiKTeX o TeX Live)  
- Fuente sans instalada (por ejemplo Calibri o Arial)  
- El archivo de carátula: `caratula/Caratula.pdf` dentro de esta carpeta

Instalación (Windows, opciones rápidas)
- Instalar Pandoc: https://pandoc.org/installing.html  
- Instalar MiKTeX: https://miktex.org/download  (asegura xelatex)  
 alternativa: instalar TeX Live
- Opcional: instalar Calibri o usar otra fuente disponible en el YAML (mainfont/sansfont)

Compilar el informe (ejecutar desde la carpeta Informe)
````bash
# abre PowerShell o el terminal de VS Code en ...\Proyecto\Informe
pandoc Perfil_proyecto_pdf.md Perfil_proyecto.md -o Perfil_proyecto.pdf --number-sections --pdf-engine=xelatex