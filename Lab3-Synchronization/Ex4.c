#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

//TODO: Declare your shared memory / semaphore structures and variables
int fd;
int* NW;
sem_t* smutex;
sem_t* sem1;


void compute() {
    int j;
    for (j = 0; j < 1E8; j ++);
}

void cleanup(int N_PCS) {
    //TODO: Destroy your shared memory / semaphores
    sem_close(smutex);
    sem_unlink("mymx");
    sem_close(sem1);
    sem_unlink("mysem1");

    munmap(NW, N_PCS*sizeof(int));
    shm_unlink("myshm");
    
}

/**
 * This function redefines the behavior of your executable
 * when you type Ctrl-C in the terminal: instead of terminating
 * directly, the main process will call cleanup()
 */
void init() {
    sigset_t mask;
    struct sigaction act;
    
    sigfillset(&mask);
    act.sa_handler = cleanup;
    act.sa_mask = mask;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
}

void wait_barrier(int nb_pcs, int N_PCS) {
    //TODO: Write your synchronization
    int i, flag=0;
    sem_wait(smutex);

    if ((NW = mmap (NULL, N_PCS*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror("sub mmap");
        exit(1);}
    
    *(NW + nb_pcs*sizeof(int)) = 1;

    for (i = 0; i < N_PCS; i ++){
        if (*(NW + i*sizeof(int)) == 0){
            flag = 1;
            break;
        }
    }
    if (flag == 0){
        for (i = 0; i < N_PCS; i ++){
            *(NW + i*sizeof(int)) = 0;
        }
        for (i = 1; i < N_PCS; i ++){
            sem_post(sem1);
        }
        munmap(NW, N_PCS*sizeof(int));
        sem_post(smutex);
    }else{
        munmap(NW, N_PCS*sizeof(int));
        sem_post(smutex);
        sem_wait(sem1);
    }
}


int main(int argc , char **argv) {
    int i, j, pid;

    /* Check the main arguments */
    if (argc != 3) {
        printf("wrong usage: %s <# processes> <# loops>\n", argv[0]);
        exit (1);
    }
    int N_PCS = atoi(argv[1]);
    int N_LOOPS = atoi(argv[2]);

    //TODO: Setup your shared memory / semaphores
    /* Create segment myshm */
    if ((fd = shm_open ( "myshm", O_RDWR | O_CREAT, 0600)) == -1) {
        perror ( "shm_open");
        exit(1);}
    
    /* Allocate enough memory to store an integer */
    if (ftruncate (fd, N_PCS*sizeof(int)) == -1) {
        perror("ftruncate");
        exit(1);}

    if ((smutex = sem_open ( "mymx", O_CREAT | O_EXCL | O_RDWR, 0666, 1)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open");
            exit(1);
        }
        /* Semaphore already created, open without O_CREAT */
        smutex = sem_open("mymx", O_RDWR);
    }

    if ((sem1 = sem_open ( "mysem1", O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open");
            exit(1);
        }
        /* Semaphore already created, open without O_CREAT */
        sem1 = sem_open("mysem1", O_RDWR);
    }

    if ((NW = mmap (NULL, N_PCS*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror ( "parent mmap");
        exit(1);}
    
    for (i = 0; i < N_PCS; i ++){
        *(NW + i*sizeof(int)) = 0;
    }

    /* Record the PID of the initial parent process */
    pid = getpid();

    init();

    for (i = 0; ((i < N_PCS - 1) && (fork() > 0)); i++);
    
    for (j = 0; j < N_LOOPS; j++) {
        compute();
        printf("%d(%d)> Before barrier #%d\n", getpid(), i, j);
        wait_barrier(i, N_PCS);
        printf("%d(%d)> After barrier #%d\n", getpid(), i, j);
        compute();
    }
    
    if (pid == getpid()) {
        for (i = 0; i < N_PCS -1; i++) {
            if (wait(NULL) == -1) {
                perror("wait");
                exit(2);
            }
        }
        cleanup(N_PCS);
    }
    
    return 0;
}
