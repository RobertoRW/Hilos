#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "control.h"

void recorrer(void);

static nodo *primero = NULL; 

void main() {
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

}

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
    nuevo_nodo->compresor_esperando = 0;
    nuevo_nodo->destructor_esperando = 0;

    for (cur = primero, prev = NULL; cur != NULL && cur->inicio <= ini;
            prev = cur, cur = cur->siguiente)
    ;

    nuevo_nodo->siguiente = cur;
    if (prev == NULL) {
        primero = nuevo_nodo;
    } else
        prev->siguiente = nuevo_nodo;
}

nodo *obtener(char *nombre)
{
    nodo *cur;

    for (cur = primero; cur != NULL && strcmp(cur->ID, nombre);
            cur = cur->siguiente)
        ;

    if (cur == NULL)
        return NULL;
    else if(strcmp(cur->ID, nombre) == 0)
        return cur;
}

void eliminar(char *nombre)
{
    nodo *cur, *prev;
    
    for (cur = primero, prev = NULL; cur != NULL && strcmp(cur->ID, nombre);
            prev = cur, cur = cur->siguiente)
        ; 
    
    if(cur == NULL)
        return;

    if(prev == NULL)
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
        printf("Nombre %s, Inicio %d\n", cur->ID, cur->inicio);
    }
}
