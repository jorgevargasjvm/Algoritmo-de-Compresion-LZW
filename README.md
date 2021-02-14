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
  
  ###### LZW V5 es capaz de comprimir archivos y directorios
  ###### LZW2 es capaz de comprimir múltiples archivos


