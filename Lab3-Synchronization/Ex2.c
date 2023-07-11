#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>

#define N 3


int main () {
    int *sp;
    int fd, i, p, random_val, result;

    sem_t* smutex;
    sem_t* smutex2;
    /* Do Not name semaphores as smutex unless initialized to 0 (stands for mutual exclusion)*/
    // sem_t* mx;
    // mx = sem_open ( "mx", O_CREAT | O_EXCL | O_RDWR, 0666, 1);

    if ((smutex = sem_open ( "mysem", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open");
            exit(1);
        }
        /* Semaphore already created, open without O_CREAT */
        smutex = sem_open("mysem", O_RDWR);
    }

    if ((smutex2 = sem_open ( "mysem2", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open2");
            exit(1);
        }
        /* Semaphore already created, open without O_CREAT */
        smutex = sem_open("mysem2", O_RDWR);
    }


    /* Create segment myshm */
    if ((fd = shm_open ( "myshm", O_RDWR | O_CREAT, 0600)) == -1) {
        perror ( "shm_open");
        exit(1);}
    
    /* Allocate enough memory to store an integer */
    if (ftruncate (fd, (N+1) * sizeof(int)) == -1) {
        perror("ftruncate");
        exit(1);}
    

    for (i = 0; i < N; i ++){
        p = fork();
        if (p == 0){
            if ((sp = mmap (NULL, (N+1) * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
                perror("child mmap");
                exit(1);}
            
            // sem_wait(mx);
            srand(getpid());
            random_val = rand()%10;
            *(sp + i*sizeof(int)) = random_val;
            printf("child process %d with random value %d\n", i+1, random_val);
            // sem_post(mx);

            // if (i == N-1){
            //     sem_post(smutex2);
            //     sem_close(smutex2);
            //     sem_unlink("mysem2");
            // }
            /* Do not guarantee consistency */

            sem_post(smutex2);
            sem_close(smutex2);
            sem_unlink("mysem2");

            sem_wait(smutex);
            result = *(sp + N*sizeof(int));
            printf("Sum is %d, displayed by child process %d\n", result, i+1);

            sem_close(smutex);
            sem_unlink("mysem");

            munmap(sp, sizeof(int));
            exit(0);


        }
    }

    

    /* Map the segment in shared RW mode */
    if ((sp = mmap (NULL, (N+1) * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror ( "parent mmap");
        exit(1);}

    for (i = 0; i < N; i ++){
        sem_wait(smutex2);
    }
    
    result = 0;
    for (i = 0; i < N; i ++){
        result += *sp;
        sp += sizeof(int);
    }
    *sp = result;
    printf("(Parent)sum of N values is %d\n", result);
    
    for (i = 0; i < N; i ++){
        sem_post(smutex);
    }

    for (i = 0; i < N; i ++){
        wait(NULL);
    }

    sem_close(smutex);
    sem_unlink("mysem");
    sem_close(smutex2);
    sem_unlink("mysem2");
    
    /* "Detach" segment */
    munmap(sp-N*sizeof(int), (N+1) * sizeof(int));

    /* Destroy * the segment*/
    shm_unlink("myshm");
    return 0;
}