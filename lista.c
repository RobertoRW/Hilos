#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "control.h"
/* lista.c
/  Mantiene la lista enlazada */
void recorrer(void);

nodo *primero = NULL; /* primer nodo de la lista */ 

/* la funcion main es solo para las pruebas */
/* void main() {
    agregar("A", 3, 2);
    agregar("E", 1, 2);
    agregar("3", 0, 2);
    agregar("G", 0, 8);
    agregar("l", 3, 2);
    
    recorrer();
    
    eliminar("3");
    putchar('\n');
    eliminar("G");

    recorrer();
    
    putchar('\n');

    nodo *lol;
    lol = obtener("l");
    if (lol != NULL)
        printf("Nodo %s inicia en %d\n", lol->ID, lol->inicio);

}*/
/* agrega un nodo con el nombre, inicio y longitud dados */
void agregar(char *nombre, int ini, int lon)
{
    nodo *cur, *nuevo_nodo, *prev;

    nuevo_nodo = malloc(sizeof(nodo));
    nuevo_nodo->ID = nombre;
    nuevo_nodo->inicio = ini;
    nuevo_nodo->longitud = lon;
    nuevo_nodo->ultimo_escritor = NULL;
    nuevo_nodo->escritor_adentro = 0;
    nuevo_nodo->lector_adentro = 0;
    nuevo_nodo->destructor_adentro = 0;
    nuevo_nodo->escritores_esperando = 0;
    nuevo_nodo->lectores_esperando = 0;
    nuevo_nodo->destructor_esperando = 0;

    /* Busca la posicion que le corresponde para mantener la lista ordenada */
    for (cur = primero, prev = NULL; cur != NULL && cur->inicio <= ini;
            prev = cur, cur = cur->siguiente)
    ;

    nuevo_nodo->siguiente = cur;
    if (prev == NULL) {     /* si la lista esta vacia */
        primero = nuevo_nodo;
    } else
        prev->siguiente = nuevo_nodo;
}

/* obtiene el nodo con el nombre dado */
nodo *obtener(char *nombre)
{
    nodo *cur;

    /* recorre la lista hasta que encuentre el elemento o hasta que se termine.
       strcmp(s,s) compara que dos strings sean iguales, retorna 0 si son
       iguales */
    for (cur = primero; cur != NULL && strcmp(cur->ID, nombre);
            cur = cur->siguiente)
        ;

    if (cur == NULL)
        return NULL;    /* no se encontro nada */
    else if(strcmp(cur->ID, nombre) == 0)
        return cur;
}

/* elimina de la lista el nodo con el nombre dado */
void eliminar(char *nombre)
{
    nodo *cur, *prev;
    
    for (cur = primero, prev = NULL; cur != NULL && strcmp(cur->ID, nombre);
            prev = cur, cur = cur->siguiente)
        ; 
    
    if(cur == NULL)
        return;      /* si no lo encontro */

    if(prev == NULL) /* si es el primer elemento */
        primero = cur->siguiente;
    else if(strcmp(cur->ID, nombre) == 0)
        prev->siguiente = cur->siguiente;

    free(cur);
    return;
}

void recorrer(void)
{
    nodo *cur;

    for (cur = primero; cur != NULL; cur = cur->siguiente) {
        pthread_cond_signal(cur->espera);
    }
}
