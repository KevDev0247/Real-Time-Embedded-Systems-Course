#CFLAGS = -DDEBUG
CFLAGS =

beowulfpc: processes_sync.c
	gcc $(CFLAGS) -o processes_sync  processes_sync.c -lrt -pthread

clean:
	rm -f processes_sync
