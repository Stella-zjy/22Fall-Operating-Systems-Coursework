#include <list.h>
#include <stdlib.h>
#include <stdio.h>


void init_list(struct list_type *l) {
	l->head = NULL;
	l->tail = NULL;
}
	
	
void insert_head(struct list_type *l, void* element) {
	cell* new_cell = malloc(sizeof(cell));
	new_cell->content = element;
	new_cell->previous = NULL;
	new_cell->next = l->head;
	if (l->head != NULL)
		l->head->previous = new_cell;
	l->head = new_cell;
	if (l->tail == NULL)
			l->tail = l->head;
}


void* extract_head(struct list_type *l) {
	/* TODO */
	if (l->head == NULL){
		printf("Empty, nothing to delete.\n");
		return NULL;
	}
	cell *target_head = l->head;
	if (l->head->next != NULL){
		l->head = l->head->next;
		l->head->previous = NULL;
		void *target_element = target_head->content;
		free(target_head);
		return target_element;
	}
	else{
		l->head = NULL;
		l->tail = NULL;
		void *target_element = target_head->content;
		free(target_head);
		return target_element;
	}
}


void* extract_tail(struct list_type *l) {
	/* TODO */
	if (l->head == NULL){
		printf("Empty, nothing to delete.\n");
		return NULL;
	}
	cell *target_tail = l->tail;
	if (l->tail->previous != NULL){
		l->tail = l->tail->previous;
		l->tail->next = NULL;
		void *target_element = target_tail->content;
		free(target_tail);
		return target_element;
	}
	else{
		l->head = NULL;
		l->tail = NULL;
		void *target_element = target_tail->content;
		free(target_tail);
		return target_element;
	}
}


int list_size(struct list_type *l) {
	/* TODO */
	if (l->head == NULL){
		return 0;
	}
	else{
		int count = 1;
		cell* temp = l->head;
		while (temp->next != NULL){
			count += 1;
			temp = temp->next;
		}
		return count;
	}
}
	