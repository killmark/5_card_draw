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
    p1 = (Player*) malloc (sizeof(Player));
    player_init(p1);
    /* p1->cards[0] = 0; */
    /* p1->cards[1] = 3; */
    /* p1->cards[2] = 17; */
    /* p1->cards[3] = 16; */
    /* p1->cards[4] = 19; */
    
    deal(deck, p1);

    p1->browseHands(p1);
    p1->showHands(p1);
    printf ("%d\n",p1->hand_value);
    
    p1->discard(deck, p1);
    
    printf ("****************************\n");
    
    p1->browseHands(p1);
    p1->showHands(p1);
    printf ("%d\n",p1->hand_value);

    /* deck_destroy(deck); */
    /* free(p1); */
    return 0;
}

