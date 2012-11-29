#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef Queue Deck;

typedef struct{
    int val;
    int type;
}Card;

#define deck_init queue_init
#define deck_destroy queue_destroy
#define deck_size queue_size
#define deck_print queue_print
#define deck_push queue_enqueue
#define deck_pop  queue_dequeue
void deck_shuffle(Deck* deck);



