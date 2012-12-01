#include<stdlib.h>
#include<stdio.h>
#include "deck.h"

struct P{
    int cards[5];
    int hand_type;
    int hand_value;
    void (*browseHands)(struct P* pl);
    void (*showHands)(struct P* pl);
    void (*discard)(Deck* deck, struct P* pl);
};

typedef struct P Player;

void player_init(Player* player);
void deal(Deck* deck, Player* player);
void cal_value(Player* player);
void sort_by_val(Player* player);
void sort_by_type(Player* player);
int check_type(Player* player);
void swap(Player* player, int a, int b);
void printHand(Player* player);
void discardHand(Deck* deck, Player* player);
int MC(Player* player);
