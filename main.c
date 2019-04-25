/* ###########################################################
           PROGRAMAÇÃO PARALELA E CONCORRENTE - UFLA 
                           2019/1 
                    (Crivo de Erstótenes)

    Integrantes:
        - Gabriel Marques de Melo
   ###########################################################  */
 
 
#include <pthread.h>                                 // to use pthread API   
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>                                 // to use bool type
#include <math.h>                                    // to use sqrt()

#define DEBUG 0                                      // enables debug printf()

#if DEBUG
  #define SPAM(a) printf a
#else
  #define SPAM(a) (void)0
#endif


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


void *_thread(void *argv){
    Arg *a = (Arg*) argv;
    int base = a->actual;
    int i = base + 1;

    SPAM(("Thread id: %li\n", pthread_self()));
    while (i <= a->limit){
        if(is_multiple(i, base) && (!a->shared->number[i].checked)){
            SPAM(("%i is multiple of %i\n", i, base));
            a->shared->number[i].checked = 1; 
        }
        i++;
    }
    pthread_exit(NULL);
}


int main(){
    int n;
    int base = 2;

    printf("Type the n (the last number to verify):\n");
    scanf("%i", &n);

    number_t *list = malloc(sizeof(number_t)*(n+1));

    for(int j = 0; j <= n; j++)
        list[j] = default_number;

    shared_t *shared = malloc(sizeof(shared_t));
    shared->number = list;

    const int MAX_NUMBER_THREADS = sqrt(n);
    SPAM(("\nThreads count: %i\n",  MAX_NUMBER_THREADS));
    SPAM(("--------------\n"));
 
    pthread_t threads[MAX_NUMBER_THREADS];

    Arg a[MAX_NUMBER_THREADS];

    for(int j = 0; j < MAX_NUMBER_THREADS; j++)
        a[j].shared = shared;
    
    int i = 3;
    int thread_counter = 0;


    while (i <= n){
        if(!is_multiple(i, base)){
            if(i <= MAX_NUMBER_THREADS){
                a[thread_counter].limit = n;
                a[thread_counter].actual = i;
                int return_code = pthread_create(&threads[thread_counter], NULL, _thread, &a[thread_counter]);
                if (return_code){
                    printf("ERROR; return code from pthread_create() is: %i\n", return_code);
                    exit(-1);
                }
                thread_counter++;
            }
        }
        else{
            if(!shared->number[i].checked){
                shared->number[i].checked = 1; 
                    SPAM(("%i is multiple of %i\n", i, base));
            }
        }
        i++;
    }

    for(int j = 0; j < thread_counter; j++)
        pthread_join(threads[j], NULL);
    
    printf("\nPrime numbers:\n");
    printf("--------------\n");
    
    for(int i = 2; i <= n; i++)
        if(!shared->number[i].checked)
            printf("| %i ", i);
    printf("|\n");    

    pthread_exit(NULL);
}
