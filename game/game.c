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
    game->p[0]->is_AI = 0; /* default: player 0 is user. */
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
    int val = 0;
    val = game->p[0]->discard(game->deck, game->p[0]);
    game->p[0]->browseHands(game->p[0]);
    hint(val);
    user_discard(game);
    game->p[0]->browseHands(game->p[0]);
    for (i = 1; i < 4; i++){
        val = game->p[i]->discard(game->deck, game->p[i]);
        game->p[i]->browseHands(game->p[i]);
    }
}
void user_discard(Game* game){
    int a[5] = {0};
    int i;
    int opt;
    char x[100];
    Card* tmp;

    while(1){
        printf ("Input one card to discard:");
        scanf("%s", x);
        if(!strcmp(x,"Y") | !strcmp(x,"y")){
            break;
        }
        if(!strcmp(x,"N") | !strcmp(x,"n")){
            for (i = 0; i < 5; i++){
                a[i] = 0;
            }
        }
        else{
            opt = atoi(x);
            if(opt >= 1 && opt <= 5){
                if(a[opt - 1] == 0){
                    a[opt - 1] = 1;
                }
                else{
                    printf ("You have chosen this card.\n");
                }
            }
        }
    }
    tmp = (Card*) malloc (sizeof(Card));
    for (i = 0; i < 5; i++){
        if(a[i] == 1){
            deck_pop(game->deck, (const void**) &tmp);
            game->p[0]->cards[i] = tmp->val;
        }
    }
}
void hint(val){
    int i;
    printf ("You are luck, I will give you some advice: \n");
    if(val == 0){
        printf ("You have a good hand. Just keep it.\n");
    }
    else{
        printf ("You should discard Card ");
        for (i = 0; i < 5; i++){
            if(val%2 == 1){
                printf ("%d ",i+1);
            }
            val = val/2;
        }
    }
    printf ("\n");
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
    char x[100];
    int opt;
    
    /* if no check it is correct. */
    do{
        user_decision(game);
        for (i = 1; i < 4; i++){
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

        /* first betting */
        printf ("First Betting: \n");
        game->reg_betting(game);
        printf ("*************************\n");

        /* discard */
        printf ("Discard Round\n");
        game->discard_round(game);
        game->p[0]->showHands(game->p[0]);

        reset(game);

        
        /* /\* finish *\/ */
        /* printf ("Continue? Mr 47 (Any key/N)\n"); */
        /* scanf ("%s", x); */
        /* if(!strcmp(x, "N") | !strcmp(x,"n")){ */
        /*     break; */
        /* } */
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
void user_decision(Game* game){
    char x[100];
    int opt;
    
    if(!game->p[0]->is_fold){
        if(game->currentCall == 0){
            printf ("1. Fold\n2. Check\n3. Bet 2\n");
            printf ("Your option?\n");
            scanf ("%s", x);
            opt = atoi(x);
            switch(opt){
            case 1:
                game->p[0]->fold(game->p[0]);
                printf ("Mr 47\n");
                break;
            case 2:
                game->p[0]->check(game->p[0]);
                printf ("Mr 47\n");
                break;
            case 3:
                game->p[0]->bet(game->p[0], &(game->currentCall));
                printf ("Mr 47\n");
                break;
            default:
                user_decision(game);
                break;
            }
        }
        else{
            printf ("1. Fold\n2. Call %d\n3. Raise %d\n", game->currentCall, game->currentCall);
            printf ("Your option?\n");
            scanf ("%s", x);
            opt = atoi(x);
            switch(opt){
            case 1:
                game->p[0]->fold(game->p[0]);
                printf ("Mr 47\n");
                break;
            case 2:
                game->p[0]->call(game->p[0], &game->currentCall);
                printf ("Mr 47\n");
                break;
            case 3:
                game->p[0]->raise(game->p[0], &(game->currentCall));
                printf ("Mr 47\n");
                break;
            default:
                user_decision(game);
                break;
            }
        }
    }
}
void reset(Game* game){
    int i;
    Deck* tmp = game->deck;
    free(tmp);
    game->deck = (Deck*) malloc (sizeof(Deck));
    deck_shuffle(game->deck);
    for (i = 0; i < 4; i++){
        game->p[i]->is_fold = 0;
        game->p[i]->is_finish = 0;
        game->p[i]->current_betting = 0;
    }
}
