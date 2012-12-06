#include<stdlib.h>
#include<stdio.h>
#include "player.h"

typedef struct game{
    Player *p[4];               /* p[0] is user and rest are AI */
    Deck* deck;
    int pot;
    int currentCall;
    void (*first_round)(struct game* g);
    void (*discard_round)(struct game* g);
    void (*show_results)(struct game* g);
    void (*blind_betting)(struct game* g);
    void (*reg_betting)(struct game* g);
}Game;

void game_init(Game* game);
void game_deal(Deck* deck, Player* player);
void game_first(Game* game);
void game_discard(Game* game);
void game_start(Game* game);
void game_show(Game* game);
void game_blind_bet(Game* game);
void game_reg_bet(Game* game);
void game_welcome(Game* game);
void game_menu(Game* game);
void game_info();
int check_end(Game* game);
void printStatus(Game* game, int s);
void user_decision(Game* game);
void reset(Game* game);
void hint(int val);
void user_discard(Game* game);
void confirm();
int check_finish(Game* game);
