#include<stdlib.h>
#include<stdio.h>
#include "deck.h"

typedef struct {
    int cards[5];
    int hand_type;
    int hand_value;
}Player;

void deal(Deck* deck, Player* player);
void cal_value(Player* player);
void sort_by_val(Player* player);
void sort_by_type(Player* player);
int check_type(Player* player);
void swap(Player* player, int a, int b);
void printHand(Player* player);
