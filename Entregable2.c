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

    //Se crea el manejador de señales
    act.sa_handler = manejador;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);

    //Bloqueo de SIGUSR1 antes de que la envíe el hijo
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    int status0, status1;
        
    pid_t proceso_padre =  getpid();

    printf("Soy el padre y mi pid es %d\n", proceso_padre);

    pid_t hijo = fork();

    if(hijo == 0){ //Proceso hijo
        printf("Soy el hijo y voy a crear otro hijo, mi pid es: %d\n", getpid());

        pid_t nieto = fork();

        if(nieto == 0){ //Proceso nieto
            printf("Soy el proceso nieto y mi pid es: %d\n", getpid());
            kill(proceso_padre,SIGUSR2); //Envía señal SIGUSR2 al abuelo
            printf("El proceso nieto envía SIGUSR2 a %d\n", getppid());
            sleep(5); //Espera antes de enviar la señal para asegurar que el hijo envía SIGUSR1 primero
            exit(22);
        }
        else{ //Proceso hijo
            kill(proceso_padre, SIGUSR1); //Envía señal SIGUSR1 al abuelo
            printf("El proceso hijo envía SIGUSR1 a %d\n", getppid());
            pid_t nietoid = wait(&status1);
            printf("El proceso nieto %d ya terminó con código %d\n",nietoid,WEXITSTATUS(status1));
            exit(42);
        }
    }
    //Proceso padre

    sleep(7); //Espera para asegurarse de que el nieto envíe su señal también    
    pid_t pidhijo = wait(&status0); //Espera al hijo

    printf("El proceso hijo %d ya terminó con código %d\n",pidhijo,WEXITSTATUS(status0));

    sigemptyset(&pending); //Inicializa pending
    sigpending(&pending);
    if(sigismember(&pending, SIGUSR1)){
        printf("La señal SIGUSR1 está pendiente en el proceso padre %d\n", proceso_padre);
    }

    //Desbloqueo de SIGUSR1
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
       
    return 0;
}

void manejador(int sig) {
    if (sig == SIGUSR1)
        printf("[HANDLER]: Proceso %d recibió SIGUSR1\n", getpid());
    else if (sig == SIGUSR2)
        printf("[HANDLER]: Proceso %d recibió SIGUSR2\n", getpid());
}