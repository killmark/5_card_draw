#include<stdlib.h>
#include<stdio.h>
#include "game.h"

void game_deal(Deck* deck, Player* player){
    /* should be put in other files */
    Card* tmp;
    int i;
    for (i = 0; i < 5; i++){
        tmp = (Card*) malloc (sizeof(Card));
        deck_pop(deck, (const void**) &tmp);
        player->cards[i] = tmp->val;
    }
}
void game_init(Game* game){
    int i;
    game->deck = (Deck*) malloc (sizeof(Deck));
    deck_shuffle(game->deck);
    for (i = 0; i < 4; i++){
        game->p[i] = (Player*) malloc (sizeof(Player));
        player_init(game->p[i]);
    }
    game->start_round = game_start;
    game->discard_round = game_discard;
    game->show_results = game_show;
}
void game_start(Game* game){
    int i;
    for (i = 0; i < 4; i++){
        game_deal(game->deck, game->p[i]);
        game->p[i]->browseHands(game->p[i]);
    }
}
void game_discard(Game* game){
    int i;
    for (i = 0; i < 4; i++){
        game->p[i]->discard(game->deck, game->p[i]);
        game->p[i]->browseHands(game->p[i]);
    }
}
void game_show(Game* game){
    int i;
    int maxV,tmp;
    maxV = 0;
    tmp = 0;
    for (i = 0; i < 4; i++){
        printf ("Player %d:\n",i);
        game->p[i]->showHands(game->p[i]);
        if(game->p[i]->hand_value > maxV){
            maxV = game->p[i]->hand_value;
            tmp = i;
        }
    }
    printf ("the winner is Player %d\n",tmp);
}
