#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <string.h>

#define EJEMPLO "WYS*WYGWYS*WYSWYSG"

#define SEPARADOR '|'

using namespace std;

// Convertir el índice del diccionario en una secuencia de bytes
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
// la secuencia comprimida por salida estándar
void codificar(ifstream &ifs) {
    unordered_map<string, short int> diccionario;

    for (int i = 0; i < 256; i++) {
        string s = "";
        s += char(i);
        diccionario[s] = i;
    }

    istream_iterator<char> eos;
    istream_iterator<char> iti (ifs);
    string p = "", c = "";
    unsigned short int codigo = 256;

    p += (char) *iti;
    while (iti != eos) {
        iti++;
        if (iti != eos)
            c += *iti;
        if (diccionario.find(p + c) != diccionario.end()) {
            p = p + c;
        }
        else {
            if (codigo == 65535) {
                cerr << "Tabla de codificación completa, no se puede continuar" << endl;
                exit(-1);
            }
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
    unordered_map<unsigned short int, string> diccionario;

    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        diccionario[i] = ch;
    }

    istream_iterator<unsigned char> eos;
    istream_iterator<unsigned char> iti (ifs);

    while (iti != eos) {
        // El nombre del fichero de salida está al principio del
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
            cerr << "No se puede crear el fichero " << fichero << endl;
            exit(0);
        }

        cerr << "* Generando el fichero " << fichero << endl;
        unsigned short int old = leer_valor(iti), n;
        tamanyo -= 2;
        string s = diccionario[old], c = "";
        c += s[0];
        fichero_salida << s;

        unsigned short int contador = 256;
        while (tamanyo >= 0) { //(iti != eos) {
            n = leer_valor(iti); // Al leer el valor, el iterador apunta al que le sigue
            if (diccionario.find(n) == diccionario.end()) {
                s = diccionario[old];
                s = s + c;
            }
            else {
                s = diccionario[n];
            }
            tamanyo -= s.size();    // Descontar los 2 bytes que se han descomprimido
            fichero_salida << s;

            c = "";
            c += s[0];
            diccionario[contador] = diccionario[old] + c;
            contador++;
            old = n;
        }

        cerr << "  Fichero " << fichero << " creado" << endl;
        fichero_salida.close();
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        cerr << "Uso: " << argv[0] << "<nombre_fichero> ... | -u <nombre_fichero_lzw>" << endl;
        exit(0);
    }

    if (strcmp(argv[1], "-u") == 0) {
        ifstream fichero_salida(argv[2], ios::in | ios::binary);
        if (!fichero_salida) {
            cerr << "No existe el fichero " << argv[2] << endl;
            exit(0);
        }
        cerr << "Descomprimiendo el fichero " << argv[2] << endl;
        decodificar(fichero_salida);
        cerr << "Fichero " << argv[2] << " descomprimido" << endl;
        fichero_salida.close();

    } else if(strcmp(argv[1],"-h") == 0){
		cout << "Help:" <<endl;
		cout << " " << endl;
        cout << "./lzw -u [file.lzw]" << "\t Utilice el comando -u para descomprimir el archivo deseado"<<endl;
	    cout << "./lzw [file] > [result].lzw" << "\t Utilice este comando para comprimir el archivo deseado" <<endl;

	}else {

        for (int i = 1; i < argc; i++) {
            ifstream fichero_entrada(argv[i], ios::in | ios::binary);
            if (!fichero_entrada) {
                cerr << "No existe el fichero " << argv[1] << endl;
                exit(0);
            }
            cerr << "--> Comprimiendo el fichero " << argv[i] << endl;
            cout << argv[i] << SEPARADOR;

            fichero_entrada.seekg(0, ios::end);
            unsigned long tamanyo = fichero_entrada.tellg();
            fichero_entrada.seekg(0, ios::beg);
            cout << tamanyo << SEPARADOR;

            codificar(fichero_entrada);
            cerr << "    Fichero " << argv[i] << " comprimido" << endl;

            fichero_entrada.close();
        }
    }
}
