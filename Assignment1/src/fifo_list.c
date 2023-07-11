#include <fifo.h>
#include <list.h>
#include <stdio.h>
#include <stdlib.h>

struct list_type q;

/* Initialization of the queue */
int init_queue(){
    init_list(&q);
    return 0;
}

/* Insertion of a new element.
 The new element is added to the head of the queue. */
int queue(void* element){
    insert_head(&q, element);
    return 0;
}

/* Extraction of the element at the tail of the queue.
 Returns (also deletes) that element.  */
void* dequeue(){
    return extract_tail(&q);
}

/* Returns the number of elements in the queue */
int size(){
    return list_size(&q);
}