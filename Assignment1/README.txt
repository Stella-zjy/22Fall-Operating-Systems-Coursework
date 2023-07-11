
First Name: Jingyi

Last Name: Zhao

NYU ID: jz3676

=======================================================================================
Files submitted:

src:
	fifo_list.c: Question 4
	fifo_test.c: Question 4
	list_impl.c: Question 2, Question 3, Question 4
	stack_array.c: Question 1
	stack_list.c: Question 3
	stack_test.c: Question2, Question 3

include:
	fifo.h: Question 4
	list.h: Question 3, Question 4
	stack.h: Question 1, Question 2, Question 3


=======================================================================================
Compilation rules in the Makefile:

# A few examples below
# make doublell: 	compile the doubly linked list implementation (Q2)
# make newlibstack:	recompiles the stack, implemented as a double-linked list, then integrates it in an update of libstack.a (Q3)
# make libfifo: 	compiles a dynamic queue implemented as a double-linked list, then integrates it in a library (lib/libfifo.a) (Q4)


make clean: remove all the files in obj/bin/lib
make runstack: run the stack_test with the stack implemented by stack_array using array
make runstacknew: run the stack_test with the stack implemented by stack_list using doubly linked list (Q3)
make runqueue: run the fifo_test with the queue implemented by fifo_list using doubly linked list (Q4)

=======================================================================================
Comments:



=======================================================================================
Textual answers: 

Question 5:

In list_impl.c file, when defining init_list(), we may add one more attribute to l, say, l->size, to record the size of the linked list.
Everytime we modify the linked list (insertion/extraction), we modify the value of l->size accordingly.
Then, the list_size() can directly return l->size, which costs O(1) each calling.