#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "control.h"
#include "hilos_struct.h"

/* Temporal, el arreglo y su longitud */
int maximo = 100;
char arreglo[100];

creador *nuevo_creador(char *id, char *nombre, int inicio, int longitud,
                       int espera, char *tira);
escritor *nuevo_escritor(char *id, char *nombre, int espera, char *tira);
lector *nuevo_lector(char *id, char *nombre, int espera);
/*compresor *nuevo_compresor(int variante);
destructor *nuevo_destructor(char *id, char *nombre, int espera);
void adelantar(int adelantar);*/


void get_line(char *linea, int maximo);

int main(void)
{
    char tira[100];
    get_line(tira, 100);
    nuevo_creador("AE", "C", 8, 8, 34, tira);
    nuevo_lector("EE", "C", 34);
}

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
    tira[(lon-1)] = '\0';

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


escritor *nuevo_escritor(char *id, char *nombre, int espera, char *tira)
{
    escritor *escr;
    escr = malloc(sizeof(escritor));

    tira[(lon-1)] = '\0';
    escr->id = id;
    escr->nombre = nombre;
    escr->espera = espera;
    
}

lector *nuevo_lector(char *id, char *nombre, int espera) 
{
    nodo *segmento;
    segmento = obtener(nombre);

    int i;
    for (i = segmento->inicio; i < (segmento->inicio + segmento->longitud); i++)
        if (arreglo[i] == '\0')
            printf("\\0");
        else
            putchar(arreglo[i]);
    putchar('\n');
    return NULL;
}


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
