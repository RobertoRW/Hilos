#include <stdio.h>
#include <stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<stdlib.h>
#include	<string.h>
#include <stdbool.h>

#include	"msg.h"
#include	"mem.h"


int memid, sem_salida, sem_hilos;
Msg *ptr_msg;

main()
{
	fprintf(stderr, "Obteniendo segmento de memoria compartida.\n");
	if ( (memid=shmget(LLAVE_MEM,sizeof(Msg),0))<0){
		fprintf(stderr, "No se pudo obtener la memoria compartida.\n");
        fprintf(stderr, "¿Ejecutar Hilos Primero?.\n");
        exit(1);
	}
        fprintf(stderr, "Conectando (attach) segmento de memoria compartida.\n");
	if ( (ptr_msg=(Msg *)shmat(memid,(char *)0,0))==(Msg *) -1)
		err_sys("client: can't attach shared memory segment");

    ptr_msg->terminar = false;
    fprintf(stderr, "Localizando semáforos creados.\n");
	if ( (sem_salida=sem_open(LLAVE_SEM_SALIDA))<0)
		err_sys("client: can't open client semaphore");
	if ( (sem_hilos=sem_open(LLAVE_SEM_HILOS))<0)
		err_sys("client: can't open server semaphore");
    printf("============ESCUCHANDO===============\n");


	salida();


    printf("============TERMINANDO===============\n");
    fprintf(stderr, "Desconectando el área de memoria compartida.\n");
	if (shmdt(ptr_msg)<0)
		err_sys("client: can't detach shared memory");


    exit(0);
}

salida(){
    sem_wait(sem_salida);
	while( !(ptr_msg->terminar)  ) {
	    if (ptr_msg->mesg_type == 1) {
	        printf(COLOR_ERR);
	        printf("Error: ");
        } // si es un error, pone el color en rojo.
        printf(ptr_msg->mesg_data); //Imprime el mensaje/error
        printf("\x1B[0m"); //Cambia el color de la salida al normal.
		sem_signal(sem_hilos);	 //Prende el semaforo de los hilos
		sem_wait(sem_salida);	//Espera el de salida.
	}
	printf("Recibido mensaje de cierre.\n");
	sem_signal(sem_hilos);	 //Prende el semaforo de los hilos
}
