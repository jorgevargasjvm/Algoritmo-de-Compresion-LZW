#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define EJEMPLO "WYS*WYGWYS*WYSWYSG"

#define SEPARADOR '|'
#define ESDIR "__DIR"
#define ESFINDIR "__FINDIR"

using namespace std;

// Convertir el índice del diccionario en una secuencia de bytes
void secuencia_bytes(unsigned short int dato) {
    cout << (unsigned char) (dato >> 8);
    cout << (unsigned char) (dato & 255);
}

// Leer el valor entero de dos bytes en el fichero de entrada comprimido
short unsigned int leer_valor(ifstream &ifs) {
    unsigned short int v = ifs.get();
    v = v * 256 + ifs.get();

    return v;
}

// Codificar la secuencia de caracteres del flujo de entrada y sacar
// la secuencia comprimida por salida estándar
void codificar(ifstream &ifs) {
    unordered_map<string, unsigned short int> diccionario;

    for (int i = 0; i < 256; i++) {
        string s = "";
        s += char(i);
        diccionario[s] = i;
    }

    string p = "", c = "";
    unsigned short int codigo = 256;

    p += ifs.get();
    bool ultimo = false;
    while (!ifs.eof() && !ultimo) {
        if (ifs.eof())
            ultimo = true;
        else
            c += ifs.get();

        if (diccionario.find(p + c) != diccionario.end()) {
            p = p + c;
        } else {
            if (codigo == 65535) {
                cerr << "Tabla de codificación completa, no se puede continuar" << endl;
                exit(-1);
            }
            secuencia_bytes(diccionario[p]);
            diccionario[p + c] = codigo;
            codigo++;
            p = c;
        } 
        c = "";
    }
    secuencia_bytes(diccionario[p]);
    cout << SEPARADOR;
}

// Decodificar la secuencia de caracteres del flujo de entrada y guardar
// la secuencia descomprimida en el fichero indicado en el flujo de entrada
void decodificar(ifstream &ifs) {
    unordered_map<unsigned short int, string> diccionario;

    for (int i = 0; i < 256; i++) {
        string ch = "";
        ch += char(i);
        diccionario[i] = ch;
    }

    char ch = 0;
    string fichero = "";
    while (!ifs.eof()) { 
        // El nombre del fichero de salida está al principio del
        // flujo de entrada, terminado en el caracter SEPARADOR
        while ((ch = ifs.get()) != SEPARADOR && ch != EOF) {
            fichero += ch;
        }
        if (ch == EOF) break;

        if (fichero == ESDIR) {
            fichero = "";
            while ((ch = ifs.get()) != SEPARADOR)
                fichero += ch;

            if (mkdir(fichero.c_str(), 0777) != 0) {
                cerr << "No se puede crear el directorio " << fichero << endl;
                exit(0);
            }
            if (chdir(fichero.c_str()) != 0) {
                cerr << "No se puede acceder al directorio " << fichero << endl;
                exit(0);
            }
            cerr << "____Directorio " << fichero << " creado____" << endl;
            continue;
        } else if (fichero == ESFINDIR) {
            if (chdir("..") != 0) {
                cerr << "No se puede volver al directorio " << endl;
                exit(0);
            }
            cerr << "____Fin de directorio____" << endl;
            continue;
        }

        // Leer la longitud del fichero antes de comprimir
        long int tamanyo = 0;
        while ((ch = ifs.get()) != SEPARADOR)
            tamanyo = tamanyo * 10 + (ch - '0');

        ofstream fichero_salida(fichero, ios::out | ios::binary);
        if (!fichero_salida) {
            cerr << "No se puede crear el fichero " << fichero << endl;
            exit(0);
        }

        cerr << "* Generando el fichero " << fichero << endl;
        unsigned short int old = leer_valor(ifs), n;
        tamanyo -= 2;
        string s = diccionario[old], c = "";
        c += s[0];
        fichero_salida << s;

        unsigned short int contador = 256;
        while (tamanyo >= 0) { 
            n = leer_valor(ifs); 
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
        while ((ch = ifs.get()) != SEPARADOR);
    }
}

void comprimir_fichero(char *nombre) {
    ifstream fichero_entrada(nombre, ios::in | ios::binary);
    if (!fichero_entrada) {
        cerr << "No se puede abrir el fichero " << nombre << endl;
        exit(0);
    }
    cerr << "--> Comprimiendo el fichero " << nombre << endl;
    cout << nombre << SEPARADOR;

    fichero_entrada.seekg(0, ios::end);
    unsigned long tamanyo = fichero_entrada.tellg();
    fichero_entrada.seekg(0, ios::beg);
    cout << tamanyo << SEPARADOR;

    codificar(fichero_entrada);
    cerr << "    Fichero " << nombre << " comprimido" << endl;

    fichero_entrada.close();
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

    } else if (strcmp(argv[1], "-h") == 0) {
        cout << "Help:" << endl << endl;
        cout << "./lzw -u [file.lzw]" << "\t Utilice el comando -u para descomprimir el archivo deseado" << endl;
        cout << "./lzw [file | dir] > [result].lzw" << "\t Utilice este comando para comprimir los archivos y directorios deseados" << endl;

    } else {
        for (int i = 1; i < argc; i++) {
            struct stat st;
            if (stat(argv[i], &st) == -1) {
                cerr << "No existe el fichero " << argv[i] << endl;
                exit(0);                
            }

            if (S_ISDIR(st.st_mode)) {
                cerr << "--> Comprimiendo el directorio " << argv[i] << endl;
                DIR *dir;
                struct dirent *ent;

                if ((dir = opendir(argv[i])) == NULL) {
                    cerr << "No se puede abrir el directorio " << argv[i] << endl;
                    exit(0);
                }
                if (chdir(argv[i]) != 0) {
                    cerr << "No se puede acceder al directorio " << argv[i] << endl;
                    exit(0);
                }

                cout << ESDIR << SEPARADOR << argv[i] << SEPARADOR;                
                while ((ent = readdir(dir)) != NULL) {
                    if (ent->d_type == DT_REG)
                        comprimir_fichero(ent->d_name);
                }
                cout << ESFINDIR << SEPARADOR;
                closedir(dir);
                if (chdir("..") != 0) {
                    cerr << "No se puede volver al directorio " << endl;
                    exit(0);
                }                
                cerr << "    Directorio " << argv[i] << " comprimido" << endl;
            } else {
                comprimir_fichero(argv[i]);
            }
        }
    }
}
