test: queue.o list.o test.o
	gcc -o test test.o queue.o list.o 

test.o: test.c queue.h ../list/list.h
	gcc -c test.c -I../list

queue.o: queue.c queue.h ../list/list.h
	gcc -c queue.c -I../list

list.o: ../list/list.c ../list/list.h
	gcc -c ../list/list.c -I../list

clean:
	rm -f *.o main *~
