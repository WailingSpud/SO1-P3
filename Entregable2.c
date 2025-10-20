#define _POSIX_C_SOURCE 200809L
#include <unistd.h> 
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void signalHandler(int señal);
int main(void){

    struct sigaction act;
    int status0, status1;
    sigaction(SIGUSR1,act.sa_handler=signalHandler,NULL);
    sigaction(SIGUSR2,act.sa_handler=signalHandler,NULL);
    
    printf("Soy el padre y mi pid es %d\n", getpid());

    pid_t hijo1 = fork();

    if(hijo1 == 0){
        printf("Soy el hijo 1 y voy a crear otro hijo, mi pid es: %d\n", getpid());
        pid_t hijo2 = fork();
        if(hijo2 == 0){
            printf("Soy el proceso nieto y mi pid es: %d", getpid());
            exit(56);
        }
        else{
            printf("No se pudo crear el proceso nieto\n");
            return;
        }
        exit(42);
    }
    else{

        printf("El hijo 1 no se pudo crear\n");
        return;

    }
    wait();

    pid_t pidhijo = wait(&status0);
        printf("El proceso hijo %d ya terminó con código %d\n",pidhijo,WEXITSTATUS(status0));
        
    return 0;
}

void signalHandler( int señal ){
    printf("Mi numero de señal es: %d\n", señal);
    
}