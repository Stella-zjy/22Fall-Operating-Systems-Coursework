#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


int concurrent_compilation(char arr[][20], int len){
    int i, j, k;
    for (i = 0; i < len; i++){
        if (fork()==0){
            char *arg_list[] = {"gcc", "-c", arr[i], NULL};
            execvp(arg_list[0], arg_list);
            perror("execvp - 1");
            exit(0);
        }
    }
    for (j = 0; j < len; j++){
        wait(NULL);
    }
    for (k = 0; k < len; k++){
        char fileExe[20], fileObj[20];
        char *ptr = strstr(arr[k], ".c");
        strncpy(fileExe, arr[k], ptr - arr[k]);  //filename = tata/titi/toto
        strncpy(fileObj, arr[k], ptr - arr[k]); 
        strcat(fileObj, ".o");
        printf("%s\n%s\n",fileExe, fileObj);
        char *arg_list2[] = {"gcc", "-o", fileExe, fileObj, NULL}; 
        execvp(arg_list2[0], arg_list2);
        perror("execvp - 2");
    }
    return 0;
}

int main(int argc, char** argv){
    char arr[3][20] = {"tata.c","titi.c","toto.c"};
    concurrent_compilation(arr, 3);
    return 0;
}