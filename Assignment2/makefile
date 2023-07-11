CC=gcc -Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

# Q1: ${SRC}/trace.c
# 	${CC} -o ${OBJ}/trace.o ${SRC}/trace.c
# 	@echo "##################################################"
# 	@${OBJ}/1.o
# 	@echo "##################################################"

# Q2: ${SRC}/2.c
# 	${CC} -o ${OBJ}/2.o $^
# 	@echo "##################################################"
# 	@${OBJ}/2.o
# 	@echo "##################################################"

# Q3: ${SRC}/3.c
# 	${CC} -o ${OBJ}/3.o $^
# 	@echo "##################################################"
# 	@${OBJ}/3.o USD 256
# 	@echo "##################################################"


all: Q1 Q2 Q3

# Q1
${BIN}/q1: ${OBJ}/trace.o
	${CC} -o ${BIN}/q1 ${OBJ}/trace.o

${OBJ}/trace.o: ${SRC}/trace.c
	${CC} -c -o ${OBJ}/trace.o ${SRC}/trace.c

# Q2
${BIN}/q2: ${OBJ}/decoder.o
	${CC} -o ${BIN}/q2 ${OBJ}/decoder.o

${OBJ}/decoder.o: ${INC}/decoder.h ${SRC}/decoder.c
	${CC} -c -o ${OBJ}/decoder.o ${SRC}/decoder.c -I${INC}

# Q3
${BIN}/q3: ${OBJ}/multi-converter.o ${OBJ}/converters.o
	${CC} -o ${BIN}/q3 ${OBJ}/multi-converter.o ${OBJ}/converters.o

${OBJ}/converters.o: ${INC}/converters.h ${SRC}/converters.c
	${CC} -c -o ${OBJ}/converters.o ${SRC}/converters.c -I${INC}

${OBJ}/multi-converter.o: ${INC}/converters.h ${SRC}/multi-converter.c ${OBJ}/converters.o
	${CC} -c -o ${OBJ}/multi-converter.o ${SRC}/multi-converter.c -I${INC}


clean:
	rm -f ${LIB}/* ${OBJ}/* ${BIN}/*


Q1: ${BIN}/q1
	echo "Testing Q1"
	${BIN}/q1

Q2: ${BIN}/q2
	echo "Testing Q2"
	${BIN}/q2

Q3: ${BIN}/q3
	echo "Testing Q3"
	${BIN}/q3 USD 256
