main: game.o queue.o list.o deck.o player.o type.o main.o
	gcc -o main queue.o list.o deck.o player.o type.o game.o main.o

main.o: main.c game.h ../player/player.h ../deck/deck.h ../queue/queue.h ../list/list.h
	gcc -c main.c -I../queue -I../list -I../deck -I../player

game.o: game.c game.h ../player/player.h ../deck/deck.h ../queue/queue.h ../list/list.h
	gcc -c game.c -I../queue -I../list -I../deck -I../player

player.o: ../player/player.c ../player/player.h ../deck/deck.h ../queue/queue.h ../list/list.h
	gcc -c ../player/player.c -I../queue -I../list -I../deck -I../player

type.o: ../player/type.c ../player/type.h
	gcc -c ../player/type.c -I../player

deck.o: ../deck/deck.c ../deck/deck.h ../queue/queue.h ../list/list.h
	gcc -c ../deck/deck.c -I../queue -I../list -I../deck

queue.o: ../queue/queue.c ../queue/queue.h ../list/list.h 
	gcc -c ../queue/queue.c -I../queue -I../list

list.o: ../list/list.c ../list/list.h
	gcc -c ../list/list.c -I../list

clean:
	rm -f *.o test *~
