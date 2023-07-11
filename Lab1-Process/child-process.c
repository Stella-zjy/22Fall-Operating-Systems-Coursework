#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
    }
    // exit(0);
}

int child_recur(int N_CHILDREN){
    if (N_CHILDREN==0){
        exit(0);
    }else{
        pid_t pid;
        pid = fork();
        if (pid==0){
            printf("create child with pid %d and ppid %d\n", getpid(), getppid());
            exit(0);
        }else{
            wait(&pid); // what we're not supposed to do --> Put the wait() outside the recursion
            child_recur(N_CHILDREN - 1);
        }
    }
}


// Rrcursion
// void cr

int main(){
    int N_CHILDREN = 3;
    // printf("Iterative Method\n");
    // child_iter(N_CHILDREN);
    printf("Recursive Method\n");
    child_recur(N_CHILDREN);
    return 0;
}

