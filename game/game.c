#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <time.h>
#include "game.h"

void game_deal(Deck* deck, Player* player){
    
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
    /* deal the cards */
    int i;
    for (i = 0; i < 4; i++){
        game_deal(game->deck, game->p[i]);
        game->p[i]->browseHands(game->p[i]);
    }
}
void game_discard(Game* game){
    int i;
    int val = 0;
    int ret;
    if(no_others(game, -1))
        return;
    printf ("Input the card number to discard one card each time.\n");
    printf ("-------------------------\n");
    printf ("1-5 to discard card by number\n");
    printf ("Y/y to finish or skip discarding\n");
    printf ("N/n to cancel previous discarding\n");
    printf ("-------------------------\n");
    if(!game->p[0]->is_fold){
        val = game->p[0]->discard(game->deck, game->p[0]);
        game->p[0]->browseHands(game->p[0]);
        hint(val);
        user_discard(game);
        game->p[0]->browseHands(game->p[0]);
    }
    
    for (i = 1; i < 4; i++){
        ret = 0;
        if (!game->p[i]->is_fold){
            val = game->p[i]->discard(game->deck, game->p[i]);
            game->p[i]->browseHands(game->p[i]);
            while(val){
                if(val%2 == 1)
                    ret++;
                val = val/2;
            }
            printf ("-------------------------\n");
            printf ("Player %d discards %d cards.\n", i, ret);
        }
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
            printf ("\n-------Waiting for other players-------\n");
            break;
        }
        if(!strcmp(x,"N") | !strcmp(x,"n")){
            printf ("\n-------You have cancelled previous discarding.-------\n");
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
                    printf ("\n-------You have already chosen this card.-------\n");
                }
            }
            else{
                printf ("\n--------- Y/y, N/n or 1-5.--------\n");            }
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
    if(val == 0){
        printf ("You have a good hand. Just keep it.\n");
    }
    else{
        printf ("You may discard Card ");
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
    printf ("--------------Pot: %d chips----------------\n", game->pot);
    if(!game->p[0]->is_fold){
        maxV = game->p[0]->hand_value;
        tmp = 0;
        printf ("Mr 47:\n");
        game->p[0]->showHands(game->p[0]);
    }

    for (i = 1; i < 4; i++){
        if(!game->p[i]->is_fold){
            printf ("Player %d:\n",i);
            game->p[i]->showHands(game->p[i]);
            if(game->p[i]->hand_value > maxV){
                maxV = game->p[i]->hand_value;
                tmp = i;
            }
        }
    }
    
    game->p[tmp]->chips += game->pot;
    
    if(tmp != 0){
        printf ("The winner is Player %d\n",tmp);
        printf ("-------------------------\n");
    }
    else{
        printf ("The winner is Mr 47.\n");
        printf ("-------------------------\n");
    }
}
void game_blind_bet(Game* game){
    int i;
    /* Ante betting part */
    for (i = 0; i < 4; i++){
        if(game->p[i]->chips >= 3){
            game->p[i]->chips --;
            game->pot ++;
        }
        else{
            game->p[i]->is_fold = 1;
        }
    }
}
void game_reg_bet(Game* game){
    /* regular betting */
    int i;
    int r = 0;
    char x[100];
    int opt;
    int fn;
    
    /* if the game is finish, don't need to betting again */
    for (i = 0; i < 4; i++){
        if(game->p[i]->is_finish)
            return;
    }

    do{
        user_decision(game);
        if(game->p[0]->is_finish == 1){
            break;
        }
        for (i = 1; i < 4; i++){
            if(no_others(game, i)){
                game->p[i]->is_finish = 1;
                break;
            }
            if(!game->p[i]->is_fold){
                if(game->currentCall == 0 | game->currentCall != game->p[i]->current_betting){
                    printf ("-------------------------\n");
                    printf ("Player %d\t",i);
                }
            }
            game->p[i]->make_decision(game->p[i], &(game->currentCall));
            if(game->p[i]->is_finish == 1){
                break;
            }
        }
    }while(check_end(game));
    /* renew the pot and play's chips */
    for (i = 0; i < 4; i++){
        game->p[i]->chips -= game->p[i]->current_betting;
        game->pot += game->p[i]->current_betting;
        game->p[i]->current_betting = 0;
        game->p[i]->is_finish = 0;
        game->currentCall = 0;
    }
}
int check_end(Game* game){
    /* whether the betting is finished */
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
    int i;

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
        printf ("2. Get More Chips\n");
        printf ("3. Help\n");
        printf ("4. Quit\n");
        scanf ("%s", x);
        opt = atoi(x);
        switch(opt){
        case 1:
            system("clear");
            game_start(game);
            break;
        case 2:
            system("clear");
            printf ("I am kidding.\n\n");
            confirm();
            break;
        case 3:
            system("clear");
            help();
            confirm();
            break;
        case 4:
            exit(0);
        default:
            system("clear");
            printf ("I hope you know Arabic numbers.\n\n");
        }
    }
}
void help(){
    /* printf game_guide to console */
    FILE* fd;
    char x[100];
    fd = fopen("../game_guide","r");  
    if(fd == NULL){  
        printf("read file error!");  
    }
    else{
        while(!feof(fd)){  
            fgets(x,100,fd);
            printf("%s",x);
        }
        fclose(fd);
    }
}
void game_start(Game* game){
    char x[100];
    
    system("clear");
    printf ("*******5 Card Draw*******\n");
    printf ("Enjoy yourself, Mr 47\n");
    confirm();
    
    while(1){

        if(check_finish(game)){
            confirm();
            break;
        }
        
        /* current status */
        printf ("Hi everyone,\n");
        printf ("We have 4 players on the table:\n");
        printStatus(game, 1);

        confirm();

        /* Ante*/
        printf ("*******Ante*******\n");
        printf ("Each player must bet 1 chip in this round.\n");
        game->blind_betting(game);
        printf ("-------------------------\n");
        printf ("------Pot: %d chips-------\n",game->pot);
        printStatus(game, 1);
        confirm();
        
        /* first round */
        printf ("Dealing...\n");
        game->first_round(game);
        printStatus(game, 2);
        confirm();

        /* first betting */
        printf ("*******First Betting*******\n");
        printf ("\n---------Your chips: %d --------\n", game->p[0]->chips);
        printStatus(game, 2);
        game->reg_betting(game);
        printf ("-------------------------\n");
        confirm();

        /* discard */
        if(!no_others(game, -1)){
            printf ("*******Discard Round*******\n");
            printStatus(game, 2);
            game->discard_round(game);
            printStatus(game, 2);
            confirm();
        }

        /* second betting */
        if(!no_others(game, -1)){
            printf ("*******Second Betting*******\n");
            printf ("\n---------Your chips: %d --------\n", game->p[0]->chips);
            printStatus(game, 2);
            game->reg_betting(game);
            printf ("-------------------------\n");
            confirm();
        }

        /* show results */
        game->show_results(game);

        reset(game);
        
        /* /\* finish *\/ */
        printf ("Continue? Mr 47 (Any key/N)\n");
        scanf ("%s", x);
        if(!strcmp(x, "N") | !strcmp(x,"n")){
            /* if not continue, we should restart the game */
            restart(game);
            break;
        }
        else{
            system("clear");
        }
    }
}
void printStatus(Game* game, int s){
    /* s = 1: only print chips and pot information */
    /* s =2: print hands information of player */
    int i;
    char x[100];
    if(s == 1){
        if(!game->p[0]->is_fold){
            printf ("-------------------------\n");
            printf ("Mr 47 has %d chips.\n",game->p[0]->chips);
        }
        for (i = 1; i < 4; i++){
            if(!game->p[i]->is_fold){
                printf ("-------------------------\n");
                printf ("Player %d has %d chips.\n", i, game->p[i]->chips);
            }
        }
        printf ("-------------------------\n");
    }
    else{
        printf ("\n");
        if(!game->p[0]->is_fold){
            game->p[0]->showHands(game->p[0]);
        }
    }
}
int no_others(Game* game, int k){
    int i;
    int fn;
    fn = 0;
    /* If 3 players fold. The game will be end. */
    for (i = 0; i < 4; i++){
        if(game->p[i]->is_fold && i != k){
            fn++;
        }
    }
    if(fn == 3)
        return 1;
    else
        return 0;

}
void user_decision(Game* game){
    char x[100];
    int opt;
    int ret;
    /* betting part for user */
    ret = no_others(game, 0);
    if(ret){
        game->p[0]->is_finish;
        return;
    }
    if(!game->p[0]->is_fold){
        if(game->currentCall == 0){
            printf ("-------------------------\n");
            printf ("1. Fold\n2. Check\n3. Bet 2\n");
            printf ("Your option?\n");
            scanf ("%s", x);
            opt = atoi(x);
            switch(opt){
            case 1:
                printf ("-------------------------\n");
                printf ("Mr 47\t");
                game->p[0]->fold(game->p[0], &(game->currentCall));
                break;
            case 2:
                printf ("-------------------------\n");
                printf ("Mr 47\t");
                game->p[0]->check(game->p[0]);
                break;
            case 3:
                printf ("-------------------------\n");
                printf ("Mr 47\t");
                game->p[0]->bet(game->p[0], &(game->currentCall));
                break;
            default:
                user_decision(game);
                break;
            }
        }
        else{
            printf ("-------------------------\n");
            printf ("1. Fold\n2. Call %d\n3. Raise %d\n", game->currentCall, game->currentCall);
            printf ("Your option?\n");
            scanf ("%s", x);
            opt = atoi(x);
            switch(opt){
            case 1:
                printf ("-------------------------\n");
                printf ("Mr 47\t");
                game->p[0]->fold(game->p[0], &game->currentCall);
                break;
            case 2:
                printf ("-------------------------\n");
                printf ("Mr 47\t");
                game->p[0]->call(game->p[0], &game->currentCall);
                break;
            case 3:
                printf ("-------------------------\n");
                printf ("Mr 47\t");
                game->p[0]->raise(game->p[0], &(game->currentCall));
                break;
            default:
                user_decision(game);
                break;
            }
        }
    }
}
void restart(Game* game){
    /* reset the chips of each player */
    int i;
    for (i = 0; i < 4; i++){
        game->p[i]->chips = CHIPS;
    }
}
void reset(Game* game){
    int i;

    /* after the finish of each set, we should reset the pot and chips */
    game->deck = (Deck*) malloc (sizeof(Deck));
    deck_shuffle(game->deck);
    game->currentCall = 0;
    game->pot = 0;
    
    for (i = 0; i < 4; i++){
        game->p[i]->is_fold = 0;
        game->p[i]->is_finish = 0;
        game->p[i]->current_betting = 0;
    }
}
void confirm(){
    char x[100];
    /* user should confirm after each step */
    printf ("-------------------------\n");
    printf ("Press anything with Enter to continue.\n");
    scanf("%s", x);
    system("clear");
}
int check_finish(Game* game){
    int n;
    int i;
    int pl;
    /*
      If there is only one player on the table, the game should be finished.
      And this player will be final winner.
    */
    n = 0;
    for (i = 0; i < 4; i++){
        if(game->p[i]->chips < 3){
            n++;
        }
        else{
            pl = i;
        }
    }
    if(n == 3){
        printf ("--------------------------------------------\n");
        if(pl == 0)
            printf ("Congratulations Mr 47, ");
        else
            printf ("Congratulations Player %d, ",pl);
        printf ("You beat other three players on this table!\n");
        printf ("--------------------------------------------\n");
        return 1;
    }
    return 0;
}
