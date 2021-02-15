# Algoritmo-de-Compresion-LZW
Implementación del algoritmo de compresión LZW para comprimir archivos y directorios.

Este proyecto cuenta con dos formas de compresión:

*lzw v5.cpp y lzw2.cpp*

**Para compilar, ejecutar el comando:**
  ```
  g++ "lzw v5.cpp" -o lzw
  ```
 o 
   ```
  g++ lzw2.cpp -o lzw
  ```
**Para comprimir:**
 ```
 ./lzw [files] [files] > [name].lzw 
 ```
**Para descomprimir:**
 ```
 ./lzw -u [name].lzw 
 ```
**Para ver como usar el lzw:**
  ```
  ./lzw -h
  ```
  **Para usar el script**
  ```
  Tiene varias opciones:
•	Sin argumentos: comprueba que el fichero ejecutable está en el directorio actual. Si está, añade este directorio al PATH.
•	Con un argumento, que será el directorio que se quiera añadir al path: comprueba que el fichero ejecutable está en el directorio indicado y, si está, lo añade al path.
Para ejecutar el script:
1.	Convertirlo en ejecutable con: chmod u+x script.sh
2.	Ejecutarlo con el comando punto (".") o "source" :   . script.sh (OJO: el comando punto no se debe confundir con el punto de la carpeta actual: "/."; son dos cosas muy distintas.
  ```
  
  ###### LZW V5 es capaz de comprimir archivos y directorios
  ###### LZW2 es capaz de comprimir múltiples archivos


