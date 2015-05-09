#include <iostream>
#include <string>
#include<fstream>
#include<sstream>
#include <math.h>
using namespace std;

//para leer archivo de texto

ifstream File;
string FileRead;

//variables que manejan marcos

int marcosreal[256]; //(numero de proceso que utiliza el marco)
int marcosrealmodificado[256]; //(0 o 1, modificado o no)
int marcosrealtimestamps[256]; //(para calcular tiempo en el que se modifico)
int marcosvirtual[512]; //(apunta al proceso del marco real que tenia la memoria real)


void cargarProceso()
{

}

int main()
{
    //Cargar archivo de texto
    File.open("text.txt");
	//string FileRead;

    while (!File.eof()) {

        File>>FileRead; //agarra cada caracter separado por un espacio.

        if(FileRead == "P" || FileRead == "p"){cargarProceso();}
        //else if(FileRead == "A" || FileRead == "a"){A();}
        //else if(FileRead == "L" || FileRead == "l"){L();}
        //else if(FileRead == "E" || FileRead == "e"){E();}
        //else if(FileRead == "F" || FileRead == "f"){F();}
        else{}//cout<<FileRead<<endl;}


}//while
    File.close();
    cout << "Hello world!" << endl;
    return 0;
}
