CFLAGS=-Iinclude -Wall


run1:
	gcc $(CFLAGS) -c -o obj/upper.o src/upper.c
	gcc $(CFLAGS) -c -o obj/multi-upper.o src/multi-upper.c
	gcc $(CFLAGS) -o bin/multi-upper obj/multi-upper.o obj/upper.o -lrt -lpthread
	bin/multi-upper f1 f2 f3 f4

run2:
	gcc $(CFLAGS) -c -o obj/upper.o src/upper.c
	gcc $(CFLAGS) -c -o obj/multi-upper2.o src/multi-upper2.c
	gcc $(CFLAGS) -o bin/multi-upper2 obj/multi-upper2.o obj/upper.o -lrt -lpthread
	bin/multi-upper2 f1 f2 f3 f4

run3:
	gcc $(CFLAGS) -c -o obj/bounded-buffer.o src/bounded-buffer.c
	gcc $(CFLAGS) -o bin/bounded-buffer obj/bounded-buffer.o -lrt -lpthread
	bin/bounded-buffer


clean:
	rm -f obj/* bin/* *.UPPER.txt