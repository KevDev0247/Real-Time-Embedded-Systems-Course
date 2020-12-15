#CFLAGS = -DDEBUG
CFLAGS =

beowulfpc: multiple_threads.c
	gcc $(CFLAGS) -o multiple_threads  multiple_threads.c -lrt -pthread

clean:
	rm -f multiple_threads
