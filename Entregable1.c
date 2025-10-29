//Programa hecho por Sebastián Montoto Peña y Manuel Pérez Cancela

#include <unistd.h> 
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void alarmaValor(int signal);
void signalHandler(int signal);

double numero = 1;

int main(void){
    
    signal(SIGALRM,alarmaValor); //Alarma
    signal(SIGUSR1,signalHandler);
    alarm(1); //Primera alarma después de 1 segundo
    
    while( 1 ){
        numero += pow(numero,0.5);
        pause(); //Para hasta recibir una señal que ejecute el signalHandler o termine el programa      
    }

    return 0;
}


void alarmaValor(int signal){
    printf("\n[Señal %d recibida]. Número actual: %lf\n", signal,numero);
    printf("Cuando desee terminar la sucesión haga kill -USR1 y el pid %d\n",getpid());
    alarm(1); //Programa la siguiente alarma
}

void signalHandler(int signal){
    printf("El programa se va a terminar con la señal %d\n",signal);
    exit (EXIT_SUCCESS);
}