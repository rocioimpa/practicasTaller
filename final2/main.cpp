#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <list>
#include <math.h>
#include <iterator>

#define MAX_RECTANGULOS 8
#define MAX_RECTANGULOS_INTERSEC 4
using namespace std;
struct Rectangulo{
    string id_rect;
    float x_inf_izq;
    float y_inf_izq;
    float x_sup_der;
    float y_sup_der;

};

struct RectanguloSalida{
    string id_rect_1;
    string id_rect_2;
    string id_rect_3;
    float distancia;
    float area;

};

float calcularAreaRectangulo(float base, float altura){
    return base*altura;
}

/*if (RectA.X1 < RectB.X2 && RectA.X2 > RectB.X1 &&
    RectA.Y1 < RectB.Y2 && RectA.Y2 > RectB.Y1) */

void verificarInterseccion(Rectangulo rectanguloActual,Rectangulo rectangulos[], list<Rectangulo*> &listIntersecados){
    for (int i = 0; i < MAX_RECTANGULOS-1; i++){
        if(rectanguloActual.id_rect != rectangulos[i].id_rect){
            if((rectanguloActual.x_inf_izq < rectangulos[i].x_sup_der) && (rectanguloActual.x_sup_der > rectangulos[i].x_inf_izq)
            && (rectanguloActual.y_inf_izq < rectangulos[i].y_sup_der) && (rectanguloActual.y_sup_der > rectangulos[i].y_inf_izq)){
               //Acá habria que agregar el rectangulo al vecto de salida.
               if(listIntersecados.size() < 3){
                listIntersecados.push_back(&rectangulos[i]);
               }
            }
        }
    }
}

float buscarMaximo(float valores[]){
    float valor = 0;
    for (int i = 0; i < 3; i++){
        if (valores[i] >= valor) {
            valor = valores[i];
        }
    }

    return valor;
}

float buscarMinimo(float valores[]){
    float valor = 100;
    for (int i = 0; i < 3; i++){
        if(valores[i] <= valor){
            valor = valores[i];
        }
    }

    return valor;
}

float calcularDistanciaAlOrigen(float x, float y){
    return sqrt(x*x+y*y);

}

void calcularAreaIntersecada(list<Rectangulo*> &listIntersecados, RectanguloSalida* rectanguloSalida){
    string idsRectangulos[3];
    float valoresX_iniciales[3];
    float valoresX_finales[3];
    float valoresY_iniciales[3];
    float valoresY_finales[3];
    int i = 0;
    for (list<Rectangulo*>::iterator iterador = listIntersecados.begin(); iterador != listIntersecados.end(); iterador++){
        Rectangulo* rectangulo = (*iterador);
        idsRectangulos[i] = rectangulo->id_rect;
        valoresX_iniciales[i] = rectangulo->x_inf_izq;
        valoresX_finales[i] = rectangulo->x_sup_der;
        valoresY_iniciales[i] = rectangulo->y_inf_izq;
        valoresY_finales[i] = rectangulo->y_sup_der;
        i++;
    }

    rectanguloSalida->id_rect_1 = idsRectangulos[0];
    rectanguloSalida->id_rect_2 = idsRectangulos[1];
    rectanguloSalida->id_rect_3 = idsRectangulos[2];

    float coordX1 = buscarMaximo(valoresX_iniciales);
    float coordX2 = buscarMinimo(valoresX_finales);
    float coordY1 = buscarMaximo(valoresY_iniciales);
    float coordY2 = buscarMinimo(valoresY_finales);

    float ladoA = abs(coordX2 - coordX1);
    float ladoB = abs(coordY2 - coordY1);

    float area = calcularAreaRectangulo(ladoA,ladoB);
    float distancia = calcularDistanciaAlOrigen(coordX1,coordY2);

    rectanguloSalida->area = area;
    rectanguloSalida->distancia = distancia;
}

int compar(const void* a, const void* b){
    if((*(RectanguloSalida*)a).distancia > (*(RectanguloSalida*)b).distancia) return -1;
    if((*(RectanguloSalida*)a).distancia == (*(RectanguloSalida*)b).distancia) return 0;
    if((*(RectanguloSalida*)a).distancia < (*(RectanguloSalida*)b).distancia) return 1;
}

int main(){
    ifstream archivo;
    Rectangulo rectangulos[MAX_RECTANGULOS];
    RectanguloSalida* rectangulosSalida = new RectanguloSalida[MAX_RECTANGULOS];
    archivo.open("rectangulos.txt");
    if(archivo){
        int i = 0;
        while(!archivo.eof()){
        cout << "i: " << i << endl;
            string id_rect, x_inf_izq, y_inf_izq, x_sup_der, y_sup_der;
            getline(archivo, id_rect, ',');
            rectangulos[i].id_rect = id_rect;
            getline(archivo, x_inf_izq, ',');
            rectangulos[i].x_inf_izq = atof(x_inf_izq.c_str());
            getline(archivo, y_inf_izq, ',');
            rectangulos[i].y_inf_izq = atof(y_inf_izq.c_str());
            getline(archivo, x_sup_der, ',');
            rectangulos[i].x_sup_der = atof(x_sup_der.c_str());
            getline(archivo, y_sup_der, '\n');
            rectangulos[i].y_sup_der = atof(y_sup_der.c_str());
            i++;
        }

    }

    archivo.close();

    for(int i = 0; i < MAX_RECTANGULOS-1 ; i++){
        cout << "ID RECTANGULO: " << rectangulos[i].id_rect << endl;
        cout << "X_INF_IZQ: " << rectangulos[i].x_inf_izq << endl;
        cout << "Y_INF_IZQ: " << rectangulos[i].y_inf_izq << endl;
        cout << "X_SUP_DER: " << rectangulos[i].x_sup_der << endl;
        cout << "Y_SUP_DER: " << rectangulos[i].y_sup_der << endl;
    }

    for(int i = 0; i < MAX_RECTANGULOS -1; i++){
        list<Rectangulo*> listIntersecados;
        listIntersecados.push_back(&rectangulos[i]);
        verificarInterseccion(rectangulos[i], rectangulos, listIntersecados);

        RectanguloSalida unRectanguloSalida = rectangulosSalida[i];
        calcularAreaIntersecada(listIntersecados,&unRectanguloSalida);
        rectangulosSalida[i] = unRectanguloSalida;
    }

    qsort(rectangulosSalida,MAX_RECTANGULOS-1,sizeof(RectanguloSalida),compar);

    ofstream archivoSalida;
    archivoSalida.open("resultadosEjercicioHijoDePuta.txt");

    for(int i = 0; i < MAX_RECTANGULOS-1 ; i++){
        cout << rectangulosSalida[i].area << endl;
        archivoSalida << rectangulosSalida[i].area << endl;
        cout << rectangulosSalida[i].distancia << endl;
        archivoSalida << rectangulosSalida[i].distancia << endl;
        cout << rectangulosSalida[i].id_rect_1 << endl;
        archivoSalida << rectangulosSalida[i].id_rect_1 << endl;
        cout << rectangulosSalida[i].id_rect_2 << endl;
        archivoSalida << rectangulosSalida[i].id_rect_2 << endl;
        cout << rectangulosSalida[i].id_rect_3 << endl;
        archivoSalida << rectangulosSalida[i].id_rect_3 << endl;
        cout << ".........................." << endl;
    }
    return 0;
}
