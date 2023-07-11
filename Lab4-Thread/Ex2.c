#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3

pthread_mutex_t mutex_end = PTHREAD_MUTEX_INITIALIZER;
int x=0;

void * rand_thread(void * arg) {

    int random_val;
    random_val = rand()%10;
    printf ("Creation Order:%d, thread_id:%ld, random value %d\n", *(int*)arg, (long) pthread_self(), random_val);
  
    pthread_mutex_lock (& mutex_end);
    x = x + random_val;
    pthread_mutex_unlock (& mutex_end);

    free(arg);

    pthread_exit(0);
}



int main (int argc, char ** argv) {
    int i,j; pthread_t tid[NUM_THREADS];
    int *pt_ind;   

    for (i = 0; i < NUM_THREADS; i ++) {
        pt_ind = (int*) malloc(sizeof(int));
        *pt_ind = i;
        if (pthread_create(&(tid[i]), NULL, rand_thread, (void*)pt_ind) != 0){
            printf ( "pthread_create \n"); 
            exit (1);
        }
    }

    for (j = 0; j < NUM_THREADS; j ++) {
        if (pthread_join (tid[j], NULL) != 0){ 
            printf ( "pthread_join"); 
            exit (1); 
        }
        // else{
        //     printf ( "Thread finished with status:%d \n", status);
        // }
    }

    printf("Summation:%d\n", x);

    return EXIT_SUCCESS;
}
