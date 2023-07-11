#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
            wait(&pid);
            child_recur(N_CHILDREN - 1);
        }
    }
}

int main(){
    int N_CHILDREN = 3;
    printf("Recursive Method\n");
    child_recur(N_CHILDREN);
    return 0;
}