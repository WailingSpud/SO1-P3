#include <unistd.h> 
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void signalHandler(int signal);
void signalHandler2(int signal);

volatile double numero = 1;

int main(void){
    
    signal(SIGALRM,signalHandler);
    signal(SIGUSR1,signalHandler2);
    
    
    while( 1 ){
        
        numero += pow(numero,0.5);
        alarm(1);
        printf("Mi sucesión: %lf\n",numero);
        printf("Cuando desee terminar la sucesión haga kill -USR1 y el pid %d\n",getpid());
        sleep(1);
        
    }

    return 0;
}


void signalHandler(int signal){
    printf("\n[Señal %d recibida]. Número actual: %lf\n", signal,numero);
}

void signalHandler2(int signal){
    printf("El programa se va a terminal con la señal %d\n",signal);
}