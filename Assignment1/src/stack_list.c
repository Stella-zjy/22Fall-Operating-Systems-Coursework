#include <list.h>
#include <stack.h>
#include <stdlib.h>
#include <stdio.h>


struct list_type s;

/* Initialization of the stack */
int init_stack(){
    init_list(&s);
    return 0;
}

/* Insertion: adds an element to the top of the stack */
int push(void* element){
    if (list_size(&s) == STACK_SIZE){
        printf("stack length out of range");
        return -1;
    }
    else{
        insert_head(&s, element);
    }
    return 0;
}

/* Extraction: the element at the top of the stack is removed and its value is returned */
void* pop(){
    return extract_head(&s);
}

/* Returns the number of elements in the stack */
int size(){
    return list_size(&s);
}
