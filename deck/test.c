#include<stdlib.h>
#include<stdio.h>
#include "deck.h"

int main(){
    Deck *deck;
    deck = (Deck*)malloc(sizeof(Deck));
    deck_shuffle(deck);
    return 0;
}


