#CFLAGS = -DDEBUG
CFLAGS =

beowulfpc: rt_thread.c
	gcc $(CFLAGS) -o rt_thread  rt_thread.c -lrt -pthread

clean:
	rm -f rt_thread
