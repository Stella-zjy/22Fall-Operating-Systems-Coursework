#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3

pthread_mutex_t mutex_end = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_end = PTHREAD_COND_INITIALIZER;
int x = 0;
int count = 0;


void * rand_thread(void * arg) {

    int random_val;
    random_val = rand()%10;
    printf ("Creation Order: %d, thread_id: %ld, random value: %d\n", *(int*)arg, (long) pthread_self(), random_val);
  
    pthread_mutex_lock (& mutex_end);
    x = x + random_val;
    count ++;
    if (count == NUM_THREADS){
        pthread_cond_signal (& cond_end);
    }
    pthread_mutex_unlock (& mutex_end);

    free(arg);
    pthread_exit(0);
}


void * print_thread(void * arg) {
    pthread_cond_wait (& cond_end, & mutex_end);
    printf("Summation: %d\n", x);
    pthread_mutex_unlock (& mutex_end);
    pthread_exit(0);
}


int main (int argc, char ** argv) {
    int i; 
    pthread_t tid[NUM_THREADS+1];
    int *pt_ind;

    srand(pthread_self());   

    pthread_mutex_lock (& mutex_end);

    for (i = 0; i < NUM_THREADS; i ++) {
        pt_ind = (int*) malloc(sizeof(int));
        *pt_ind = i;
        if (pthread_create(&(tid[i]), NULL, rand_thread, (void*)pt_ind) != 0){
            printf ( "pthread_create \n"); 
            exit (1);
        }
        pthread_detach(tid[i]);
    }

    if (pthread_create(&(tid[NUM_THREADS]), NULL, print_thread, NULL) != 0){
        printf ( "pthread_create2 \n"); 
        exit (1);
    }

    if (pthread_join(tid[NUM_THREADS], NULL) != 0){ 
        printf ( "pthread_join"); 
        exit (1); 
    }

    printf("(main) Finished\n");

    return EXIT_SUCCESS;
}

/* Note that if all secondary threads are detached, then when main thread terminates, the process terminates.*/
/* A pthread_join() or cond_wait */