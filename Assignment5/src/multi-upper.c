#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <upper.h>


void * thread_control(void * arg) {

    upper_init((char*)arg);
    
    pthread_exit(0);
}


int main (int argc, char ** argv) {
    int i, j;
    pthread_t tid[argc - 1];

    for (i = 1; i < argc; i ++){
        if (pthread_create(&(tid[i - 1]), NULL, thread_control, argv[i]) != 0){
            printf ( "pthread_create \n"); 
            exit (1);
        }
    }

    for (j = 1; j < argc; j ++) {
        if (pthread_join (tid[j-1], NULL) != 0){ 
            printf ( "pthread_join"); 
            exit (1); 
        }
    }

    return EXIT_SUCCESS;

}