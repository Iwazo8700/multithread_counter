/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define WORKER 4
#define ATIVO 1
#define INATIVO 0

/*tipo pro thread ter a info do numero e o id*/
typedef struct{
    int N;
    int ID;
    } thread_args;

/*Variaveis Globais*/
pthread_mutex_t trava;//mutex
int resultado = 0;//variavel para o resultado final
pthread_t workers[WORKER];//threads
int worker_status[WORKER];//status das threads(ativo ou inativo)
int n_workers = 0;//numero de threads executando no momento

int primo(int n){
    /*funcao que verifica se um numero eh primo ou nao
      funcao nao esta otimizada, portanto ele faz o calculo
      verifica a divisao inteira de todos os numeros inteiros
       menores que n e maiores que 0.*/

    if(n < 2) return 0;
    for(int i=2; i<n; i++){
        if(n % i == 0) return 0;
    }
 
    return 1;
    }


void* worker(void *arg){
    /*thread
    pego a parametro, dou o typecast pra thread_args e calculo o primo do valor N do thread_args*/
    
    thread_args *work = (thread_args*)(arg);
    resultado+=primo(work->N);
    pthread_mutex_lock(&trava);
    n_workers--;
    worker_status[work->ID] = INATIVO;
    free(work);
    pthread_mutex_unlock(&trava);
    return NULL;
    }


int main() {
    
    int numero;
    char c;
    thread_args *send_args;
    int j;
    int ordem = 0;
    while(scanf("%d%c", &numero, &c)){
        
        if(n_workers >= WORKER) pthread_join(workers[ordem%4], NULL);
        pthread_mutex_lock(&trava);
        send_args = (thread_args*)malloc(sizeof(thread_args));
        send_args->N = numero;

        j = 0;
        while(worker_status[j] == ATIVO) j++;
        send_args->ID = j;
        worker_status[j] = ATIVO;
        n_workers++;
        pthread_create(&(workers[j]), NULL, worker, (void*)send_args);
        pthread_mutex_unlock(&trava);
        
        
        
        ordem++;
        if(c=='\n') break;
    }

    for(int i=0; i<WORKER; i++){
        pthread_join(workers[i], NULL);
    }

    printf("%d\n", resultado);
    return 0;
}
