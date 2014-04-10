#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "control.h"
#define MAXLINE 1000
#define WHITESPACE " \t\n"
#define ERROR_ID "El id del hilo solo puede tener 2 caracteres."

void get_line(char *linea, int maximo);
int parsear(char *comando);


/* Prototipos de Placeholders */
void nuevo_creador(char *id, char *nombre, int inicio, int longitud,
                    int espera, char *tira);
void nuevo_escritor(char *id, char *nombre, int espera, char *tira);
void nuevo_lector(char *id, char *nombre, int espera);
void nuevo_compresor(int variante);
void adelantar(int adelantar);
void nuevo_destructor(char *id, char *nombre, int espera);
void terminar(void);


main()
{
    char linea[MAXLINE];     /* Guarda la linea */
    printf("Bienvenido al parser\n");
    
    do {
        get_line(linea, MAXLINE);
    } while(parsear(linea));
}

int parsear(char *comando)
{
    char *token;
	
    /* comando vacio */
	if((token = strtok(comando, WHITESPACE)) == NULL)
        return 1;
    
    if (strcmp(token, "creador") == 0) {
        /* Arranca un creador */
        char *id;
        char *nombre;
        int inicio;
        int longitud;
        int espera;
        char *tira;

        id = strtok(NULL, WHITESPACE);
        if (strlen(id) != 2) {
            enviar_error(ERROR_ID);
            return 1;
        }

        nombre = strtok(NULL, WHITESPACE);
        inicio = atoi(strtok(NULL, WHITESPACE));
        longitud = atoi(strtok(NULL, WHITESPACE));
        espera = atoi(strtok(NULL, WHITESPACE));
        tira = strtok(NULL, "\n");

        nuevo_creador(id, nombre, inicio, longitud, espera, tira);

    } else if (strcmp(token, "escritor") == 0) {
        /* Arranca un escritor */
        char *id;
        char *nombre;
        int espera;
        char *tira;

        id = strtok(NULL, WHITESPACE);
        if (strlen(id) != 2) {
            enviar_error(ERROR_ID);
            return 1;
        }

        nombre = strtok(NULL, WHITESPACE);
        espera = atoi(strtok(NULL, WHITESPACE));
        tira = strtok(NULL, "\n");

        nuevo_escritor(id, nombre, espera, tira);
    } else if (strcmp(token, "lector") == 0) {
        /* Arranca un lector */
        char *id;
        char *nombre;
        int espera;

        id = strtok(NULL, WHITESPACE);
        if (strlen(id) != 2) {
            enviar_error(ERROR_ID);
            return 1;
        }

        nombre = strtok(NULL, WHITESPACE);
        espera = atoi(strtok(NULL, WHITESPACE));

        nuevo_lector(id, nombre, espera);
    } else if (strcmp(token, "compresor") == 0) {
        /* Arranca un compresor */
        token = strtok(NULL, WHITESPACE);
        if (strcmp(token, "primera") == 0) {
            /* compresor primera variante */
            nuevo_compresor(1);
        } else if (strcmp(token, "segunda") == 0) {
            /* compresor segunda variante */
            nuevo_compresor(0);
        } else {
            enviar_error("Argumento invalido para 'compresor'\n");
        }
    } else if (strcmp(token, "adelantar") == 0) {
        /* Indica si se debe adelantar */
        token = strtok(NULL, WHITESPACE);
        if (strcmp(token, "si") == 0) {
            /* adelantar si */
            adelantar(1);
        } else if (strcmp(token, "no") == 0) {
            /* adelantar no */
            adelantar(0);
        } else {
            enviar_error("Argumento invalido para 'adelantar'\n");
        }
    } else if (strcmp(token, "destructor") == 0) {
        /* Arranca un destructor */
        char *id;
        char *nombre;
        int espera;

        id = strtok(NULL, WHITESPACE);
        if (strlen(id) != 2) {
            enviar_error(ERROR_ID);
            return 1;
        }

        nombre = strtok(NULL, WHITESPACE);
        espera = atoi(strtok(NULL, WHITESPACE));

        nuevo_destructor(id, nombre, espera);
    } else if (strcmp(token, "terminar") == 0) {
        /* Termina el programa */
        terminar();
        return 0;
    } else {
        /* Comando invalido */
        enviar_error("Comando invaldio\n");
    }
    return 1;
}

/* get_line: recibe una linea y la pone en s,
    con el maximo de caracteres dado por lim.
    tomado de K&R*/
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


void nuevo_creador(char *id, char *nom, int ini, int lon, int esp,
                      char *tira) {
    /*extern*/ int tamano_arreglo /*;*/ = 100;
    if ((ini + lon) > tamano_arreglo) {
        enviar_error("Tamaño insuficiente en el arreglo\n");
        return;
    }
    if (obtener(nom) != NULL) {
        enviar_error("Ya existe un creador con ese nombre");
        return;
    }
        
    creador *p, cre;
    p = malloc(sizeof(creador));
    cre = *p;

    tira[lon-1] = '\0';

    cre.id = id;
    cre.nombre = nom;
    cre.inicio = ini;
    cre.longitud = lon;
    cre.espera = esp;
    cre.tira = tira;
    agregar(nom, ini, lon);
    printf("Nuevo creador, id: %s, nombre: %s, inicio: %d, longitud: %d, "
           "espera: %d, tira: %s\n",
           cre.id, cre.nombre, cre.inicio, cre.longitud, cre.espera, cre.tira);
    return;
}

void nuevo_escritor(char *id, char *nom, int esp, char *tira) {
    nodo *sector;
    if ((sector = obtener(nom)) == NULL) {
        enviar_error("No existe un sector con ese nombre");
        return;
    }

    escritor *p, es;
    p = malloc(sizeof(escritor));
    es = *p;

    tira[sector->longitud-1] = '\0';

    es.id = id;
    es.nombre = nom;
    es.espera = esp;
    es.tira = tira;

    printf("Nuevo escritor, id: %s, nombre: %s, espera: %d, tira: %s\n",
            es.id, es.nombre, es.espera, es.tira);
    return;
}

void nuevo_lector(char *id, char *nom, int esp) {
    nodo *sector;
    if ((sector = obtener(nom)) == NULL) {
        enviar_error("No existe un sector con ese nombre");
        return;
    }

    lector *p, le;
    p = malloc(sizeof(lector));
    le = *p;

    le.id = id;
    le.nombre = nom;
    le.espera = esp;

    printf("Nuevo lector, id: %s, nombre: %s, espera: %d\n",
           le.id, le.nombre, le.espera);
    return;
}

void nuevo_compresor(int variante) {
    compresor *p, ce;
    p = malloc(sizeof(compresor));
    ce = *p;

    ce.tipo = variante;

    char *tipo = (variante) ? "primera" : "segunda";
    printf("Nuevo compresor de la %s variante\n", tipo);
    return;
}

void adelantar(int adelantar) {
    char *activado = (adelantar) ? "activado" : "desactivado";
    printf("Adelanto %s\n", activado);
    return;
}

void nuevo_destructor(char *id, char *nom, int esp) {
    nodo *sector;
    if ((sector = obtener(nom)) == NULL) {
        enviar_error("No existe un sector con ese nombre");
        return;
    }

    destructor *p, de;
    p = malloc(sizeof(destructor));
    de = *p;

    de.id = id;
    de.nombre = nom;
    de.espera = esp;

    printf("Nuevo destructor, id: %s, nombre: %s, espera: %d\n",
           de.id, de.nombre, de.espera);
    return;
}

void terminar(void) {
    printf("Adios\n");
    return;
}
