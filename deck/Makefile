test: queue.o list.o deck.o test.o
	gcc -o test test.o deck.o queue.o list.o 

test.o: test.c deck.h ../queue/queue.h ../list/list.h
	gcc -c test.c -I../list -I../queue

deck.o: deck.c deck.h ../queue/queue.h ../list/list.h
	gcc -c deck.c -I../queue -I../list

queue.o: ../queue/queue.c ../queue/queue.h ../list/list.h 
	gcc -c ../queue/queue.c -I../queue -I../list

list.o: ../list/list.c ../list/list.h
	gcc -c ../list/list.c -I../list

clean:
	rm -f *.o main *~


