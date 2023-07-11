#include <os-scheduling.h>

/*******************************
 * QUEUE MANAGEMENT PRIMITIVES *
 *******************************/

void printQueues(task tasks[], sched_data* schedData) {
    int i, j, taskIndex = 0;
    printf("Nb of queues %d\n", schedData->nbOfQueues);
    for (i = 0; i < schedData->nbOfQueues; i++) {
        j = 0;
        printf("Q%d => ", i);
        while (j < MAX_NB_OF_TASKS) {
            taskIndex = schedData->queues[i][j];
            if (taskIndex == -1) {
                j = MAX_NB_OF_TASKS;
            } else {
                printf("%s ", tasks[taskIndex].name);
                j++;
            }
        }
        printf("\n");
    }
}

void initQueues(int nbQ, sched_data* schedData) {
    int i, j;
    printf("Initializing %d job queue(s)\n", nbQ);
    schedData->nbOfQueues = nbQ;
    for (j = 0; j < nbQ; j++) {
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[j][i] = -1;
        }
    }
}

int enqueue(sched_data* schedData, int queueIndex, int taskIndex) {
    int end = 0;
    while ((end < MAX_NB_OF_TASKS) && (schedData->queues[queueIndex][end] != -1))
        end++;
    if (end < MAX_NB_OF_TASKS) {
        schedData->queues[queueIndex][end] = taskIndex;
        return 0;
    } else {
        // memory exhausted
        return -1;
    }
}


int dequeue(sched_data* schedData, int queueIndex) {
    int j;
    int taskIndex = schedData->queues[queueIndex][0];
    if (taskIndex != -1) {
        for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
            schedData->queues[queueIndex][j] = schedData->queues[queueIndex][j+1];
        }
        schedData->queues[queueIndex][MAX_NB_OF_TASKS - 1] = -1;
    }
    return taskIndex;
}

int head(sched_data* schedData, int queueIndex) {
    return schedData->queues[queueIndex][0];
}


/*******************************
 * TASKS MANAGEMENT PRIMITIVES *
 *******************************/

int admitNewTasks(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j;
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            schedData->queues[0][j] = i;
            j++;
        }
    }
    return 1;
}


/***********************
 * SCHEDULING POLICIES *
 ***********************/

int FCFS(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    
    int i;
    
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        initQueues(1, schedData);
    }
    
    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    printQueues(tasks, schedData);
    
    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    i = head(schedData, 0);
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                dequeue(schedData, 0);
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    i = head(schedData, 0);
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].state = RUNNING;
        return i;
    }
    
    // No task could be elected
    return -1;
}


int SJF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j, minCompTime;
    
    // N.B: SJF does not require any queue!
    
    // Admit new tasks (currentTime >= arrivalTime)
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
        }
    }
    
    // Is there a task running? Has that task finished its computations?
    //   If so, put it in terminated state
    //   If not, continue this task
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                break;
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    
    //Otherwise, find the task in READY state that has the shortest computation time
    j = -1;
    minCompTime = 0;
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == READY) {
            if ((j == -1) || (minCompTime > tasks[i].computationTime)) {
                j = i;
                minCompTime = tasks[i].computationTime;
            }
        }
    }
    if (j != -1){
        tasks[j].executionTime ++;
        tasks[j].state = RUNNING;
        return j;
    }
    
    return -1;
}


int SRTF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j, remainingCompTime, minRemainingCompTime;
    
    // N.B: SRTF does not require any queue!
    
    // Admit new tasks (currentTime >= arrivalTime)
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
        }
    }
    
    // Is there a task running?
    //      => determine its remaining computation time
    //   Has that task finished its computations?
    //      => put it in terminated state
    //   else
    //      => put it back to READY
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == RUNNING) {
            remainingCompTime = tasks[i].computationTime - tasks[i].executionTime;
            if (remainingCompTime == 0) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
            } else {
                tasks[i].state = READY;
            }
            break;
        }
    }
    
    //Now elect the task in READY state that has the shortest remaining time
    j = -1;
    minRemainingCompTime = -1;
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == READY) {
            remainingCompTime = tasks[i].computationTime - tasks[i].executionTime;
            if ((j == -1) ||
                (minRemainingCompTime > remainingCompTime)) {
                j = i;
                minRemainingCompTime = remainingCompTime;
            }
        }
    }
    if (j != -1){
        tasks[j].executionTime ++;
        tasks[j].state = RUNNING;
        return j;
    }
    
    return -1;
}


int RR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO
    return -1;
}

int MFQ(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO
    return -1;
}

int IORR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO
    return -1;
}
