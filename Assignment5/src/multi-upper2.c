#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <upper.h>

#define NB_THREAD 3

int count, file_nb;
pthread_mutex_t mutex_end = PTHREAD_MUTEX_INITIALIZER;


void * thread_control(void * arg) {
    
    int flag = 1;
    char ** x = arg;

    while (flag){
        pthread_mutex_lock (& mutex_end);
        if (count < file_nb){
            upper_init(x[count + 1]);
            // printf("tid = %ld\n", (long)pthread_self());
            count ++;
        } else {
            flag = 0;
        }
        pthread_mutex_unlock (& mutex_end);
        sched_yield();
    }

    pthread_exit(0);
}


int main (int argc, char ** argv) {
    int i, j;
    pthread_t tid[NB_THREAD];

    count = 0;
    file_nb = argc - 1;

    for (i = 0; i < NB_THREAD; i ++){
        if (pthread_create(&(tid[i]), NULL, thread_control, argv) != 0){
            printf ( "pthread_create \n"); 
            exit (1);
        }
    }

    for (j = 0; j < NB_THREAD; j ++) {
        if (pthread_join (tid[j], NULL) != 0){ 
            printf ( "pthread_join"); 
            exit (1); 
        }
    }

    return EXIT_SUCCESS;
}