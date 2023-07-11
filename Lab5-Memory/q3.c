#define _XOPEN_SOURCE 700
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/stat.h> 
#include <stdlib.h>

#define N 3
#define RMAX 10
#define BUFSZ 100
char buffer[BUFSZ];

int main(int argc, char* argv[]){
    int i, j, n, total_val;
    int fd[N+1];

    if ((fd[0] = open(argv[1], O_CREAT | O_RDONLY)) == -1){
        printf ( "%s parent open \n", argv[1]); 
        return EXIT_FAILURE; 
    }

    for (i = 1; i < N+1; i ++){
        if (fork() == 0){

            if ((fd[i] = open(argv[1], O_WRONLY | O_APPEND)) == -1){ 
                printf( "child %d open \n", i); 
                return EXIT_FAILURE; 
            }

            int random_val;
            srand(getpid());
            random_val = rand() % RMAX;
            printf("random number %d is %d \n", i, random_val);
            char num[1];
            num[1] = random_val;

            if (write(fd[i], num, sizeof(num)) == -1){ 
                printf( "write number %d \n", i); 
                return EXIT_FAILURE; 
            }

            exit(0);
        }
    }

    for (j = 0; j < N; j ++){
        wait(NULL);
    }

    if (lseek(fd[0],0, SEEK_SET) == -1){ 
        perror ( "seek"); 
        return EXIT_FAILURE; 
    }

    if ((n = read(fd[0], buffer, BUFSZ)) <= 0){
        printf( "parent read \n"); 
        return EXIT_FAILURE;
    }

    printf("n = %d\n", n);

    total_val = 0;
    for (j = 0; j < n; j ++){
        printf("number %c \n", buffer[j]);
    }
    total_val += (int)buffer[j];
    printf("total value = %d\n", total_val);

    return EXIT_SUCCESS;
}