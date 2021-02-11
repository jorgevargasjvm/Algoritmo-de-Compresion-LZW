#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <string.h>
#include <bits/stdc++.h>
#include <sstream>


using namespace std; 

#include <bits/stdc++.h> 
using namespace std; 

vector<int> codificar(string filename) { 
    ifstream fichero;
    fichero.open(filename);
    stringstream strStream;
    strStream << fichero.rdbuf();

    string str = strStream.str();
    
    unordered_map<string, int> table; 
	for (int i = 0; i <= 255; i++) { 
		string ch = ""; 
		ch += char(i); 
		table[ch] = i; 
	} 

	string p = "", c = ""; 
	p += str[0]; 
	int code = 256; 
	vector<int> output_code; 
	for (int i = 0; i < str.length(); i++) { 
		if (i != str.length() - 1) 
			c += str[i + 1]; 
		if (table.find(p + c) != table.end()) { 
			p = p + c; 
		} 
		else { 
		
			output_code.push_back(table[p]); 
			table[p + c] = code; 
			code++; 
			p = c; 
		} 
		c = ""; 
	} 
	output_code.push_back(table[p]); 
    return output_code; 
} 
  

void decodificar(vector<int> op) 
{ 
	unordered_map<int, string> table; 
	for (int i = 0; i <= 255; i++) { 
		string ch = ""; 
		ch += char(i); 
		table[i] = ch; 
	} 
	int old = op[0], n; 
	string s = table[old]; 
	string c = ""; 
	c += s[0]; 
	cout << s; 
	int count = 256; 
	for (int i = 0; i < op.size() - 1; i++) { 
		n = op[i + 1]; 
		if (table.find(n) == table.end()) { 
			s = table[old]; 
			s = s + c; 
		} 
		else { 
			s = table[n]; 
		} 
		cout << s; 
		c = ""; 
		c += s[0]; 
		table[count] = table[old] + c; 
		count++; 
		old = n; 
	} 
} 

int main(int argc, char *argv[]) { 
    if (argc == 1) {
        cout << "Uso: " << argv[0] << "<nombre_fichero> | -u <nombre_fichero>" << endl;
        exit(0);
    }

    if (strcmp(argv[1], "-u") == 0) {
      /*  ifstream fichero_salida(argv[2], ios::in | ios::binary);
        if (!fichero_salida) {
            cerr << "No existe el fichero " << argv[2] << endl;
            exit(0);
        }
        decodificar(fichero_salida);*/
    } else if(strcmp(argv[1], "-h")== 0){
        cout << "Formas de uso" <<endl;
        cout << "./lzw -u [file.lzw]" << "\t Utilice el comando -u para descomprimir el archivo deseado"<<endl;
        cout << "./lzw [file] > [result].lzw" << "\t Utilice este comando para comprimir el archivo deseado" <<endl; 
    }else
    {
        vector<int> output_code = codificar(argv[1]); ; 
        cout << "Output Codes are: "; 
        for (int i = 0; i < output_code.size(); i++) { 
            cout << output_code[i] << " "; 
        }
        cout << endl;

        decodificar(output_code);
    }
} 
