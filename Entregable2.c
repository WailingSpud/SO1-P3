//Programa hecho por Sebastián Montoto Peña y Manuel Pérez Cancela

#define _POSIX_C_SOURCE 200809L
#include <unistd.h> 
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

void manejador(int sig);
char *tiempo_local();


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

    printf("[%s] Soy el padre y mi pid es %d\n",tiempo_local() , proceso_padre);

    pid_t hijo = fork();

    if(hijo == 0){ //Proceso hijo
        printf("[%s] Soy el hijo y voy a crear otro hijo, mi pid es: %d\n",tiempo_local() ,getpid());

        pid_t nieto = fork();

        if(nieto == 0){ //Proceso nieto
            printf("[%s] Soy el proceso nieto y mi pid es: %d\n",tiempo_local() , getpid());
            sleep(2); //Nieto se duerme para asegurarse de que SIGUSR1 llega primero
            kill(proceso_padre,SIGUSR2); //Envía señal SIGUSR2 al abuelo
            printf("[%s] El proceso nieto envía SIGUSR2 a %d\n",tiempo_local() , proceso_padre);
            sleep(5); 
            exit(22);
        }
        else{ //Proceso hijo
            kill(proceso_padre, SIGUSR1); //Envía señal SIGUSR1 al abuelo
            printf("[%s] El proceso hijo envía SIGUSR1 a %d\n",tiempo_local() , getppid());
            pid_t nietoid = wait(&status1);
            printf("[%s] El proceso nieto %d ya terminó con código %d\n",tiempo_local() ,nietoid,WEXITSTATUS(status1));
            exit(42);
        }
    }
    //Proceso padre

    sleep(9); //Espera para asegurarse de que el nieto envíe su señal también    
    pid_t pidhijo = wait(&status0); //Espera al hijo

    printf("[%s] El proceso hijo %d ya terminó con código %d\n",tiempo_local() ,pidhijo,WEXITSTATUS(status0));

    sigemptyset(&pending); //Inicializa pending
    sigpending(&pending);
    if(sigismember(&pending, SIGUSR1)){
        printf("[%s] La señal SIGUSR1 está pendiente en el proceso padre %d\n",tiempo_local() , proceso_padre);
    }

    //Desbloqueo de SIGUSR1
    printf("[%s] SIGUSR1 va a ser desbloqueado\n",tiempo_local());
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
       
    return 0;
}

void manejador(int sig) {
    if (sig == SIGUSR1)
        printf("[%s] [HANDLER]: Proceso %d recibió SIGUSR1\n",tiempo_local() , getpid());
    else if (sig == SIGUSR2)
        printf("[%s] [HANDLER]: Proceso %d recibió SIGUSR2\n",tiempo_local() , getpid());
}

char *tiempo_local(){
    static char hora[9];  // HH:MM:SS + '\0'
    time_t ahora = time(NULL);
    struct tm *tlocal = localtime(&ahora);

    strftime(hora, sizeof(hora), "%H:%M:%S", tlocal); //Guarda en hora una string con los campos de hora, minutos y segundos de tlocal (tm)
    return hora;
}