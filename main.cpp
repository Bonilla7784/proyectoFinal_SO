#include <stdio.h>
#include <stdlib.h>
#include<bits/stdc++.h>
using namespace std;

//Funciones a utilzar
void despliegaMarcos(int);
int sigMarcoDisponible(int);
void print(unordered_set<int> const &s);
void LRU_replacement();

// Variables globales con sus valores iniciales
int secuencia[30];
double lenSecuencia = 30;
int marcos[5] = {-1, -1, -1, -1, -1}; // -1 indica marco de página desocupado
int numMarcos = 5;
int arrFallos[30] = {0}; // variable que guarda los page fault del fifo, 0 es ninguno y 1 es page fault
double nfp = 0; // page faults
double rendimiento; // variable que guarda el rendimiento

int main(int argc, char *argv[]) {
  int POLITICA;
  cout << "OPRIMA 0 PARA FIFO O OPRIMA 1 PARA LRU\n";
  cin >> POLITICA;
  switch(POLITICA){
    case 0: 
      int i, j;  // índices para fors

      srand(time(0));

      for(int i = 0; i < lenSecuencia; i++){
        secuencia[i] = (rand() % 10) + 1;
      }

      // Desplegamos la secuencia de referencias
      printf("Secuencia de referencias:\n");
      for(i=0; i<lenSecuencia; i++)
        printf("%d ", secuencia[i]);
        printf("\n");

      // Asignamos las referencias en los marcos de página disponibles
      for(i=0; i<lenSecuencia; i++) {
        j = sigMarcoDisponible(i);
        if (j != -1) {
        nfp++;
        arrFallos[i] = 1;
        marcos[j]=secuencia[i];
        despliegaMarcos(i);
        }
        else {
        printf("Referencia %d: %d actualmente en memoria.\n",
          i+1, secuencia[i]);
        }
      }
      printf("Total de fallos de página: %f.\n", nfp);
      rendimiento = nfp / lenSecuencia;
      cout << "Rendimiento: " <<  rendimiento << endl;
      break;
    case 1:

      srand(time(0));

      for(int i = 0; i < lenSecuencia; i++){
        secuencia[i] = (rand() % 10) + 1;
      }

      cout<<"Secuencia de referencias:\n";
      for(int i=0; i<lenSecuencia; i++){
        cout << secuencia[i] << " ";
      }
      cout << "\n";

      LRU_replacement();
      rendimiento = nfp / lenSecuencia;
      cout << "Rendimiento: " <<  rendimiento << endl;
      break;
  }
  exit(0);
}

int sigMarcoDisponible(int index) {
  /* Esta función implementa el FIFO
   * Regresa el índice del marco de página donde almacenar
   * la página correspondiente a la referencia actual de la secuencia.
   * "index" es el índice de la referencia actual dentro del arreglo
   * secuencia[].
   * La implementación contempla los siguientes casos:
   * 1) Si la referencia actual ya está en un marco de memoria, entonces
   * no hay fallo de página, por lo que la función regresa -1.
   * 2) Si hay algún marco de página desocupado, regresa el índice de ese
   * marco de página.
   * 3) Sino, entonces buscamos la siguiente referencia de cada página
   * actualmente en memoria y calculamos su "distancia" a partir de
   * la referencia actual.
   */
  int i, j; // indices para recorrer los arreglos.
  int ra;   // referencia actual.
  int aparece; // para indicar si una página aparece en la sec de refs
  int dmin;    // distancia máxima a la siguiente referencia
  int imax;    // índice de la distancia máxima.
  int distSigRefencia[30]; // Arreglo para almacenar distancia a sig ref
  /*
   * 1) Si la referencia actual está en algún marco, no hay
   * fallo de página, por tanto regresa -1
   */
  ra = secuencia[index];
  for(i=0; i<numMarcos; i++) {
    if (marcos[i] == ra)
      return -1;
  }
  /*
   * 2) Si encontramos un marco de página desocupado (contenido == -1)
   * devolvemos su índice
   */
  for(i=0; i<numMarcos; i++)
    if (marcos[i] == -1)
      return i;
  /*
   */
  /* 3) Sino, entonces buscamos la siguiente referencia de cada página
   * actualmente en memoria y calculamos su "distancia" a partir de
   * la referencia actual.
   */
  for(i=0; i<numMarcos; i++) {
    ra = marcos[i];
    for(j= index; j>=0; j--){
      if (secuencia[j] == ra) {
        if (arrFallos[j] == 1) {
          distSigRefencia[i] = j;
          break;
        }
      }
    }
  }
  /* Ahora buscamos el índice de la referencia que mas tiempo tiene en memoria y se regresa ese indice.
   */
  dmin = 20;
  for(i=0; i<numMarcos; i++) {
    if(distSigRefencia[i] < dmin) {
      dmin = distSigRefencia[i];
      imax = i;
    }
  }
  return imax;
}

void despliegaMarcos(int i){
  /*
   * Esta función despliega los marcos de página
   * i es el número de referencia dentro de la
   * secuencia de referencias.
   */
  int j;
  printf("Referencia %d: %d\n", i+1, secuencia[i]);
  printf("Marcos de página: ");
  for(j=0; j<numMarcos; j++)
    printf("%d ", marcos[j]);
  printf("\n");
}

//Funcion para imprimir el unordered set
void print(unordered_set<int> const &s)
{
    copy(s.begin(),
            s.end(),
            ostream_iterator<int>(cout, " "));
}

//Funcion que realiza el algoritmo LRU
void LRU_replacement()
{
    //Declaracion de set para la secuencia y del map para los index
    unordered_set<int> s;
    unordered_map<int, int> index;

    //For para recorrer nuestra secuencia generada
    for (int i=0; i<lenSecuencia; i++)
    { 
        if(s.find(secuencia[i])!=s.end())//Condicion para cuando la pagina ya esta en la memoria
        {
            cout << "Referencia " << secuencia[i] << " actualmente en memoria\n";
            printf("Marcos de página: ");
            print(s);
            cout << "\n";
        }
        else
        {
            if (s.size() < numMarcos)//Condicion para insertar las paginas
            {
                s.insert(secuencia[i]);
                nfp++;
            }
            else
            {
                //Aqui realizamos la operacion de remover la pagina que menor no se ha frecuentado
                int lru = INT_MAX, val;
                for (auto it : s)
                {
                    if (index[it] < lru)
                    {
                        lru = index[it];
                        val = it;
                    }
                }
                s.erase(val);
                s.insert(secuencia[i]);
                nfp++;
            }
            //Mostramos el marco de pagina actual
            printf("Marcos de página: ");
            print(s);
            cout << "\n";
        }
        index[secuencia[i]] = i;
    }
    cout << "Total de fallos de pagina: " << nfp << "\n";
}
