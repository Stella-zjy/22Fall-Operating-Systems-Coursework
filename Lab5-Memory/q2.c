#define _XOPEN_SOURCE 700
#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdlib.h>

#define BUFSZ 100
char buffer[BUFSZ];

int main (int argc, char* argv[]) {
    int fd1, fd2;
    int n;

    fd1 = open(argv[1], O_RDWR);
    fd2 = open(argv[2], O_CREAT | O_RDWR);

    // Check existence for file2

    if (fd1 == -1){ 
        printf ( "%s open", argv[1]); 
        return EXIT_FAILURE; 
    }
    if (fd2 == -1){ 
        printf ( "%s open", argv[2]); 
        return EXIT_FAILURE; 
    }

    // May use while loop to check whether the buffer is big enough
    if ((n = read (fd1, buffer, BUFSZ)) <= 0){
        perror ( "read \n"); 
        return EXIT_FAILURE;
    }

    if (write(fd2, buffer, n) == -1){ 
        perror ( "write \n"); 
        return EXIT_FAILURE; 
    }

    return EXIT_SUCCESS;
}