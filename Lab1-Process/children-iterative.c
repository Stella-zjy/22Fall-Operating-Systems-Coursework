#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Q2.3: wait for the termination of all children
int child_iter(int N_CHILDREN){
    int i,j;
    for (i = 0;i < N_CHILDREN; i++){
        if(fork()==0){
            printf("create child %d with pid %d and ppid %d\n", i+1, getpid(),getppid());
            exit(0);
        }
    }
    for (j = 0;j < N_CHILDREN; j++){
        wait(NULL);
        j++;
    }
    exit(0);
}



int main(){
    int N_CHILDREN = 3;
    printf("Iterative Method\n");
    child_iter(N_CHILDREN);
    return 0;
}