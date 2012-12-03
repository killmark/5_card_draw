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
    game->blind_betting = game_blind_bet;
    game->reg_betting = game_reg_bet;
    game->pot = 0;
    game->currentCall = 0;
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
    game->p[tmp]->chips += game->pot;
    printf ("the winner is Player %d\n",tmp);
    for (i = 0; i < 4; i++){
        printf ("player %d chips: %d\n", i, game->p[i]->chips);
    }
}
void game_blind_bet(Game* game){
    int i;
    for (i = 0; i < 4; i++){
        game->p[i]->chips --;
        game->pot ++;
    }
}
void game_reg_bet(Game* game){
    int i;
    int r = 0;
    /* if no check it is correct. */
    do{
        for (i = 0; i < 4; i++){
            game->p[i]->make_decision(game->p[i], &(game->currentCall));
            if(game->p[i]->is_finish == 1){
                break;
            }
            else{
                printf ("player %d\n",i);
            }
        }
        printf ("--------------------\n");
    }while(check_end(game));
    
    for (i = 0; i < 4; i++){
        game->p[i]->chips -= game->p[i]->current_betting;
        game->pot += game->p[i]->current_betting;
        game->p[i]->current_betting = 0;
        game->p[i]->is_finish = 0;
        game->currentCall = 0;
    }
}
int check_end(Game* game){
    int i;
    int bet;
    for (i = 0; i < 4; i++){
        if(!game->p[i]->is_fold){
            bet = game->p[i]->current_betting;
            break;
        }
    }
    for (i = 0; i < 4; i++){
        if(!game->p[i]->is_fold){
            if(bet != game->p[i]->current_betting)
                return 1;
        }
    }
    return 0;
}

