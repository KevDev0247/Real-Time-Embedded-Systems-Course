#CFLAGS = -DDEBUG
CFLAGS =

beowulfpc: simple_thread_affinity.c
	gcc $(CFLAGS) -o simple_thread_affinity  simple_thread_affinity.c -lrt -pthread

clean:
	rm -f simple_thread_affinity
