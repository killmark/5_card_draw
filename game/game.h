#include<stdlib.h>
#include<stdio.h>
#include "player.h"

typedef struct game{
    Player *p[4];               /* p[0] is user and rest are AI */
    Deck* deck;                 /* deck */
    int pot;                    /* pot */
    int currentCall;            /* current max call of player, each player on the table should
                                   bet this number of chips */
    void (*first_round)(struct game* g);   /* point to game_first */
    void (*discard_round)(struct game* g); /* point to game_discard */
    void (*show_results)(struct game* g);  /* point to game_show */
    void (*blind_betting)(struct game* g); /* point to game_blind_bet */
    void (*reg_betting)(struct game* g);   /* point to game_reg_bet */
}Game;

void game_init(Game* game);     /* init game */
void game_deal(Deck* deck, Player* player); /* deal the cards to player */
void game_first(Game* game); /* dealing round*/
void game_discard(Game* game);  /* discarding round */
void game_start(Game* game);    /* start the game */
void game_show(Game* game);     /* show the results of each set */
void game_blind_bet(Game* game); /* Ante betting round */
void game_reg_bet(Game* game);   /* first and second betting round */
void game_welcome(Game* game);   /* welcome menu */
void game_menu(Game* game);      /* main menu */
int check_end(Game* game);       /* check whether the betting round is finished */
void printStatus(Game* game, int s); /* print the game status(chips, pots, and user hands) */
void user_decision(Game* game);      /* betting decision for users */
void reset(Game* game);              /* reset the game status for next set */
void hint(int val);                  /* run MC for user to give him some hint */
void user_discard(Game* game);       /* discarding decision for users */
void confirm();                      /* waiting for user to continue */
int check_finish(Game* game);        /* check whether one set is finish */
void help();                         /* show the help information */
void restart();                      /* restart the game */
