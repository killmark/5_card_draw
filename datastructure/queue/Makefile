test: queue.o list.o test.o
	gcc -o test test.o queue.o ../list/list.o 

queue.o: queue.c queue.h
	gcc -c queue.c -I. -I../list

main.o: main.c queue.h
	gcc -c main.c -I. -I../list

list.o: ../list/list.c ../list/list.h
	gcc -c ../list/list.c -I.

clean:
	rm -f *.o main *~
