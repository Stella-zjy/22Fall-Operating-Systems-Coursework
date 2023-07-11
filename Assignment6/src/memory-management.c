#include <os-memory.h>


void first_fit(int size, freezone* fz) {
    int found = freelist;
    int previous = -1;

    while ((found != -1) && (heap[found] < size)) {
        previous = found;
        found = heap[found + 1];
    }

    fz->previous = previous;
    fz->found = found;
}

void best_fit(int size, freezone* fz) {
    // TODO
    int found = freelist;
    int previous = -1;
    int min_size, min_idx, min_prev_idx;
    min_size = HEAP_SIZE;
    
    while (found != -1) {
        // Loop to find the smallest possible freezone
        if (heap[found] >= size && heap[found] < min_size){
            min_size = heap[found];
            min_idx = found;
            min_prev_idx = previous;
        }
        previous = found;
        found = heap[found + 1];
    }

    fz->previous = min_prev_idx;
    fz->found = min_idx;
}

void worst_fit(int size, freezone* fz) {
    // TODO
    int found = freelist;
    int previous = -1;
    int max_size, max_idx, max_prev_idx;
    max_size = 0;
    
    while (found != -1) {
        // Loop to find the largest possible freezone
        if (heap[found] >= size && heap[found] > max_size){
            max_size = heap[found];
            max_idx = found;
            max_prev_idx = previous;
        }
        previous = found;
        found = heap[found + 1];
    }

    fz->previous = max_prev_idx;
    fz->found = max_idx;
}


void* heap_malloc(int size) {
    freezone result;                    //free zone found for the allocation
    int allocation_size = size + 1;     //size of the allocated zone
    void *ptr = NULL;                   //pointer to the allocated zone
    
    find_free_zone(size, &result);
    
    //Cannot find a free zone
    if (result.found == -1)
        return NULL;
    
    // TODO

    ptr = (void *) (&heap[result.found] + 1);
    int cur_size = heap[result.found];
    int next_free_idx = heap[result.found + 1];
   
    // First consider if the remaining space of this zone is still a valid freezone
    if (cur_size - allocation_size < MIN_BLOCK_SIZE){
        // If the whole heap is occupied
        if (next_free_idx == -1){
            freelist = -1;
        }
        // If this is the first freezone
        if (result.previous == -1){
            freelist = next_free_idx;
        } else {
            heap[result.previous + 1] = next_free_idx;
        }

    } else {
        // If there exists a new fragmented freezone

        // If this is the first freezone
        if (result.previous == -1){
            freelist += allocation_size;
        } else {
            heap[result.previous + 1] += allocation_size;
        }
        // Update the fragment freezone
        heap[result.found + allocation_size] = cur_size - allocation_size;
        heap[result.found + allocation_size + 1] = next_free_idx;
    }

    // Update current allocated memory size
    heap[result.found] = size;
     
    return ptr;
}


int heap_free(void *dz) {
    // TODO
    int target_idx = (char *) dz - heap - 1;
    // printf("targetidx %d\n", target_idx);
    int target_prev_idx, target_next_idx;
    int target_size = heap[target_idx];

    // Consider if the original heap is fully-occupied
    if (freelist == -1){
        freelist = target_idx;
        heap[target_idx + 1] = -1;
        target_prev_idx = -1;
        target_next_idx = -1;
    
    } else { 
        // When the freelist != -1

        // Consider if this is the new first freezone
        if (target_idx < freelist){

            // If this freezone continues to the next freezone (the original first)
            if (target_idx + target_size + 1 == freelist){
                heap[target_idx] = heap[freelist] + target_size + 1;
                heap[target_idx + 1] = heap[freelist + 1];
            } else {
                // If the next freezone (the original first) locates one cell after this freezone
                if (target_idx + target_size + 2 == freelist){
                    heap[target_idx] = heap[freelist] + target_size + 2;
                    heap[target_idx + 1] = heap[freelist + 1];
                } else {
                    // No freezone merge

                    heap[target_idx + 1] = freelist;
                }    
            }
            target_prev_idx = -1;
            // target_next_idx = heap[target_idx + 1];
            freelist = target_idx;


        } else {
            // When this is not the first freezone

            target_next_idx = freelist;
            // Loop until find the target location
            while (target_next_idx < target_idx){
                target_prev_idx = target_next_idx;
                target_next_idx = heap[target_next_idx + 1];
                // If this freezone is the last freezone
                if (target_next_idx == -1){
                    break;
                }
            }

            // If the previous freezone locates right before this freezone
            if (target_prev_idx + heap[target_prev_idx] + 1 == target_idx){
                target_idx = target_prev_idx;
                target_size += heap[target_prev_idx] + 1;
                target_next_idx = heap[target_idx + 1];
            } else {
                // If the previous freezone locates one cell before this freezone
                if (target_prev_idx + heap[target_prev_idx] + 2 == target_idx){
                    target_idx = target_prev_idx;
                    target_size += heap[target_prev_idx] + 2;
                    target_next_idx = heap[target_idx + 1];
                } else {
                    // No freezone merge with previous
                    heap[target_prev_idx + 1] = target_idx;
                }     
            }
            heap[target_idx] = target_size;
            

            // If the next freezone locates right after this freezone
            if (target_idx + target_size + 1 == target_next_idx){
                heap[target_idx] = heap[target_next_idx] + target_size + 1;
                target_next_idx = heap[target_next_idx + 1];
            } else {
                 // If the next freezone locates one cell after this freezone
                if (target_idx + target_size + 2 == target_next_idx){
                    heap[target_idx] = heap[target_next_idx] + target_size + 2;
                    target_next_idx = heap[target_next_idx + 1];
                }
            }
            heap[target_idx + 1] = target_next_idx;  
        }
    }

    // Reset heap value corresponding to the freezone
    int i;
    for (i = 0; i < heap[target_idx]-1; i ++){
        heap[target_idx + i + 2] = 0;
    }
    
    return 0;
}

int heap_defrag() {
    // TODO
    return 0;
}


