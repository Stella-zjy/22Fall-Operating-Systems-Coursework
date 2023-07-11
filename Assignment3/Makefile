CFLAGS=-Iinclude -Wall
FILE=tasks
FILE2=tasks2
FILEY=tasksY
POLICYRR=RR 4
POLICYMFQ=MFQ 3
POLICYIORR=IORR 4
#POLICY values
#	FCFS
#	SJF
#	SRTF
#	RR 4
#	MFQ 3
#	IORR 4

all: build runrr runmfq runiorr

build: clean
	mkdir obj
	gcc $(CFLAGS) -c -o obj/policies.o src/sched-policies.c	
	gcc $(CFLAGS) -c -o obj/simulator.o src/sched-simulator.c
	mkdir bin
	gcc $(CFLAGS) -o bin/sched-simulator obj/simulator.o obj/policies.o

# run:
# 	bin/sched-simulator $(FILE) $(POLICY) 1>sched-run.txt
# 	cat *.txt

# run2:
# 	bin/sched-simulator $(FILE2) $(POLICY) 1>sched-run2.txt
# 	cat *.txt

runrr:
	bin/sched-simulator $(FILE) $(POLICYRR) 1>sched-run-RR.txt
	cat *.txt	

runmfq:
	bin/sched-simulator $(FILE) $(POLICYMFQ) 1>sched-run-MFQ.txt
	cat *.txt

runiorr:
	bin/sched-simulator $(FILEY) $(POLICYIORR) 1>sched-run-IORR.txt
	cat *.txt

clean:
	rm -rf bin obj sched-run.txt sched-run2.txt sched-run-RR.txt sched-run-MFQ.txt sched-run-IORR.txt
