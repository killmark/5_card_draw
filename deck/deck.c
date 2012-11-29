#include<stdlib.h>
#include <time.h>
#include "deck.h"

void deck_shuffle(Deck *deck){
    int wDeck[4][13] = {0};
    int row, col, card;
    Card *tmp;
    
    srand(time(0));
    deck_init(deck, destroy);
    
    for (card = 0; card < 52; card++){
        do
        {
            row = rand()%4;
            col = rand()%13;
        } while (wDeck[row][col] != 0);
        wDeck[row][col] = card;
    }


    for (row = 0; row < 4; row++){
        for (col = 0; col < 13; col++){
            tmp = (Card*) malloc(sizeof(Card));
            tmp->val = wDeck[row][col];
            deck_push(deck, (const void*) tmp);
        }
    }
}
