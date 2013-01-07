#include<stdlib.h>
#include<stdio.h>
#include "deck.h"

struct P{
    int cards[5];
    int chips;                  /* player's chips */
    int is_fold;                /* whether the player is fold in betting round */
    int current_betting;        /* player's current betting number */
    int hand_type;              /* type of current hand */
    int hand_value;             /* value of current hand */
    int is_finish;              /* to show whether the betting is finished */
    int is_AI;                  /* to show whether is AI or User */
    void (*browseHands)(struct P* pl); /* point to cal_value */
    void (*showHands)(struct P* pl);   /* point to printHand */
    int (*discard)(Deck* deck, struct P* pl); /* point to discardHand */
    void (*make_decision)(struct P* pl, int* curC); /* point to player_decision */
    void (*check)(struct P* pl);                    /* point to player_check */
    void (*bet)(struct P* pl, int* n);              /* point to player_bet  */
    void (*call)(struct P* pl, int* n);             /* point to player_call */
    void (*raise)(struct P* pl, int* n);            /* point to player_raise */
    void (*fold)(struct P* pl, int* n);             /* point to player_fold */
};

typedef struct P Player;
#define CHIPS 50;               /* chips for each player at the starting of game */
#define NUMR 32;                /* the max betting chips */
#define N 10000;                /* each case will be simulated N times */

void player_init(Player* player);        /* init player */
void cal_value(Player* player); /* calculate hand value */
void sort_by_val(Player* player); /* sort hand by face value */
void sort_by_type(Player* player); /* sort hand by type */
int check_type(Player* player);    /* check the hand type */
void swap(Player* player, int a, int b); /* swap two cards in hand */
void printHand(Player* player);          /* print hand info */
int discardHand(Deck* deck, Player* player); /* discard cards in hand */
int MC(Deck* deckForMC, Player* player);     /* MC simulation */
void player_check(Player* player);           /* check */
void player_call(Player* player, int* n);    /* call */
void player_raise(Player* player, int* n);   /* raise */
void player_fold(Player* player, int* n);    /* fold */
void player_bet(Player* player, int* n);     /* bet */
void player_decision(Player* player, int* curC); /* AI decision of betting. */
void do_decision(Player* player, int* curC, int check, int fold, int call); /* do decision according to hand type */
