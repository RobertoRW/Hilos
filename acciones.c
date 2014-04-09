#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "control.h"
#include "hilos_struct.h"

/* Temporal, el arreglo y su longitud */
int maximo = 100;
char arreglo[100];
void enviar_mensaje(char*);

void crear(creador *creador);
void escribir(escritor *es);
void leer(lector *le);
void destruir(destructor *de);
/*compresor *nuevo_compresor(int variante); */


void get_line(char *linea, int maximo);

int main(void)
{
    char tira[100];
    get_line(tira, 100);
}

void crear(creador *cr)
{
    char *nom = cr->nombre;
    int ini = cr->inicio;
    int lon = cr->longitud;
    char *tira = cr->tira;
   
    tira[lon-1] = '\0';   /* trunca la tira */

    agregar(nom, ini, lon);
    strcpy(arreglo+ini, tira);

}


void escribir(escritor *es)
{
    char *tira = es->tira;
    char *nom = es->nombre;
    
    nodo *segmento = obtener(nom);
    int ini = segmento->inicio;
    int lon = segmento->longitud;

    tira[lon-1] = '\0';
    strcpy(arreglo+ini, tira);
}

void leer(lector *le)
{
    char *nom = le->nombre;
    
    nodo *segmento= obtener(nom);

    int i; 
    char tira[segmento->longitud];
    for (i = 0; i < segmento->longitud; i++)
        tira[i] = arreglo[segmento->inicio + i];

   enviar_mensaje(tira);
}

void destruir(destructor *de)
{
    char *nom = de->nombre;
    
    eliminar(nom);
}


/* FIN DE EL CODIGO

EL RESTO ES PARA HACER PRUEBAS */

void get_line(char s[], int lim)
{
    int c, i;

    for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
        s[i] = c;
    if (c == '\n') {
        s[i++] = c;
    }
    s[i] = '\0';
    return;
}

void enviar_mensaje(char *str) {
    printf("%s\n", str);
}

/* basura */
/*
    if ((ini + lon) > maximo)
        ; 
        
    creador *cre;
    cre = malloc(sizeof(creador));

    tira[(lon-1)] = '\0';

    cre->id = id;
    cre->nombre = nom;
    cre->inicio = ini;
    cre->longitud = lon;
    cre->espera = esp;
    cre->tira = tira;*/
