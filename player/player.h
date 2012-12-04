#include<stdlib.h>
#include<stdio.h>
#include "deck.h"

struct P{
    int cards[5];
    int chips;
    int is_fold;
    int current_betting;
    int hand_type;
    int hand_value;
    int is_finish;
    int is_AI;
    void (*browseHands)(struct P* pl);
    void (*showHands)(struct P* pl);
    int (*discard)(Deck* deck, struct P* pl);
    void (*make_decision)(struct P* pl, int* curC);
    void (*check)(struct P* pl);
    void (*bet)(struct P* pl, int* n);
    void (*call)(struct P* pl, int* n);
    void (*raise)(struct P* pl, int* n);
    void (*fold)(struct P* pl);
};

typedef struct P Player;

void player_init(Player* player);
void cal_value(Player* player);
void sort_by_val(Player* player);
void sort_by_type(Player* player);
int check_type(Player* player);
void swap(Player* player, int a, int b);
void printHand(Player* player);
int discardHand(Deck* deck, Player* player);
int MC(Deck* deckForMC, Player* player);
void player_check(Player* player);
void player_call(Player* player, int* n);
void player_raise(Player* player, int* n);
void player_fold(Player* player);
void player_bet(Player* player, int* n);
void player_decision(Player* player, int* curC);
void do_decision(Player* player, int* curC, int check, int fold, int call);
