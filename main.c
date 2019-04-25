/* ###########################################################
           PROGRAMAÇÃO PARALELA E CONCORRENTE - UFLA 
                           2019/1 
                    (Crivo de Erstótenes)

    Integrantes:
        - Gabriel Marques de Melo
   ###########################################################  */
 
// Consulta: https://computing.llnl.gov/tutorials/pthreads/#PthreadsAPI
 
#include <pthread.h>                                 // to use pthread API   
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>                                 // to use bool type
#include <math.h>                                    // to use sqrt()
#include <time.h>                                    // to implement delay() function

// TODO: remove unnecessary printfs

typedef struct {
    bool checked;
} number_t;


typedef struct {
    number_t *number;
    int limit;
    int actual;
} Arg;

bool is_multiple(int actual, int base){
    return actual % base == 0;
}

void delay(int microseconds){
    long pause;
    clock_t now,then;

    pause = microseconds*(CLOCKS_PER_SEC/1000000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

void *_thread(void *argv){
    Arg *a = (Arg*) argv;

    printf("PORRA %i\n", a->actual);
    int base = a->actual;

    printf("O atual é igual a: %i\n", a->actual);
    int i = base + 1;
    printf("Valor de K: %i\n", base);
    printf("Thread id: %li\n", pthread_self());
    while (i <= a->limit){
        if(is_multiple(i, base)){
            printf("%i é multiplo de %i\n", i, base);
            a->number[i].checked = 1; 
        }
        i++;
    }
    pthread_exit(NULL);
}

int main(){
    int n;
    int base = 2;

    number_t *list = (number_t*) malloc(sizeof(number_t)*n);
    
    printf("Type the n (the last number to verify):\n");
    scanf("%i", &n);

    for(int i = 0; i < n; i++){
        list[i].checked = false;
    }

    const int NUMBER_THREADS = sqrt(n);
    printf("Threads count: %i\n",  NUMBER_THREADS);
    pthread_t threads[NUMBER_THREADS];
    int last_thread;

    Arg *a = (Arg*) malloc(sizeof(Arg));
    a->number = list;
    a->limit = n;
    int i = 3;

    while (i <= n){
        if(!is_multiple(i, base)){
            if(i <= NUMBER_THREADS){
                a->actual = i;
                last_thread = i-3;
                int rc = pthread_create(&threads[i-3], NULL, _thread, a);
                delay(100);  // 150 microseconds to make sure that thread received the properly a->actual value
                if (rc){
                    printf("ERROR; return code from pthread_create() is: %i\n", rc);
                    exit(-1);
                }
            }
        }
        else{
            printf("%i é multiplo de %i\n", i, base);
            a->number[i].checked = true; 
        }

        i++;
    }

    // TODO: ADD a call to pthread_join(XXX,NULL);
    pthread_join(threads[last_thread], NULL);
    
    printf("Prime numbers:\n");
    for(int i = 2; i <= n; i++){
        if(!list[i].checked)
            printf("| %i ", i);
    }
        
    pthread_exit(NULL);
}
