#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3


void * thread_control(void * arg) {

    int * pt = malloc(sizeof(int)); 
    printf ("Creation Order:%d, thread_id:%d \n", *(int*)arg, (int) pthread_self());
    *pt = *(int*)arg * 2;
    free(arg);
    pthread_exit((void *) (intptr_t) *pt);
    pthread_exit(0);
}



int main (int argc, char ** argv) {
    int i,j, status; pthread_t tid[NUM_THREADS];
    int *pt_ind;

    

    for (i = 0; i < NUM_THREADS; i ++) {
        pt_ind = (int*) malloc(sizeof(int));
        *pt_ind = i;
        if (pthread_create(&(tid[i]), NULL, thread_control, (void*)pt_ind) != 0){
            printf ( "pthread_create \n"); 
            exit (1);
        }
    }

    for (j = 0; j < NUM_THREADS; j ++) {
        if (pthread_join (tid[j], (void **) &status) != 0){ 
            printf ( "pthread_join"); 
            exit (1); 
        }
        else{
            printf ( "Thread finished with status:%d \n", status);
        }
    }

    return EXIT_SUCCESS;
}
