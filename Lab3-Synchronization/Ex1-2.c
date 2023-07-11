#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 3



int main () {
    int *sp;
    int fd, i, p, random_val, result;
    
    /* Create segment myshm */
    if ((fd = shm_open ( "myshm", O_RDWR | O_CREAT, 0600)) == -1) {
        perror ( "shm_open");
        exit (1);}
    
    /* Allocate enough memory to store an integer */
    if (ftruncate (fd, N * sizeof(int)) == -1) {
        perror("ftruncate");
        exit (1);}
    

    for (i = 0; i < N; i ++){
        p = fork();
        if (p == 0){
            if ((sp = mmap (NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, i * sizeof(int))) == MAP_FAILED) {
                perror("child mmap");
                exit (1);}
            
            srand(getpid());
            random_val = rand()%10;
            *sp = random_val;
            printf("child process %d with random value %d\n", i+1, random_val);

            munmap(sp, sizeof(int));
            exit(0);
        }
    }

    for (i = 0; i < N; i ++){
        wait(NULL);
    }

    /* Map the segment in shared RW mode */
    if ((sp = mmap (NULL, N * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror ( "mmap");
        exit (1);}

    result = 0;
    for (i = 0; i < N; i ++){
        result += *sp;
        sp += sizeof(int);
    }
    printf("sum of N values is %d\n", result);
    
    /* "Detach" segment */
    munmap(sp-N*sizeof(int), N * sizeof(int));

    /* Destroy * the segment*/
    shm_unlink("myshm");
    return 0;
}