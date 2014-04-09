#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "control.h"
#include "hilos_struct.h"

/* Temporal, el arreglo y su longitud */
int maximo = 100;
int arreglo[maximo];

creador *nuevo_creador(char *id, char *nombre, int inicio, int longitud,
                       int espera, char *tira);
escritor *nuevo_escritor(char *id, char *nombre, int espera, char *tira);
lector *nuevo_lector(char *id, char *nombre, int espera);
compresor *nuevo_compresor(int variante);
destructor *nuevo_destructor(char *id, char *nombre, int espera);
void adelantar(int adelantar);


creador *nuevo_creador(char *id, char *nom, int ini, int lon, int esp,
                       char *tira)
{
    /* si no hay espacio */
    if ((ini + lon) > maximo)
        ; /* ENVIAR ERROR ---- POR IMPLEMENTAR */
        
    /* crea la estructura */
    creador *cre;
    cre = malloc(sizeof(creador));

    /* asegura que la tira no se exeda de la longitud permitida */
    tira[lon-1] = '\0';

    cre->id = id;
    cre->nombre = nom;
    cre->inicio = ini;
    cre->longitud = lon;
    cre->espera = esp;
    cre->tira = tira;

    agregar(nom, ini, lon);
    strcpy(arreglo+ini, tira);

    return cre;
}
