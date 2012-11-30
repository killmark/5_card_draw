driver: queue.o list.o deck.o player.o type.o driver.o
	gcc -o driver queue.o list.o deck.o player.o type.o driver.o

driver.o: driver.c player.h ../deck/deck.h ../queue/queue.h ../list/list.h
	gcc -g -c driver.c -I../queue -I../list -I../deck

player.o: player.c player.h ../deck/deck.h ../queue/queue.h ../list/list.h
	gcc -c player.c -I../queue -I../list -I../deck

type.o: type.c type.h
	gcc -c type.c

deck.o: ../deck/deck.c ../deck/deck.h ../queue/queue.h ../list/list.h
	gcc -c ../deck/deck.c -I../queue -I../list -I../deck

queue.o: ../queue/queue.c ../queue/queue.h ../list/list.h 
	gcc -c ../queue/queue.c -I../queue -I../list

list.o: ../list/list.c ../list/list.h
	gcc -c ../list/list.c -I../list

clean:
	rm -f *.o test *~


