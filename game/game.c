#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <time.h>
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
    game->first_round = game_first;
    game->discard_round = game_discard;
    game->show_results = game_show;
    game->blind_betting = game_blind_bet;
    game->reg_betting = game_reg_bet;
    game->pot = 0;
    game->currentCall = 0;
}
void game_first(Game* game){
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

void game_welcome(Game* game){
    char x[100];

    srand(time(NULL));
    system("clear");
    printf ("*******Welcome Noob!!!*******\n");
    printf ("What's your name?\n");
    scanf("%s", x);
    system("clear");
    printf ("Actually I don't care. I will call you Mr 47.\n");
    while(1){
        printf ("Are you ready? Mr 47 (Y/N)\n");
        scanf ("%s", x);
        if(!strcmp(x, "Y") | !strcmp(x,"y")){
            system("clear");
            game_menu(game);
            break;
        }
        else{
            if(!strcmp(x, "N") | !strcmp(x,"n")){
                exit(0);
            }
            else{
                if(rand()%2){
                    system("clear");
                    printf ("What the hell are you inputing?\n");
                }
                else{
                    system("clear");
                    printf ("Come on. Don't embarrass me.\n");
                }
            }
        }
    }
}
void game_menu(Game* game){
    char x[100];
    int opt;

    while(1){
        system("clear");
        printf ("*******Main Menu*******\n");
        printf ("1. Start Game\n");
        printf ("2. Personal Info\n");
        printf ("3. Get More Chips\n");
        printf ("4. Help\n");
        printf ("5. Quit\n");
        scanf ("%s", x);
        opt = atoi(x);
        switch(opt){
        case 1:
            system("clear");
            game_start(game);
            break;
        case 2:
            system("clear");
            printf ("2\n");
            break;
        case 3:
            system("clear");
            printf ("I am kidding.\n\n");
            break;
        case 4:
            system("clear");
            printf ("rules\n\n");
            break;
        case 5:
            exit(0);
        default:
            system("clear");
            printf ("I hope you know Arabic numbers.\n\n");
        }
    }
}
void game_start(Game* game){
    char x[100];
    while(1){
        system("clear");
        printf ("*******5 Card Draw*******\n");
        /* blind betting */
        printf ("Blind Betting:\n");
        printStatus(game, 1);
        printf ("\nEach player must bet 1 chip in this round.\n");
        game->blind_betting(game);
        printf ("*************************\n");
        
        /* first round */
        printf ("First Round:\n");
        printStatus(game, 1);
        game->first_round(game);
        printStatus(game, 2);
        /* game->reg_betting(game); */
        printf ("*************************\n");


        
        /* finish */
        printf ("Continue? Mr 47 (Any key/N)\n");
        scanf ("%s", x);
        if(!strcmp(x, "N") | !strcmp(x,"n")){
            break;
        }
    }
}
void printStatus(Game* game, int s){
    /* s = 1: blind betting */
    int i;
    char x[100];
    if(s == 1){
        printf ("Game Status:\n");
        printf ("Pot: %d\n",game->pot);
        if(!game->p[0]->is_fold){
            printf ("Mr 47 has %d chips.\n",game->p[0]->chips);
        }
        for (i = 1; i < 4; i++){
            if(!game->p[i]->is_fold){
                printf ("Player %d has %d chips.\n", i, game->p[i]->chips);
            }
        }
    }
    else{
        printf ("\n");
        if(!game->p[0]->is_fold){
            game->p[0]->showHands(game->p[0]);
        }
    }
}
