#include <os-scheduling.h>


int admitNewTasks(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j;
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            // Initialize cyclesInQuantum to be zero
            tasks[i].cyclesInQuantum = 0; 
            // Initialize cyclesInIO to be zero (used for IORR)
            tasks[i].cyclesInIO = 0; 
            schedData->queues[0][j] = i;
            j++;
        }
    }
    return 1;
}


int FCFS(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    
    int i, j;
    
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }
    
    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    printQueues(tasks, schedData);
    
    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[0][j] = schedData->queues[0][j+1];
                }
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    i = schedData->queues[0][0];
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].state = RUNNING;
        return i;
    }
    
    // No task could be elected
    return -1;
}



int SJF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO (LAB Q3)
    int i, j, p, q, temp;
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }
    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    
    // We put the task with the shortest computationTime to the second in the queue (right after the running task)
    // Note that every time a new task is inserted, it's added to the end of the queue automatically
    // Here we sort the queue by computationTime
    j = 0;
    while (schedData->queues[0][j] != -1){
        j += 1;
    }
    // By looping p, we put each new task in the correct order
    for (p = j-nbOfTasks; p < j; p++){
        // By looping p, we compare the current new task with the (ordered) tasks in the queue
        for (q = p; q > 1; q--){
            if (tasks[schedData->queues[0][q]].computationTime < tasks[schedData->queues[0][q-1]].computationTime){
                temp = schedData->queues[0][q];
                schedData->queues[0][q] = schedData->queues[0][q-1];
                schedData->queues[0][q-1] = temp;
            }
        }
    }   

    printQueues(tasks, schedData);
    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[0][j] = schedData->queues[0][j+1];
                }
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    // Otherwise, elect the task with the shortest computation time in the queue
    // which is the first task in the queue
    i = schedData->queues[0][0];
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].state = RUNNING;
        return i;
    }   
    // No task could be elected
    return -1;
}



int SRTF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO (LAB Q3)
    int i, j, p, q, temp;
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }
    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    
    j = 0;
    while (schedData->queues[0][j] != -1){
        j += 1;
    }
    // By looping p, we put each new task in the correct order
    for (p = j-nbOfTasks; p < j; p++){
        // By looping p, we compare the current new task with the (ordered) tasks in the queue
        for (q = p; q > 0; q--){
            if (tasks[schedData->queues[0][q]].computationTime < tasks[schedData->queues[0][q-1]].computationTime - tasks[schedData->queues[0][q-1]].executionTime){
                if (q-1 == 0){
                    tasks[schedData->queues[0][q-1]].state = READY;
                }
                temp = schedData->queues[0][q];
                schedData->queues[0][q] = schedData->queues[0][q-1];
                schedData->queues[0][q-1] = temp;
            }
        }
    }
    
    printQueues(tasks, schedData);
    
    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[0][j] = schedData->queues[0][j+1];
                }
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    // Otherwise, elect the first task in the queue
    i = schedData->queues[0][0];
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].state = RUNNING;
        return i;
    }   
    // No task could be elected
    return -1;
}



int RR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO (ASSIGNMENT Q1)
    int i, j, temp, new_first;
    
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }
    
    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);

    // Initialize cyclesInQuantum to be zero every time we have new tasks added (by modifying the admitNewTasks() function)

    printQueues(tasks, schedData);

    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            // Check completion
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[0][j] = schedData->queues[0][j+1];
                }
                schedData->queues[0][MAX_NB_OF_TASKS - 1] = -1;
            } else {
                // Check quantum end
                if (tasks[i].cyclesInQuantum == schedData->quantum){
                    tasks[i].state = READY;
                    tasks[i].cyclesInQuantum = 0;
                    temp = schedData->queues[0][0];
                    j = 0;
                    while ((j < MAX_NB_OF_TASKS - 1) && schedData->queues[0][j+1] != -1){
                        schedData->queues[0][j] = schedData->queues[0][j+1];
                        j++;
                    }
                    schedData->queues[0][j] = temp;
                    new_first = schedData->queues[0][0];
                    tasks[new_first].state = RUNNING;
                    tasks[new_first].executionTime ++;
                    tasks[new_first].cyclesInQuantum ++;
                    return new_first;
                }else{
                    /* Reelect this task */
                    tasks[i].executionTime ++;
                    tasks[i].cyclesInQuantum ++;
                    return i;
                }
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    i = schedData->queues[0][0];
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].cyclesInQuantum ++;
        tasks[i].state = RUNNING;
        return i;
    }
    
    // No task could be elected
    return -1;
}



int MFQ(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO (ASSIGNMENT Q2)
    int i, j, p, new_p, temp, new_first;
    // Initialize 3 queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 3;
        for (j = 0; j < schedData->nbOfQueues; j++){
            for (i = 0; i < MAX_NB_OF_TASKS; i++) {
                schedData->queues[j][i] = -1;
            }
        } 
    }
    
    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    printQueues(tasks, schedData);

    // Loop to check every queue orderly
    for (p = 0; p < schedData->nbOfQueues; p++){
        i = schedData->queues[p][0];
        if (i != -1){
            if (tasks[i].state == RUNNING) {
                if (tasks[i].executionTime == tasks[i].computationTime) {
                    tasks[i].state = TERMINATED;
                    tasks[i].completionDate = currentTime;
                    for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                        schedData->queues[p][j] = schedData->queues[p][j+1];
                    }
                    schedData->queues[p][MAX_NB_OF_TASKS - 1] = -1;
                } else {
                    // Check quantum end
                    if (tasks[i].cyclesInQuantum == schedData->quantum * (p+1)){
                        tasks[i].state = READY;
                        tasks[i].cyclesInQuantum = 0;
                        temp = schedData->queues[p][0];
                        // Deal with queue p
                        j = 0;
                        while (schedData->queues[p][j] != -1){
                            schedData->queues[p][j] = schedData->queues[p][j+1];
                            j++;
                        }
                        // Deal with queue new_p
                        new_p = (p + 1) % schedData->nbOfQueues;
                        j = 0;
                        while (schedData->queues[new_p][j] != -1){
                            j++;
                        }
                        schedData->queues[new_p][j] = temp;
                        // Elect task[new_first]
                        if (schedData->queues[p][0] != -1){
                            new_first = schedData->queues[p][0];
                        } else {
                            new_first = schedData->queues[new_p][0];
                        }
                        tasks[new_first].state = RUNNING;
                        tasks[new_first].executionTime ++;
                        tasks[new_first].cyclesInQuantum ++;
                        return new_first;
                    }
                }
            }
        }
    }

    // Otherwise, elect the first task in the queue
    for (p = 0; p < schedData->nbOfQueues; p++){
        i = schedData->queues[p][0];
        if (i != -1){
            tasks[i].executionTime ++;
            tasks[i].cyclesInQuantum ++;
            tasks[i].state = RUNNING;
            return i;
        }
    }
    
    // No task could be elected
    return -1;
}



int IORR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO (ASSIGNMENT BONUS)
    int i, j, p, k, temp; 
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }

    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    printQueues(tasks, schedData);

    // Update IO
    for (j = 0; j < MAX_NB_OF_TASKS; j++){
        p = schedData->queues[0][j];
        if ((p != -1) && (tasks[p].state == SUSPENDED)){
            if (tasks[p].cyclesInIO == tasks[p].IOTime){
                if (tasks[p].executionTime == tasks[p].computationTime){
                    tasks[p].state = TERMINATED;
                    tasks[p].completionDate = currentTime;
                    // Update Queue
                    for (k = j; k < MAX_NB_OF_TASKS - 1; k++) {
                        schedData->queues[0][k] = schedData->queues[0][k+1];
                    }
                    schedData->queues[0][MAX_NB_OF_TASKS - 1] = -1;
                    j --;
                } else {
                    tasks[p].state = READY;
                    tasks[p].cyclesInIO = 0;
                }
            } else {
                tasks[p].cyclesInIO ++;
            }
        }
    }  

    // Deal with Execution
    j = 0;
    while ((schedData->queues[0][j] != -1) && (tasks[schedData->queues[0][j]].state == SUSPENDED)){
        j ++;
    }

    i = schedData->queues[0][j]; // i denotes the taskIndex of the current task to execute

    /* This is the case when we're done or we only have SUSPENDED tasks in queue */
    if ((i == -1) || (tasks[i].state == SUSPENDED)){
        return -1;
    }

    /* When task[i] is READY */
    if (tasks[i].state == READY){
        tasks[i].executionTime ++;
        tasks[i].cyclesInQuantum ++;
        tasks[i].state = RUNNING;
        return i;
    }

    /* When task[i] is RUNNING */
    if (tasks[i].state == RUNNING){
        // Check I/O Request
        if ((tasks[i].IORate != 0) && (tasks[i].executionTime % tasks[i].IORate == 0)){
            tasks[i].state = SUSPENDED;
            tasks[i].cyclesInIO ++;
            tasks[i].cyclesInQuantum = 0;
            temp = schedData->queues[0][j];
            k = j;
            while ((k < MAX_NB_OF_TASKS - 1) && (schedData->queues[0][k+1] != -1)){
                schedData->queues[0][k] = schedData->queues[0][k+1];
                k++;
            }
            schedData->queues[0][k] = temp;
        } else {
            // Check Completion
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                for (k = j; k < MAX_NB_OF_TASKS - 1; k++) {
                    schedData->queues[0][k] = schedData->queues[0][k+1];
                }
                schedData->queues[0][MAX_NB_OF_TASKS - 1] = -1;
            } else {
                // Check Quantum End
                if (tasks[i].cyclesInQuantum == schedData->quantum){
                    tasks[i].state = READY;
                    tasks[i].cyclesInQuantum = 0;
                    temp = schedData->queues[0][j];
                    k = j;
                    while ((k < MAX_NB_OF_TASKS - 1) && (schedData->queues[0][k+1] != -1)){
                        schedData->queues[0][k] = schedData->queues[0][k+1];
                        k++;
                    }
                    schedData->queues[0][k] = temp;
                } else {
                    /* Reelect this task */
                    tasks[i].executionTime ++;
                    tasks[i].cyclesInQuantum ++;
                    return i;
                }
            }
        } 
        // So far we have a modified queue and we want to elect a valid task
        for (j = 0; j < MAX_NB_OF_TASKS; j++){
            p = schedData->queues[0][j];
            if (tasks[p].state == READY){
                tasks[p].state = RUNNING;
                tasks[p].executionTime ++;
                tasks[p].cyclesInQuantum ++;
                return p;
            }
        }
    }

    // No task could be elected
    return -1;
}

    