/* ###########################################################
           PROGRAMAÇÃO PARALELA E CONCORRENTE - UFLA 
                    (Crivo de Erstótenes)

    Integrantes:
        - Gabriel Marques de Melo
   ###########################################################  */
 
// Consulta: https://computing.llnl.gov/tutorials/pthreads/#PthreadsAPI
//
// Criar uma lista de números naturais 2, 3, 4,..., n não marcados
// 2.  Atribuir k=2, o primeiro número não marcado na lista
// 3.  Repita
// (a)Marque todos os múltiplo de k entre k² e n
// (b) Encontre o menor número maior do que k que está não marcado. 
// Atribua a k este novo valor.
// Até k² > n
// 4.  Os números não marcados são primos
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


typedef struct {
    bool marcado;
} numero_t;


typedef struct {
    int *k;             // TODO: k não é necessario aqui!
    numero_t *numero;
    int limit;
    int atual;
} Arg;

bool is_multiplo(int numero, int k){
    if(numero % k == 0)
        return true;
    return false;
}

void *thread(void *argv){
    Arg *a = (Arg*) argv;

    int k = a->atual;

    printf("O atual é igual a: %i\n", a->atual);
    int i = k + 1;
    printf("Valor de K: %i\n", k);
    printf("Thread id: %li\n", pthread_self());
    while (i <= a->limit){
        if(is_multiplo(i, k)){
            printf("%i é multiplo de %i\n", i, k);
            a->numero[i].marcado = 1; 
        }
        i++;
    }
    pthread_exit(NULL);
}

int main(){
    int n;
    int *k = (int*) malloc(sizeof(int));
    *k = 2;

    // Declara a list de numeros
    numero_t *list = (numero_t*) malloc(sizeof(numero_t)*n);
    
    printf("Digite o n:\n");
    scanf("%i", &n);

    for(int i = 0; i < n; i++){
        list[i].marcado = false;
    }

    /*
    for(int i = 2; i < n; i++){
        printf("%i\n", list[i].numero);
        printf("%i\n", list[i].marcado);
    }
    */

    const int NUM_THREADS = sqrt(n);
    printf("Número de threads: %i\n",  NUM_THREADS);
    pthread_t threads[NUM_THREADS];

    Arg *a = (Arg*) malloc(sizeof(Arg));
    a->k = k;
    a->numero = list;
    a->limit = n;
    int i = 3;

    while (i <= n){
        if(!is_multiplo(i, (*a->k))){
            
            if(i <= NUM_THREADS+1){
                a->atual = i;
                int rc = pthread_create(&threads[i-3], NULL, thread, a);
                if (rc){
                    printf("ERROR; return code from pthread_create() is: %i\n", rc);
                    exit(-1);
                }
            }
        }
        else{
            printf("%i é multiplo de %i\n", i, *a->k);
            a->numero[i].marcado = true; 
        }

        printf("%i\n", a->atual);
        i++;
    }

    for(int i = 2; i < n; i++){
        printf("%i:%i\n", i, list[i].marcado);
    }
        
    pthread_exit(NULL);
}
