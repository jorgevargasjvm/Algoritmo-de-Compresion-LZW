#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <string.h>

#define EJEMPLO "WYS*WYGWYS*WYSWYSG"

#define SEPARADOR '|'

using namespace std; 

// Convertir el �ndice del diccionario en una secuencia de bytes
void secuencia_bytes(short int dato) {
    cout << (unsigned char) (dato >> 8);    
    cout << (unsigned char) (dato & 255);
}

// Leer el valor entero de dos bytes en el fichero de entrada comprimido
short int leer_valor(istream_iterator<unsigned char> &it) {
    short int v = *it;
    it++;
    v = v * 256 + *it;
    it++;

    return v;
}

// Codificar la secuencia de caracteres del flujo de entrada y sacar
// la secuencia comprimida por salida est�ndar
void codificar(ifstream &ifs) { 
    unordered_map<string, long> diccionario; 

    for (int i = 0; i < 256; i++) { 
        string s = ""; 
        s += char(i); 
        diccionario[s] = i; 
    } 
      
    istream_iterator<char> eos;
    istream_iterator<char> iti (ifs);
    string p = "", c = "";
    short int codigo = 256; 

    p += (char) *iti;    
    while (iti != eos) {
        iti++;
        if (iti != eos)
            c += *iti; 
        if (diccionario.find(p + c) != diccionario.end()) { 
            p = p + c; 
        } 
        else { 
            secuencia_bytes(diccionario[p]);
            //cout << diccionario[p];
            diccionario[p + c] = codigo; 
            codigo++; 
            p = c; 
        } 
        c = "";
    } 
    secuencia_bytes(diccionario[p]);
} 
  
// Decodificar la secuencia de caracteres del flujo de entrada y guardar
// la secuencia descomprimida en el fichero indicado en el flujo de entrada
void decodificar(ifstream &ifs) {
    unordered_map<short int, string> diccionario; 

    for (int i = 0; i <= 255; i++) { 
        string ch = ""; 
        ch += char(i); 
        diccionario[i] = ch; 
    } 

    istream_iterator<unsigned char> eos;
    istream_iterator<unsigned char> iti (ifs);

    // El nombre del fichero de salida est� al principio del 
    // flujo de entrada, terminado en el caracter SEPARADOR
    string fichero = "";
    while ((unsigned char) *iti != SEPARADOR) {
        fichero += (unsigned char) *iti;
        iti++;        
    }
    iti++;

    // Leer la longitud del fichero antes de comprimir
    long int tamanyo = 0;
    while ((unsigned char) *iti != SEPARADOR) {
        tamanyo = tamanyo * 10 + (*iti - '0');
        iti++;        
    }
    iti++;

    ofstream fichero_salida(fichero, ios::out | ios::binary);
    if (!fichero_salida) {
        cout << "No se puede crear el fichero " << fichero << endl;
        exit(0);
    }

    short int old = leer_valor(iti), n; 
    string s = diccionario[old], c = ""; 
    c += s[0]; 
    fichero_salida << s; 
    
    short int contador = 256;
    while (iti != eos) {
        n = leer_valor(iti); // Al leer el valor, el iterador apunta al que le sigue
        if (diccionario.find(n) == diccionario.end()) { 
            s = diccionario[old]; 
            s = s + c; 
        } 
        else { 
            s = diccionario[n]; 
        } 
        fichero_salida << s; 
        c = ""; 
        c += s[0]; 
        diccionario[contador] = diccionario[old] + c; 
        contador++; 
        old = n; 
        tamanyo -= 2; // Descontar los 2 bytes que se han descomprimido
    } 

    // Comprobar si se ha llegado al final del fichero comprimido
    if (tamanyo <= 0)
        return;
} 

int main(int argc, char *argv[]) { 
    if (argc == 1) {
        cout << "Uso: " << argv[0] << "<nombre_fichero> | -u <nombre_fichero>" << endl;
        exit(0);
    }

    if (strcmp(argv[1], "-u") == 0) {
        ifstream fichero_salida(argv[2], ios::in | ios::binary);
        if (!fichero_salida) {
            cerr << "No existe el fichero " << argv[2] << endl;
            exit(0);
        }
        decodificar(fichero_salida);
    } else if(strcmp(argv[1], "-h")== 0){
        cout << "Formas de uso" <<endl;
        cout << "./lzw -u [file.lzw]" << "\t Utilice el comando -u para descomprimir el archivo deseado"<<endl;
        cout << "./lzw [file] > [result].lzw" << "\t Utilice este comando para comprimir el archivo deseado" <<endl; 
    }else
    {
        ifstream fichero_entrada(argv[1], ios::in | ios::binary);
        if (!fichero_entrada) {
            cout << "No existe el fichero " << argv[1] << endl;
            exit(0);
        }
        cout << argv[1] << SEPARADOR;

        codificar(fichero_entrada);    
    }
} 
