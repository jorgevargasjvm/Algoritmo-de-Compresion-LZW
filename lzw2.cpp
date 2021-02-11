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
    ifstream fileInput;
    fileInput.open(filename);
    stringstream strStream;
    strStream << fileInput.rdbuf();

    cout << filename << endl;
    string str = strStream.str();
    
    unordered_map<string, int> dictionary; 
	for (int i = 0; i <= 255; i++) { 
		string ch = ""; 
		ch += char(i); 
		dictionary[ch] = i; 
	} 

	string p = "", c = ""; 
	p += str[0]; 
	int code = 256; 
	vector<int> output_code; 
	for (int i = 0; i < str.length(); i++) { 
		if (i != str.length() - 1) 
			c += str[i + 1]; 
		if (dictionary.find(p + c) != dictionary.end()) { 
			p = p + c; 
		} 
		else { 
		
			output_code.push_back(dictionary[p]); 
			dictionary[p + c] = code; 
			code++; 
			p = c; 
		} 
		c = ""; 
	} 
	output_code.push_back(dictionary[p]); 
    return output_code; 
} 

void decodificar(string filename) 
{ 
    string token;
    ifstream myfile (filename);
    if (myfile.is_open())
    {
        string line;
        getline (myfile,line);
        ofstream outfile (line);
        if (outfile.is_open())
        {
            getline (myfile,line);


            string OC(line);
            string buf;                
            stringstream ss(OC);      

            vector<int> op; 

            while (ss >> buf){
                op.push_back(stoi(buf));
            }

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
            outfile << s; 
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
                outfile << s; 
                c = ""; 
                c += s[0]; 
                table[count] = table[old] + c; 
                count++; 
                old = n; 
            } 
          
            outfile.close();
        }
        myfile.close();
    }


} 

int main(int argc, char *argv[]) { 
    if (argc == 1) {
        cout << "Uso: " << argv[0] << "<nombre_fichero> | -u <nombre_fichero>" << endl;
        exit(0);
    }

    if (strcmp(argv[1], "-u") == 0) {
        decodificar(argv[2]);
    } else if(strcmp(argv[1], "-h")== 0){
        cout << "Formas de uso" <<endl;
        cout << "./lzw -u [file.lzw]" << "\t Utilice el comando -u para descomprimir el archivo deseado"<<endl;
        cout << "./lzw [file] > [result].lzw" << "\t Utilice este comando para comprimir el archivo deseado" <<endl; 
    }else
    {
        vector<int> output_code = codificar(argv[1]); ; 
        for (int i = 0; i < output_code.size(); i++) { 
            cout << output_code[i] << " "; 
        }
        cout << endl;
        
    }
} 
