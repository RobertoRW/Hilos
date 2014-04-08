#include <stdio.h>
#include <stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<stdlib.h>
#include	<string.h>
#include <stdbool.h>
#include <pthread.h>
#include	"msg.h"
#include	"mem.h"
#include	"control.h"

#define MAXLINE 1000
#define WHITESPACE " \t\n"

//########### VARIABLES GLOBALES ######################

//****BANDERAS DE COMPORTAMIENTO DEL SISTEMA *****************
// Modifican las prioridades de los tipos de hilos

bool escritor_adelanta;

//*************************** DEFINICIONES GLOBALES *********************

//Identificadores de memoria y semaforos
int memid, sem_salida, sem_hilos;

//Puntero a memoria compartida
Msg *ptr_msg;

//Puntero a arreglo en comun
char *arreglo;


//****************** ESTRUCTURAS DE CONTROL *************************

//**** Mutexes, variables de condicion y estructuras de control que protegen

//Escritura (controla uso de memoria compartida entre los hilos, para que solo uno pueda pasar un mensaje
//al proceso de salida a la vez
pthread_cond_t salida = PTHREAD_COND_INITIALIZER;
pthread_mutex_t salida_lock = PTHREAD_MUTEX_INITIALIZER;
bool alguien_escribiendo_salida = false;
//Puntero al primer nodo de la lista de control
Nodo_control * nodo;

//****************PROTOTIPOS DE FUNCIONES***************

void enviar_error(char *error);
void funcion_hilo_usuario();
void terminar();
void get_line(char s[], int lim);

//Funcion main
int main( int argc, char *argv[] )
{

	if ( (memid=shmget(LLAVE_MEM,sizeof(Msg),PERMS|IPC_CREAT))<0)
		printf("Error: la petición de memoria falló.\n");

	if ( (ptr_msg=(Msg *) shmat(memid,(char *)0,0))== (Msg *) -1)
		printf("Error: la conexión a la memoria compartida falló.\n");
    ptr_msg->terminar = false;

	if ( (sem_salida=sem_create(LLAVE_SEM_SALIDA,0))<0)
		printf("Error: no se pudo crear semáforo de salida.\n");

	if ( (sem_hilos=sem_create(LLAVE_SEM_HILOS,0))<0)
		printf("Error: no se pudo crear semáforo de hilos.\n");
    //REALIZAR PROCESAMIENTO ACA

    if (argc==1){  //Si no hay argumentos.
        enviar_error("No se indico tamano del arreglo.\n");
        terminar();
        return 1;
    }

    arreglo = malloc(sizeof(char) * (*argv[1])-'0'); //Tamano del arreglo es el segundo argumento del CLI (el primero es el nombre del programa).
    //Arreglo inicializado en el tamano especificado por el usuario.

    pthread_t hilo_usuario;
    pthread_create(&hilo_usuario, NULL, (void *)funcion_hilo_usuario, NULL); //Lanza el hilo que recibe los comandos.








    pthread_join(hilo_usuario, NULL);
    terminar();
    //FIN DEL PROGRAMA




}

void enviar_error(char *error){
    pthread_mutex_lock(&salida_lock); //Intenta entrar en la estructura de la salida.
    while (alguien_escribiendo_salida) pthread_cond_wait(&salida, &salida_lock); //Mientras haya alguien escribiendo, se espera.
    //REGION CRITICA
    //Si llega acá, entonces nadie esta escribiendo y tiene control del mutex.
        alguien_escribiendo_salida = 1; //Indica a todos que hay alguien escribiendo.
        char mensaje[MAX_TAM_MSG];
        memcpy(mensaje, error, strlen(error));
        mensaje[strlen(error)] ='\0'; //Concatenar el caracter nulo al mensaje de error,
                            //para que la salida sepa cuando terminar de imprimir.
        memcpy(ptr_msg->mesg_data, mensaje, strlen(mensaje)+1);//copiar el mensaje en la memoria compartida.
        ptr_msg->mesg_type = 1; //El mensaje es un error.
        sem_signal(sem_salida); //Avisar al impresor de errores.
        sem_wait(sem_hilos);//Esperar a que el impresor termine.
        //FUERA DE REGION CRITICA
        alguien_escribiendo_salida = 0;
        pthread_cond_broadcast(&salida); //Avisa a todos los demas que ya termino, para que alguno entre.
        pthread_mutex_unlock(&salida_lock); //Suelta el mutex para que los demás puedan chequear el valor de la variable.
}

void enviar_mensaje(char *msj){
    pthread_mutex_lock(&salida_lock); //Intenta entrar en la estructura de la salida.
    while (alguien_escribiendo_salida) pthread_cond_wait(&salida, &salida_lock); //Mientras haya alguien escribiendo, se espera.
    //REGION CRITICA
    //Si llega acá, entonces nadie esta escribiendo y tiene control del mutex.
        alguien_escribiendo_salida = 1; //Indica a todos que hay alguien escribiendo.
        char mensaje[MAX_TAM_MSG];
        memcpy(mensaje, msj, strlen(msj));
        mensaje[strlen(msj)] ='\0'; //Concatenar el caracter nulo al mensaje
                            //para que la salida sepa cuando terminar de imprimir.
        memcpy(ptr_msg->mesg_data, mensaje, strlen(mensaje)+1);//copiar el mensaje en la memoria compartida.
        ptr_msg->mesg_type = 0; //El mensaje no es un error.
        sem_signal(sem_salida); //Avisar al impresor de errores.
        sem_wait(sem_hilos);//Esperar a que el impresor termine.
        //FUERA DE REGION CRITICA
        alguien_escribiendo_salida = 0;
        pthread_cond_broadcast(&salida); //Avisa a todos los demas que ya termino, para que alguno entre.
        pthread_mutex_unlock(&salida_lock); //Suelta el mutex para que los demás puedan chequear el valor de la variable.
        return;
}

void terminar_salida(){
    ptr_msg->terminar = true;
    printf("Terminando salida\n");
    sem_signal(sem_salida); //Avisar al impresor de errores.
    sem_wait(sem_hilos);//Esperar a que el impresor termine.
}

void terminar(){
	terminar_salida();
	if (shmdt(ptr_msg)<0)
		enviar_error("Error: no se pudo desconectar de la memoria compartida.\n");
    printf("Memoria desconectada.\n");

    fprintf(stderr, "Destruyendo el área de memoria compartida.\n");
	if (shmctl(memid,IPC_RMID,(struct shmid_ds *)0)<0)
		err_sys("client: can't remove shared memory");

    fprintf(stderr, "Destruyendo los semáforos.\n");
	sem_rm(sem_salida);
	sem_rm(sem_hilos);
}

//************************--------PROCESOS DE LOS HILOS---------------****************************
//***** HILO USUARIO : Maneja el parseo de la linea de usuario
// Utiliza las funciones de get_line y de parsear. Solo él las usa, por lo que no es
// necesario protegerlas (no forman parte de la región crítica)

void funcion_hilo_usuario(){
    char linea[MAXLINE];     /* Guarda la linea */
    enviar_mensaje("Bienvenido al parser\n");

    do{
        //printf("hilos_sys>> ");
        get_line(linea, MAXLINE);
    } while(parsear(linea) > 0);
}

    //parsea una linea de comandos
int parsear(char *comando)
{
    char *token;

	if((token = strtok(comando, WHITESPACE)) == NULL)
        return 1;

    if (strcmp(token, "creador") == 0) {
        /* Arranca un creador */
        char *id;
        token = strtok(NULL, WHITESPACE);
        if (token != NULL){
            if (strlen(token) != 2) {
                enviar_error("El id del hilo solo puede tener 2 caracteres.\n");
                return 1;
            }
        }
        else {
            enviar_error("Faltan argumentos.\n");
            return 1;
        }

    } else if (strcmp(token, "escritor") == 0) {
        /* Arranca un escritor */
        token = strtok(NULL, WHITESPACE);

        return 1;
    } else if (strcmp(token, "lector") == 0) {
        /* Arranca un lector */
        token = strtok(NULL, WHITESPACE);

        return 1;
    } else if (strcmp(token, "compresor") == 0) {
        /* Arranca un compresor */
        token = strtok(NULL, WHITESPACE);


        if (strcmp(token, "primera") == 0) {
            /* compresor primera variante */

            return 1;
        } else if (strcmp(token, "segunda") == 0) {
            /* compresor segunda variante */

            return 1;
        } else {
            enviar_error("Argumento invalido para 'compresor'\n");

            return 1;
        }
    } else if (strcmp(token, "adelantar") == 0) {
        /* Indica si se debe adelantar */
        token = strtok(NULL, WHITESPACE);


        if (strcmp(token, "si") == 0) {
            /* adelantar si */

            return 1;
        } else if (strcmp(token, "no") == 0) {
            /* adelantar no */

            return 1;
        } else {
            enviar_error("Argumento invalido para 'adelantar'\n");
            return 1;
        }
    } else if (strcmp(token, "destructor") == 0) {
        /* Arranca un destructor */
        token = strtok(NULL, WHITESPACE);

        return 1;
    } else if (strcmp(token, "terminar") == 0) {
        /* Termina el programa */
        enviar_mensaje("Adios\n");
        return 0;
    } else {
        /* Comando invalido */
        char* p = "Comando invalido\n";
        enviar_error(p);

        return 1;
    }
}

/* get_line: recibe una linea y la pone en s,
    con el maximo de caracteres dado por lim.*/
void get_line(char s[], int lim)
{
    int c, i;

    for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
        s[i] = c;
    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return;
}

//****HILO CREADOR : Crea los espacios en el arreglo


