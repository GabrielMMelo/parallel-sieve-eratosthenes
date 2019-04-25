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

struct number {
    bool checked;
} default_number = {false};

typedef struct number number_t;

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
//            printf("%i é multiplo de %i\n", i, base);
            a->shared->number[i].checked = 1; 
        }
        i++;
    }
    pthread_exit(NULL);
}

int main(){
    int n;
    int base = 2;

    number_t *list = malloc(sizeof(number_t)*(n+30));  // TODO: CONFERIR MALLOC. GAMBIARRA PARA RESOLVER.
    printf("Type the n (the last number to verify):\n");
    scanf("%i", &n);

    const int MAX_NUMBER_THREADS = sqrt(n);
    printf("Threads count: %i\n",  MAX_NUMBER_THREADS);
    pthread_t threads[MAX_NUMBER_THREADS];

    shared_t *shared = malloc(sizeof(shared_t));
    shared->number = list;
    int i = 3;
    int thread_counter = 0;
    Arg a[MAX_NUMBER_THREADS];
    for(int j = 0; j < MAX_NUMBER_THREADS; j++){
        a[j].shared = shared;
    }

    while (i <= n){
        if(!is_multiple(i, base)){
            if(i <= MAX_NUMBER_THREADS){
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
//                printf("%i é multiplo de %i\n", i, base);
            }
        }

        i++;
    }

    for(int j = 0; j < thread_counter; j++){
        pthread_join(threads[j], NULL);
    }
    
    for(int j = 2; j <= n; j++){
        if(shared->number[j].checked == 0)
            printf("%i:%d\n", j, shared->number[j].checked);
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
