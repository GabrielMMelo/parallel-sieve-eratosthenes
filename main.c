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
} shared_t;

typedef struct {
    int limit;
    int actual;
    shared_t* shared;
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

    int base = a->actual;

    printf("O atual é igual a: %i\n", a->actual);
    int i = base + 1;
    printf("Valor de K: %i\n", base);
    printf("Thread id: %li\n", pthread_self());
    while (i <= a->limit){
//        if(is_multiple(i, base)){
        if(is_multiple(i, base) && (!a->shared->number[i].checked)){
            printf("%i é multiplo de %i\n", i, base);
            a->shared->number[i].checked = 1; 
        }
        i++;
    }
    pthread_exit(NULL);
}

int main(){
    int n;
    int base = 2;

    number_t *list = malloc(sizeof(number_t)*n);
    
    printf("Type the n (the last number to verify):\n");
    scanf("%i", &n);

    for(int i = 0; i < n; i++){
        list[i].checked = false;
    }

    const int NUMBER_THREADS = sqrt(n);
    printf("Threads count: %i\n",  NUMBER_THREADS);
    pthread_t threads[NUMBER_THREADS];

    shared_t *shared = malloc(sizeof(shared_t));
    shared->number = list;
    int i = 3;
    int thread_counter = 0;
    Arg a[NUMBER_THREADS];
    for(int j = 0; j < NUMBER_THREADS; j++){
        a[j].shared = shared;
    }

    while (i <= n){
        if(!is_multiple(i, base)){
            if(i <= NUMBER_THREADS){
                a[thread_counter].limit = n;
                a[thread_counter].actual = i;
                int rc = pthread_create(&threads[thread_counter], NULL, _thread, &a[thread_counter]);
                if (rc){
                    printf("ERROR; return code from pthread_create() is: %i\n", rc);
                    exit(-1);
                }
                thread_counter++;
            }
        }
        else{
            if(!shared->number[i].checked){
                shared->number[i].checked = 1; 
                printf("%i é multiplo de %i\n", i, base);
            }
        }

        i++;
    }

    for(int j = 0; j < thread_counter; j++){
        pthread_join(threads[j], NULL);
    }
    
    for(int j = 2; j <= n; j++){
        if(shared->number[j].checked == 0)
            printf("%i:primo\n", j);
        else
            printf("%i\n", j);
    }

    /*
    printf("Prime numbers:\n");
    for(int i = 2; i <= n; i++){
        if(!shared->number[i].checked)
            printf("| %i ", i);
    }
    */
        
    pthread_exit(NULL);
}
