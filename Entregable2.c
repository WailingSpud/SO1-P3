#define _POSIX_C_SOURCE 200809L
#include <unistd.h> 
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

void manejador(int sig);

int main(void){

    struct sigaction act;
    sigset_t mask, pending;

    act.sa_handler = manejador;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    
    int status0, status1;
    
    sigset_t mask, pending;
    
    pid_t proceso_padre =  getpid();

    printf("Soy el padre y mi pid es %d\n", proceso_padre);

    pid_t hijo1 = fork();

    if(hijo1 == 0){
        printf("Soy el hijo 1 y voy a crear otro hijo, mi pid es: %d\n", getpid());

        pid_t hijo2 = fork();

        if(hijo2 == 0){
            printf("Soy el proceso nieto y mi pid es: %d\n", getpid());
            kill(proceso_padre,SIGUSR2);
            sleep(5);
            exit(22);
        }
        
        kill(proceso_padre, SIGUSR1);
        wait(&status1);
        exit(42);

        }
        
    pid_t pidhijo = wait(&status0);

    printf("El proceso hijo %d ya terminó con código %d\n",pidhijo,WEXITSTATUS(status0));
    
        
    return 0;
}

void manejador(int sig) {
    if (sig == SIGUSR1)
        printf("Proceso %d recibió SIGUSR1\n", getpid());
    else if (sig == SIGUSR2)
        printf("Proceso %d recibió SIGUSR2\n", getpid());
}