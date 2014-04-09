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
void comprimir(void);

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
    nodo *segmento = obtener(nom);

    int i; 
    char tira[segmento->longitud];
    for (i = 0; i < segmento->longitud; i++)
        tira[i] = arreglo[segmento->inicio + i];

    char *mensaje;
    sprintf(mensaje, "ID: %s, Tira: \"%s\", Ultimo escritor: %s, Lector: %s\n", 
                segmento->ID, tira, segmento->ultimo_escritor, nom);
    enviar_mensaje(mensaje);
}

void destruir(destructor *de)
{
    char *nom = de->nombre;
    
    eliminar(nom);
}

void comprimir(void)
{
    extern nodo *primero;
    nodo *p;
    int prev = 0;
    for (p = primero; p != NULL; p = p->siguiente)
        if (p->inicio > prev) {
            int n = p->inicio;
            p->inicio = prev;
            while(n < (p->inicio + p->longitud))
                arreglo[prev++] = arreglo[n++];
        } else
            prev += p->longitud;
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
