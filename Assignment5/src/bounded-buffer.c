#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define NP 3
#define NC 2
#define MAX_CAP 8
#define NVAL 20


/* Initialize Global Variable */
int arr[MAX_CAP] = {};
int arr_index = 0; /* always points to the top empty cell in the array */
int n_consumed = 0;
pthread_mutex_t mutex_end = PTHREAD_MUTEX_INITIALIZER;


/* Secondary Thread Implementation for Producers */
void * thread_producer(void * arg) {

    int rand_val;
    int flag = 1;

    while (flag){
        pthread_mutex_lock (& mutex_end);
        if (n_consumed == NVAL){
            flag = 0;
        } else {
            rand_val = rand()%100 + 1;
            if (arr_index < MAX_CAP){
                arr[arr_index] = rand_val;
                printf("Produce random value %d at array index %d\n", rand_val, arr_index);
                // printf("test p tid = %ld\n\n", (long)pthread_self());
                arr_index ++;
            }
        }    
        pthread_mutex_unlock (& mutex_end);
        sched_yield();
    }
    
    pthread_exit(0);
}


/* Secondary Thread Implementation for Consumers */
void * thread_consumer(void * arg) {

    int flag = 1;

    while (flag){
        pthread_mutex_lock (& mutex_end);
        if (n_consumed == NVAL){
            flag = 0;
        } else {
            if (arr_index <= MAX_CAP && arr_index != 0){
                arr_index --;
                printf("Value Display: %d\n", arr[arr_index]);
                // printf("test c tid = %ld\n\n", (long)pthread_self());
                arr[arr_index] = 0;
                n_consumed ++;
            }
        }    
        pthread_mutex_unlock (& mutex_end);
        sched_yield();
    }

    pthread_exit(0);
}


/* Implementation for Main Thread  */
int main (int argc, char ** argv) {
    int i, j, k;
    pthread_t tid[NP+NC];

    srand(pthread_self());

    printf("PROGRESS START\n");

    for (i = 0; i < NP; i ++){
        if (pthread_create(&(tid[i]), NULL, thread_producer, NULL) != 0){
            printf ( "pthread_create_producer \n"); 
            exit (1);
        }
    }

    for (j = NP; j < NP + NC; j ++){
        if (pthread_create(&(tid[j]), NULL, thread_consumer, NULL) != 0){
            printf ( "pthread_create_consumer \n"); 
            exit (1);
        }
    }

    for (k = 0; k < NP + NC; k ++) {
        if (pthread_join (tid[k], NULL) != 0){ 
            printf ( "pthread_join"); 
            exit (1); 
        }
    }

    printf("PROGRESS END\n");

    return EXIT_SUCCESS;
}