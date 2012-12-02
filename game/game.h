#include<stdlib.h>
#include<stdio.h>
#include "player.h"

typedef struct game{
    Player *p[4];               /* p[0] is user and rest are AI */
    Deck* deck;
    void (*start_round)(struct game* g);
    void (*discard_round)(struct game* g);
    void (*show_results)(struct game* g);
}Game;

void game_init(Game* game);
void game_deal(Deck* deck, Player* player);
void game_start(Game* game);
void game_discard(Game* game);
void game_show(Game* game);

