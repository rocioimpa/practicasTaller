#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>
using namespace std;

#define MAX_BALAS 7

struct bala{
    string id;
    float x0, y0, xd, yd;
};

struct balaSalida{
    float longRecorrido;
    string idRecorrido;
    string destino;
};

bool choqueBordeSup(float* xf, float a, float b){
    float y = 60;
    (*xf) = (y - b)/a;

    if((*xf) <= 60){
        return true;
    }
    return false;

}

bool choqueBordeInf(float* xf, float a, float b){
    float y = 0;
    (*xf) = (y - b)/a;

    if((*xf) < 60){
        return true;
    }
    return false;

}

bool choqueBorde1(float* xf, float a, float b){
    float pendienteBorde1 = (40 - 60)/ (65 - 60);
    float ordenadaBorde1 = (40 - (65*pendienteBorde1));

    (*xf) = (ordenadaBorde1-b) / (a - pendienteBorde1);

    if((*xf) <= 65){
        return true;
    }
    return false;

}

bool choqueBorde2(float* yf, float a, float b){
    float x = 65;
    (*yf) = a*x + b;

    if(((*yf) >= 20) && ((*yf) < 40)){
        return true;
    }
    return false;

}

bool choqueBorde3(float* xf, float a, float b){
    float pendienteBorde3 = (65 - 60) / 20;
    float ordenadaBorde3 = 65 - (pendienteBorde3*20);

    (*xf)  = (ordenadaBorde3 - b) / (a - pendienteBorde3);

    if(((*xf)  >= 60)){
        return true;
    }
    return false;

}

void calcularLongitudRecorrida(float *longitud, float* xf, float* yf, float* xd, float* yd){
    float diferenciaX = (*xf)-(*xd);
    float diferenciaY = (*yf)-(*yd);

    float distancia = sqrt((diferenciaX*diferenciaX) + (diferenciaY*diferenciaY));
    (*longitud) = distancia;
}

void determinarBordeChoque(float a, float b, float* xf, float* yf, string *destino, float *longitud, float* xd, float *yd){
    //Choca con borde superior
    if(choqueBordeSup(xf, a, b)){
        *yf = a*(*xf) + b;
        *destino = "Borde_Sup";
    }else if(choqueBorde1(xf, a, b)){
        *yf = a*(*xf) + b;
        *destino = "Borde_1";
    }else if(choqueBorde2(yf, a, b)){
        *xf = ((*yf) - b)/a;
        *destino = "Borde_2";
    } else if(choqueBorde3(xf, a, b)){
        *yf = a*(*xf) + b;
        *destino = "Borde_3";
    }else{
        choqueBordeInf(xf, a, b);
        *yf = a*(*xf) + b;
        *destino = "Borde_Inf";
    }

    calcularLongitudRecorrida(longitud,xf,yf,xd,yd);
}

int compare (const void * a, const void * b){
    if( (*(balaSalida*)a).longRecorrido < (*(balaSalida*)b).longRecorrido ) return -1;
    if( (*(balaSalida*)a).longRecorrido == (*(balaSalida*)b).longRecorrido ) return 0;
	if( (*(balaSalida*)a).longRecorrido > (*(balaSalida*)b).longRecorrido ) return 1;
}

int main(){
    ifstream archivo;
    ofstream salida;
    bala arrayBalas[MAX_BALAS];
    balaSalida* arrayBalasSalida = new balaSalida[MAX_BALAS];
    archivo.open("../balas.txt");
    string id, x0, y0, xd, yd;

    //Cargamos el struct con los datos del archivo
    if(archivo){
        int i = 0;
        while (!archivo.eof()) {
        float x0_f = 0, y0_f = 0, xd_f = 0, yd_f = 0;
            cout << i << endl;
            getline(archivo,id,',');
            arrayBalas[i].id = id;
            getline(archivo,x0,',');
            x0_f = atof(x0.c_str());
            arrayBalas[i].x0 = x0_f;
            cout << "x0: " << arrayBalas[i].x0 << endl;
            getline(archivo,y0,',');
            y0_f = atof(y0.c_str());
            arrayBalas[i].y0 = y0_f;
            cout << "y0: " << arrayBalas[i].y0 << endl;
            getline(archivo,xd,',');
            xd_f = atof(xd.c_str());
            arrayBalas[i].xd = xd_f;
            cout << "xd: " << arrayBalas[i].xd << endl;
            getline(archivo,yd,'\n');
            yd_f = atof(yd.c_str());
            arrayBalas[i].yd = yd_f;
            cout << "yd: " << arrayBalas[i].yd << endl;
            i++;
        }
    }

    for (int i = 0 ; i < MAX_BALAS -1 ; i++){
        float x0 = arrayBalas[i].x0;
        float y0 = arrayBalas[i].y0;
        float xd = arrayBalas[i].xd;
        float yd = arrayBalas[i].yd;

        //Calculamos las rectas de las balas
        float pendiente = ((yd-y0)/(xd-x0));
        float ordenadaAlOrigen = yd-pendiente*xd;

        //Estos van a ser los puntos donde la bala choca con los bordes.
        float xf, yf;
        arrayBalasSalida[i].idRecorrido = arrayBalas[i].id;
        determinarBordeChoque(pendiente, ordenadaAlOrigen, &xf, &yf, &arrayBalasSalida[i].destino, &arrayBalasSalida[i].longRecorrido,&xd,&yd);


    }

    qsort(arrayBalasSalida,MAX_BALAS-1,sizeof(balaSalida),compare);

    salida.open("resultadosNoeYRo.txt");
    for (int i = 0; i < MAX_BALAS-1 ; i++){
        salida << arrayBalasSalida[i].idRecorrido << endl;
        cout << arrayBalasSalida[i].idRecorrido << endl;
        salida << arrayBalasSalida[i].destino << endl;
        cout << arrayBalasSalida[i].destino << endl;
        salida << arrayBalasSalida[i].longRecorrido << endl;
        cout << arrayBalasSalida[i].longRecorrido << endl;
        salida << "-------------------------------------" << endl;
        cout << "-------------------------------------" << endl;
    }

    archivo.close();

    return 0;
}
