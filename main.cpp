#include <iostream>
#include <string>
#include<fstream>
#include<sstream>
#include <ctime>
#include <math.h>
#include <cmath>
#include <time.h>
using namespace std;


//para leer archivo de texto

ifstream File;
string FileRead;

//Timer para LRU

int marcosrealtimestamps[256] = {0}; //(para calcular tiempo en el que se modifico)
int marcosvirtualtimestamps[512] = {0}; //(para relacionar marcos virtuales con reales)
int cputime = 0;

//variables que manejan marcos

int marcosreal[256]; //(numero de proceso que utiliza el marco)
int marcosrealmodificado[256] = {0}; //(0 o 1, modificado o no)
int marcosrealreferenciado[256] = {0}; //(0 o 1, referenciado o no)
int marcosvirtual[512];//(apunta al proceso del marco real que tenia la memoria virtual)

int cantMarcos = 0;
int cuentaVirtual = 0;
int mintimestamp = 0; //cuando ya no haya más paginas que hayan sido creadas en tiempo 0 se elevara +1
int pagefaults = 0;


void cargarProceso()
{
//Formato (P n p)
double n = 0;
int p = 0;
int residuo = 0; //utilizado para saber cuantos marcos de paginación hay que reemplazar cuando se llene la memoria
int lleno = 0; // cuando se pase de 256 paginas/marcos, este estara arriba de 1


//leer n
File>>FileRead;
istringstream (FileRead) >>n;

//leer p
File>>FileRead;
istringstream (FileRead) >>p;

//Debug de direcciones
//cout << "Proceso: " << p << " Direcciones: " << n << endl;

//Empezar a meter datos

int introducidas = ceil (n/8);
int paginas = cantMarcos + introducidas;
lleno = paginas/256;
residuo = introducidas % 256;

int ultimoMarco = 0;

//Si hay espacio en memoria real
if(lleno == 0)
{
    ultimoMarco = (residuo + cantMarcos) ;
    //cout << residuo << " " <<cantMarcos << " " <<ultimoMarco << endl;
    for(int i = cantMarcos; i < ultimoMarco; i++)
    {
        if (marcosreal[i] == -1)
        {
        //asignar cputime para el timestamp
        marcosrealtimestamps[i] = cputime;
        marcosvirtualtimestamps[i] = cputime;
        marcosreal[i] = p;
        marcosvirtual[cuentaVirtual] = p;

        //subir el cputime, pagefaults y cuenta Virtual
        cuentaVirtual++;
        cputime++;
        pagefaults++;
        }
    }
    cantMarcos += residuo;
}
else
{
    cout << "El proceso " << p <<" llenara la memoria" << endl;
    int faltantes = introducidas;
    //checar si quedan espacios libres
    if (cantMarcos < 255)
    {
        ultimoMarco = 256;
        int restanteMarcos = 256-cantMarcos;

        for(int i = cantMarcos; i < ultimoMarco; i++)
        {
            if (marcosreal[i] == -1)
        {
          marcosrealtimestamps[i] = cputime;
          marcosvirtualtimestamps[i] = cputime;
          marcosreal[i] = p;
          marcosvirtual[cuentaVirtual] = p;
           //subir el cputime, pagefaults y cuenta Virtual
        cuentaVirtual++;
        cputime++;
        pagefaults++;
        }
        }
        cantMarcos = 255;
        cout << "Quedaron fuera " << restanteMarcos << " paginas. Se aplicara LRU" << endl;
    }
    //Empezar LRU para insertar


}




//Ver si se superaron los 256 marcos con texto
cout << "Paginas: " << cantMarcos << " Lleno: " << lleno << " Paginas introducidas: " << residuo << " hecho por el proceso: " << marcosreal[ultimoMarco-1]<< " tiempo entrada: " << marcosrealtimestamps[ultimoMarco-1] << endl;

}

void liberarPaginas()
{
    int idProceso; // id del proceso a liberar
    int marcosLiberadosReal[256]; // arreglo para guardar la posicion de los marcos que se liberaron de memoria real
    int cantidadMarcosLiberadosReal = 0; // contador de marcos liberados de memoria real

    int marcosLiberadosVirtual[512]; // arreglo para guardar la posicion de los marcos que se liberaron de memoria virtual
    int cantidadMarcosLiberadosVirtual = 0; // contador de marcos liberados de memoria virtual

    // leer el id del proceso a liberar de memoria
    File>>FileRead;
    istringstream (FileRead) >>idProceso;

    // buscar en memoria real los marcos que pertenecen al id del proceso
    for (int i=0; i<256; i++) {
        if (marcosreal[i]==idProceso){
            marcosreal[i] = -1; // -1 es el valor default de memoria representando vacio
            marcosrealmodificado[i] = 0; // indicar como no modificado
            // actualizar timestamp?
            // registrar los cambios
            marcosLiberadosReal[cantidadMarcosLiberadosReal] = i;
            cantidadMarcosLiberadosReal++;
        }
    }

    // liberar los marcos de pagina en memoria secundaria de swapping
    for (int i=0; i<512; i++){
        if (marcosvirtual[i]==idProceso){
            marcosvirtual[i] = -1; // valor default para indicar que esta vacio
            //registrar los cambios
            marcosLiberadosVirtual[cantidadMarcosLiberadosVirtual] = i;
            cantidadMarcosLiberadosVirtual++;
        }
    }
    // desplegar resultados
    // memoria principal
    if (cantidadMarcosLiberadosReal>0){
        cout << "Estos son los marcos de pagina que se liberaron en memoria real: ";
        for (int i=0; i<cantidadMarcosLiberadosReal; i++){
            cout << marcosLiberadosReal[i] <<", ";
        }
        cout << endl;
    } else {
        cout << "No hay marcos en memoria real con ese ID" << endl;
    }
    // memoria virtual
    if (cantidadMarcosLiberadosVirtual>0){
        cout << "Estos son los marcos de pagina que se liberaron en memoria virtual: ";
        for (int i=0; i<cantidadMarcosLiberadosVirtual; i++){
            cout<< marcosLiberadosVirtual[i]<< ", ";
        }
        cout << endl;
    } else {
        cout << "No hay marcos en memoria virtual con ese ID" << endl;
    }
}

void Debug()
{
   int ini = 0, final = -1;
   cout << "Memoria real" << endl;
int    proceso = -2;
    for (int i = 0; i < 256; i++)
    {
        if(marcosreal[i] != proceso || i == 255 )
        {
            if (i != 0)
            {
                final = i -1;


            if (marcosreal[final] != -1)
            {
                if (i != 255)
              {cout << "Proceso : " << marcosreal[final] << " ocupa paginas " << ini << "-" << final << " en mem real" << endl;}
              else
              {cout << "Proceso : " << marcosreal[final] << " ocupa paginas " << ini << "-" << final+1 << " en mem real" << endl;}
            }
                else
                {
                    if (i != 255)
                    {cout << "Del marco : " << ini << " al marco " << final << " no hay nada" << endl;}
                    else
                    {cout << "Del marco : " << ini << " al marco " << final+1 << " no hay nada" << endl;}
                }

        ini = i;

        }
        proceso = marcosreal[i];

        }

    }
    ini = 0; final = -1;
    proceso = -2;
    cout << "Memoria virtual" << endl;
    for (int i = 0; i < 512; i++)
    {
         if(marcosvirtual[i] != proceso || i == 511)
        {
            if (i != 0)
            {
                final = i -1;

                if (marcosvirtual[final] != -1)
                {
                    if (i != 511)
                  {cout << "Proceso : " << marcosvirtual[final] << " ocupa paginas " << ini << "-" << final << " en mem virtual" << endl;}
                  else
                 {
                   cout << "Proceso : " << marcosvirtual[final] << " ocupa paginas " << ini << "-" << final+1 << " en mem virtual" << endl;
                 }
                }
                else
                {
                    if (i != 511)
                {cout << "Del marco : " << ini << " al marco " << final << " no hay nada" << endl;}
                    else
                {cout << "Del marco : " << ini << " al marco " << final+1 << " no hay nada" << endl;}
                }
            }
            ini = i;

        }
         proceso = marcosvirtual[i];
    }
}

void accesarVirtual()
{
    cout << "corriendo Accesso" << endl;
    int d, p, m;

    //leer d
    File>>FileRead;
    istringstream (FileRead) >>d;

    //leer p
    File>>FileRead;
    istringstream (FileRead) >>p;

    //leer m
    File>>FileRead;
    istringstream (FileRead) >>m;

    int marcovirtualbuscado = d/8;
    int virtualencontrado = -1;

    for(int i = 0; i < 512; i++ )
    {
        if(marcosvirtual[i] == p)
        {

           virtualencontrado = i;
           cout << "Se hizo break!" << endl;
           break;
        }
    }
    if(virtualencontrado != -1)
    {
        int contador = 0;
       for(int i = virtualencontrado; i < 512; i++)
       {
           //se encuentra el marco buscado del proceso
         if(contador == marcovirtualbuscado)
         {
           for (int j = 0; j < 256; j++)
           {
               if (marcosrealtimestamps[j] == marcosvirtualtimestamps[i])
               {
                   cout << "Se encontro la pagina " << marcovirtualbuscado << " del proceso " << p << " en el marco " << virtualencontrado+marcovirtualbuscado << " de la memoria virtual" << endl;
                   cout << "Se encuentra en el marco " << j << " de la memoria real" << endl;
                  if(marcosrealtimestamps[j] == mintimestamp)
                  {
                    mintimestamp++;
                    cout << "El marco " << j << " en real solia ser el LRU. Ya no lo es " << endl;
                  }

                  marcosrealtimestamps[j] =  cputime;
                  cputime++;
                  //si m = 0, referencia
                  if (m == 0)
                    marcosrealreferenciado[j] = 1;
                  //si m = 1, modificacion
                  else
                    marcosrealmodificado[j] = 1;
               }
           }
         }
         contador++;
        }

    }

    else
    {
       cout << "el proceso " << p << "no existe." << endl;
    }


}
int main()
{
    std::fill_n(marcosreal, 256, -1);
    std::fill_n(marcosvirtual, 512, -1);
    //Cargar archivo de texto
    File.open("texto.txt");
    while (!File.eof()) {

        File>>FileRead; //agarra cada caracter separado por un espacio.

        if(FileRead == "P" || FileRead == "p"){cargarProceso();}
        else if(FileRead == "A" || FileRead == "a"){accesarVirtual();}
        else if(FileRead == "L" || FileRead == "l"){liberarPaginas();}
        //else if(FileRead == "E" || FileRead == "e"){E();}
        //else if(FileRead == "F" || FileRead == "f"){F();}
        else{}//cout<<FileRead<<endl;}

}//while
    File.close();

    //Ver si estan correcto los datos
    Debug();

    /*for (int i = 0; i < 255; i++)
    {
        cout << "Proceso : " << marcosreal[i] << endl;
    }
*/


    return 0;
}
