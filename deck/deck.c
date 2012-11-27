#include<stdlib.h>
#include <time.h>
#include "deck.h"

char *Suit[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};
char *Face[13] = {"Ace", "Deuce", "Three", "Four",
                        "Five", "Six", "Seven", "Eight", "Nine",
                        "Ten", "Jack", "Queen", "King"};

void deck_shuffle(Deck *deck){
    int wDeck[4][13] = {0};
    int row, col, card;
    Card *tmp;
    
    srand(time(0));
    deck_init(deck, destroy);
    
    for (card = 1; card <= 52; card++){
        do
        {
            row = rand()%4;
            col = rand()%13;
        } while (wDeck[row][col] != 0);
        wDeck[row][col] = card;
    }

    for (card = 1; card <= 52; card++){
        for (row = 0; row < 4; row++){
            for (col = 0; col < 13; col++){
                if (wDeck[row][col] == card){
                    tmp = (Card*) malloc(sizeof(Card));
                    tmp->suit = Suit[row];
                    tmp->face = Face[col];
                    deck_push(deck, (const void*) tmp);
                }
            }
        }
    }
    printf ("%d\n", deck->size);
    deck_pop(deck, (const void**) &tmp);
    printf ("%s %s\n",tmp->suit, tmp->face);
    printf ("%d\n", deck->size);
}
