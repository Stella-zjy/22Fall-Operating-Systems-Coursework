CC=gcc -Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all: runstack runstacknew runqueue

# Stack Test using Stack Array
${BIN}/stack_test: ${OBJ}/stack_test.o ${OBJ}/stack_array.o
	${CC} -o ${BIN}/stack_test ${OBJ}/stack_test.o ${OBJ}/stack_array.o

${OBJ}/stack_array.o: ${INC}/stack.h ${SRC}/stack_array.c
	${CC} -c -o ${OBJ}/stack_array.o ${SRC}/stack_array.c -I${INC}

${OBJ}/stack_test.o: ${INC}/stack.h ${SRC}/stack_test.c
	${CC} -c -o ${OBJ}/stack_test.o ${SRC}/stack_test.c -I${INC}



# Stack Test using Stack List (Built from Doubly Linked List)
${BIN}/stack_test_new: ${OBJ}/stack_test.o ${LIB}/libstack.a
	${CC} -o ${BIN}/stack_test_new ${OBJ}/stack_test.o -L${LIB} -lstack

${OBJ}/list_impl.o: ${INC}/list.h ${SRC}/list_impl.c
	${CC} -c -o ${OBJ}/list_impl.o ${SRC}/list_impl.c -I${INC}

${OBJ}/stack_list.o: ${INC}/stack.h ${INC}/list.h ${SRC}/stack_list.c ${OBJ}/list_impl.o
	${CC} -c -o ${OBJ}/stack_list.o ${SRC}/stack_list.c -I${INC}

${LIB}/libstack.a: ${OBJ}/stack_list.o ${OBJ}/list_impl.o
	ar rcv ${LIB}/libstack.a ${OBJ}/stack_list.o ${OBJ}/list_impl.o
	ranlib ${LIB}/libstack.a


# Queue Test
${BIN}/fifo_test: ${OBJ}/fifo_test.o ${LIB}/libfifo.a
	${CC} -o ${BIN}/fifo_test ${OBJ}/fifo_test.o -L${LIB} -lfifo

${OBJ}/fifo_test.o: ${INC}/fifo.h ${SRC}/fifo_test.c
	${CC} -c -o ${OBJ}/fifo_test.o ${SRC}/fifo_test.c -I${INC}

${OBJ}/fifo_list.o: ${INC}/fifo.h ${INC}/list.h ${SRC}/fifo_list.c ${OBJ}/list_impl.o
	${CC} -c -o ${OBJ}/fifo_list.o ${SRC}/fifo_list.c -I${INC}

${LIB}/libfifo.a: ${OBJ}/fifo_list.o ${OBJ}/list_impl.o
	ar rcv ${LIB}/libfifo.a ${OBJ}/fifo_list.o ${OBJ}/list_impl.o
	ranlib ${LIB}/libfifo.a


runstack: ${BIN}/stack_test
	echo "Testing Stack"
	${BIN}/stack_test


runstacknew: ${BIN}/stack_test_new
	echo "Testing Stack built from doubly linked list"
	${BIN}/stack_test_new

runqueue: ${BIN}/fifo_test
	echo "Testing Queue built from doubly linked list"
	${BIN}/fifo_test

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
