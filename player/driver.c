#include<stdlib.h>
#include<stdio.h>
#include "player.h"

int main(){
    Deck* deck;
    Card* tmp;
    Player* p1;
    int i;
    int val;
    
    deck = (Deck*) malloc (sizeof(Deck));
    deck_shuffle(deck);

    p1 = (Player*) malloc (sizeof(Player));

    /* deal(deck, p1); */

    /* for (i = 0; i < 5; i++){ */
    /*     val = p1->cards[i]; */
    /*     printf ("%s %s\n",Suit[val/13], Face[val%13]); */
    /* } */

    /* printf ("***************************\n"); */
    /* sort(p1); */
    
    /* for (i = 0; i < 5; i++){ */
    /*     val = p1->cards[i]; */
    /*     printf ("%s %s\n",Suit[val/13], Face[val%13]); */
    /* } */
    
    /* test flush, straight, SF, RoyalSF */
    p1->cards[0] = 0;
    p1->cards[1] = 1;
    p1->cards[2] = 2;
    p1->cards[3] = 3;
    p1->cards[4] = 9;
    
    cal_value(p1);
    printHand(p1);
    printf ("%d\n",p1->hand_value);
    
    return 0;
}

