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
    int fd, i, j, p, random_val;
    int round_result=0;
    int total_result=0;
    sem_t* sem1;
    sem_t* sem2;
    sem_t* sem3;
    sem_t* smutex;
    
    /* Create segment myshm */
    if ((fd = shm_open ( "myshm", O_RDWR | O_CREAT, 0600)) == -1) {
        perror ( "shm_open");
        exit (1);}
    
    /* Allocate enough memory to store N integers */
    if (ftruncate (fd, N * sizeof(int)) == -1) {
        perror("ftruncate");
        exit (1);}

    if ((sem1 = sem_open ( "mysem1", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open1");
            exit(1);
        }
        /* Semaphore already created, open without O_CREAT */
        sem1 = sem_open("mysem1", O_RDWR);
    }

    if ((sem2 = sem_open ( "mysem2", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open2");
            exit(1);
        }
        /* Semaphore already created, open without O_CREAT */
        sem2 = sem_open("mysem2", O_RDWR);
    }

    if ((sem3 = sem_open ( "mysem3", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open3");
            exit(1);
        }
        /* Semaphore already created, open without O_CREAT */
        sem3 = sem_open("mysem3", O_RDWR);
    }

    // if ((smutex = sem_open ( "smutex", O_CREAT | O_EXCL | O_RDWR, 0666, 1)) == SEM_FAILED) {
    //     if (errno != EEXIST) {
    //         perror("sem_openmx");
    //         exit(1);
    //     }
    //     /* Semaphore already created, open without O_CREAT */
    //     smutex = sem_open("smutex", O_RDWR);
    // }
    
    /* Map the segment in shared RW mode */
    if ((sp = mmap (NULL, N * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror ( "mmap");
        exit (1);}

    for (i = 0; i < N; i ++){
        p = fork();
        if (p == 0){
            for (j = 0; j < N; j ++){
                // int value;
                
                sem_wait(sem3);            

                // sem_wait(smutex);
                srand(getpid());
                random_val = rand()%10;
                *(sp + i*sizeof(int)) = random_val;
                printf("(Round %d) child process %d with random value %d\n", j+1, i+1, random_val);
                // sem_post(smutex);
                
                sem_post(sem2);
                // sem_getvalue(sem1, &value);
                // printf("child process %d sem1 = %d\n",i+1, value);
                sem_wait(sem1);
                // sem_getvalue(sem1, &value);
                // printf("child process %d sem1 = %d\n",i+1, value);   
            }

            // sem_close(smutex);
            sem_close(sem1);
            sem_close(sem2);
            sem_close(sem3);
     
            munmap(sp-i*sizeof(int), N*sizeof(int));
            exit(0);
        }
    }

    /* Loop for N rounds */
    for (j = 0; j < N; j ++){

        for (i = 0; i < N; i ++){
            sem_post(sem3);
        }

        for (i = 0; i < N; i ++){
            sem_wait(sem2);
        }

        round_result += *sp;
        for (i = 1; i < N; i ++){
            sp += sizeof(int);
            round_result += *sp;
        }
        total_result += round_result;
        sp -= (N-1)*sizeof(int);
        printf("Round %d sum is %d, total sum is %d\n", j+1, round_result, total_result);
        round_result = 0;
        
        for (i = 0; i < N; i ++){
            sem_post(sem1);
        }
        
    }
    

    for (i = 0; i < N; i ++){
        wait(NULL);
    }

    sem_close(sem1);
    sem_unlink("mysem1");
    sem_close(sem2);
    sem_unlink("mysem2");
    // sem_close(sem3);
    // sem_unlink("mysem3");
    sem_close(smutex);
    sem_unlink("smutex");
    
    /* "Detach" segment */
    munmap(sp-(N-1)*sizeof(int), N * sizeof(int));

    /* Destroy * the segment*/
    shm_unlink("myshm");
    return 0;
}