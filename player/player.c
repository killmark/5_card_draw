#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include "player.h"
#include "type.h"

char *Suit[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};
char *Face[13] = {"2", "3", "4",
                  "5", "6", "7", "8", "9",
                  "10", "J", "Q", "K", "A"};
char *type[10] = {"High Card", "One Pair", "Two Pair", "Three of A Kind",
                  "Straight", "Flush", "Full House", "Four Of A Kind", "Straight Flush",
                  "Royal Straight Flush"};

void player_init(Player* player){
    player->chips = 100;
    player->is_fold = 0;
    player->current_betting = 0;
    player->is_finish = 0;
    player->is_AI = 1;
    player->browseHands = cal_value;
    player->showHands = printHand;
    player->discard = discardHand;
    player->make_decision = player_decision;
    player->bet = player_bet;
    player->check = player_check;
    player->call = player_call;
    player->fold = player_fold;
    player->raise = player_raise;
}

void cal_value(Player* player){
    int val;
    int tmp;
    int tmp2;
    int i;
    /*
      type = check_type(player);
      sort(player); according type;
      now assuming it is 0.
    */

    player->hand_type = check_type(player);

    sort_by_type(player);

    /* first byte is not used, set 0 */
    val = 0;

    /* set type */
    val |= player->hand_type << 20;

    /* set cards */
    
    tmp2 = 16;
    for (i = 0; i < 5; i++, tmp2 -= 4){
        tmp = player->cards[i];
        tmp = tmp%13;
        val |= tmp << tmp2;
    }

    player->hand_value = val;
}
void swap(Player* player, int a, int b){
    int tmp;
    
    tmp = player->cards[a];
    player->cards[a] = player->cards[b];
    player->cards[b] = tmp;
}
void sort_by_val(Player* player){
    int i,j;
    int tmp;
    int f1,f2;
    /* insertion sort */
    for (i = 1; i < 5; i++){
        tmp = player->cards[i];
        j = i;
        while(j > 0 && (player->cards[j-1])%13 < tmp%13){
            player->cards[j] = player->cards[j-1];
            j--;
        }
        player->cards[j] = tmp;
    }
}
void sort_by_type(Player* player){
    int i;
    int tmp;
    int val1, val2;
    /* firstly, sort by value */
    sort_by_val(player);
    
    /* four of kind */
    if(player->hand_type == 7){
        val1 = player->cards[0]%13;
        val2 = player->cards[1]%13;
        if(val1 != val2){
            swap(player, 0, 4);
        }
    }
    /* full house */
    if (player->hand_type == 6){
        val1 = player->cards[1]%13;
        val2 = player->cards[2]%13;
        if(val1 != val2){
            swap(player, 0, 4);
            swap(player, 1, 3);
        }
    }
    /* three kind */
    if(player->hand_type == 3){
        tmp = player->cards[2]%13;
        val1 = player->cards[0]%13;
        val2 = player->cards[1]%13;
        if (val1 != tmp && val2 == tmp){
            swap(player, 0, 3);
            return;
        }
        if (val1 != tmp && val2 != tmp){
            swap(player, 0, 4);
            swap(player, 1, 3);
        }
    }
    /* Two pair */
    if (player->hand_type == 2){
        val1 = player->cards[0]%13;
        val2 = player->cards[1]%13;
        if (val1 != val2){
            swap(player, 0, 2);
            swap(player, 2, 4);
            return;
        }
        val1 = player->cards[2]%13;
        val2 = player->cards[3]%13;
        if (val1 != val2){
            swap(player, 2, 4);            
        }
    }
    /* one pair */
    if (player->hand_type == 1){
        if((player->cards[1] - player->cards[2])%13 == 0){
            swap(player, 0, 2);
            return;
        }
        if((player->cards[2] - player->cards[3])%13 == 0){
            swap(player, 0, 2);
            swap(player, 1, 3);
            return;
        }
        if((player->cards[3] - player->cards[4])%13 == 0){
            swap(player, 2, 4);
            swap(player, 0, 2);
            swap(player, 1, 3);
        }
    }
}
int check_type(Player* player){
    int suit, face;
    int a[4][13] = {0};
    int i,j;

    for (i = 0; i < 5; i++){
        suit = (player->cards[i])/13;
        face = (player->cards[i])%13;
        a[suit][face] = 1;
    }

    if (isStraight(a)){
        if(isFlush(a)){
            if(isRoyalSF(a)){
                return 9;
            }
            else{
                return 8;
            }
        }
        else{
            return 4;
        }
    }
    else{
        if(isFlush(a)){
            return 5;
        }
        else{
            if(isFour(a)){
                return 7;
            }
            else{
                if(isTwoPair(a)){
                    return 2;
                }
                else{
                    if(isThree(a)){
                        if(isOnePair(a))
                            return 6;
                        else
                            return 3;
                    }
                    else{
                        if(isOnePair(a))
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void printHand(Player* player){
    int i;
    int suit;
    int face;
    printf ("--%s\n",type[player->hand_type]);
    for (i = 0; i < 5; i++){
        suit = player->cards[i]/13;
        face = player->cards[i]%13;
        printf ("Card %d:\t %s\t %s\n", i+1, Face[face], Suit[suit]);
    }
    printf ("********************\n");

}

int discardHand( Deck* deck, Player* player){
    int Val, ret;                    /* 0-31 */
    int i,j;
    Card* tmp;
    Deck* deckForMC;
    ListElmt *tmpE, *tmpP;
    Card *tmpC;
    Card **tmp2;
    int num;                    /* the number of total player in game (it can be parameter) */
    
    //printf ("%d\n",deckForMC->size);
    deckForMC = (Deck*) malloc (sizeof(Deck));
    deck_shuffle(deckForMC);
    tmp = (Card*) malloc (sizeof(Card));

    /* remove the  player's hand from deckforMC */
    for (i = 0; i < 5; i++){
        tmpE = deckForMC->head;
        tmpP = NULL;
        for (j = 0; j < deckForMC->size; j++){
            tmpC = (Card*)tmpE->data;
            if(tmpC->val == player->cards[i]){
                list_rem_next(deckForMC, tmpP, (const void**) &tmp);
                
                break;
            }
            else{
                tmpP = tmpE;
                tmpE = tmpE->next;
            }
        }
    }
    
    /* remove other cards. */
    if(deckForMC->size != deck->size){
        while(deckForMC->size != deck->size){
            deck_pop(deckForMC, (const void**) &tmp);
        }
    }
    
    Val = MC(deckForMC, player);
    ret = Val;
    if(player->is_AI){
        for (i = 0; i < 5; i++){
            if(Val%2 == 1){
                deck_pop(deck, (const void**) &tmp);
                player->cards[i] = tmp->val;
            }
            Val = Val/2;
        }
    }
    free(tmp);
    //printf ("%d\n",deckForMC->size);
    printf ("%d\n",ret);
    return ret;
}

int MC(Deck* deckForMC, Player* player){
    /* something wrong in this function to cause aborted (core dumped) */
    int i,j,n,h;
    int k;
    int cVal, tVal, DisVal;
    double MaxHV;
    double sum;
    int pre;
    int index,temp;
    int c[100];
    int p[5];
    Card* tmp;
    ListElmt *tmpE;
    Player* playerT;

    tmp = (Card*) malloc (sizeof(Card));
    playerT = (Player*) malloc (sizeof(Player));
    srand(time(NULL));
    
    MaxHV = (double)player->hand_value;
    DisVal = 0;
    n = 10000;
    sum = 0.0;
    *playerT = *player;
        
    for (cVal = 1; cVal < 32; cVal++){
        for (i = 0; i < n; i++){
            tVal = cVal;
            for (j = 0; j < 5; j++){
                if(tVal%2 == 1){
                    deck_pop(deckForMC, (const void**) &tmp);
                    playerT->cards[j] = tmp->val;
                    deck_push(deckForMC, (const void*) tmp);
                }
                tVal = tVal/2;
            }
            
            cal_value(playerT);
            sum += (double)playerT->hand_value;
            *playerT = *player;
            
            /* scramble the deck */
            k = 0;
            tmpE = deckForMC->head;
            while(tmpE != NULL){
                tmp = (Card*) tmpE->data;
                c[k] = tmp->val;
                tmpE = tmpE->next;
                k++;
            }
            
            /* scramble the array */
            for (k = deckForMC->size - 1; k > 0; k--){
                index = rand() % (k+1);
                temp = c[index];
                c[index] = c[k];
                c[k] = temp;
            }

            k = 0;
            tmpE = deckForMC->head;
            while(tmpE != NULL){
                tmp = (Card*) tmpE->data;
                tmp->val = c[k];
                tmpE = tmpE->next;
                k++;
            }
        }
        sum = sum/(double)n;
        if (sum > MaxHV){
            MaxHV = sum;
            DisVal = cVal;
        }
        sum = 0;
    }
    
    free(playerT);
    free(tmp);
    
    return DisVal; 
}
void player_decision(Player* player, int* curC){
    int a = 100;
    int b = 10;
    int r;
    if(!player->is_fold){
        switch(player->hand_type){
        case 0:
            do_decision(player, curC, 15, 15, 70);
            break;
        case 1:
            do_decision(player, curC, 10, 5, 85);
            break;
        case 2:
            do_decision(player, curC, 10, 0, 60);
            break;
        case 3:
            do_decision(player, curC, 10, 0, 50);
            break;
        case 4:
            do_decision(player, curC, 5, 0, 30);
            break;
        case 5:
            do_decision(player, curC, 3, 0, 25);
            break;
        case 6:
            do_decision(player, curC, 2, 0, 20);
            break;
        case 7:
            do_decision(player, curC, 1, 0, 15);
            break;
        case 8:
            do_decision(player, curC, 0, 0, 15);
            break;
        case 9:
            do_decision(player, curC, 0, 0, 15);
            break;
        }
    }

}

void do_decision(Player* player, int* curC, int check, int fold, int call){
    int r;
    srand(time(NULL));
    if(*curC == 0){
        r = rand()%100;
        if(r >= 0 && r < check){
            player->check(player);
            return;
        }
    }

    r = rand()%100;
    if(r >= 0 && r < fold){
        player->fold(player);
        return;
    }
    if(r >= fold && r < fold + call){
        if(*curC == 0){
            player->bet(player, curC);
        }
        else{
            player->call(player, curC);
        }
    }
    else{
        if(*curC == 0){
            player->bet(player, curC);
        }
        else{
            player->raise(player, curC);
        }
    }

}
void player_bet(Player* player, int* n){
    player->current_betting = 2;
    *n += 2;
    printf ("bet 2\t");
}
void player_check(Player* player){
    printf ("check\t");
}

void player_call(Player* player, int* n){
    if(player->current_betting != *n){
        player->current_betting = *n;
        printf ("call %d\t",*n);
    }
    else{
        player->is_finish = 1;
    }
}

void player_raise(Player* player, int* n){
    if(*n == 16)
        player->call(player, n);
    else{
        player->current_betting = 2*(*n);
        *n *= 2;
        printf ("raise %d\t",*n);
    }
}

void player_fold(Player* player){
    player->is_fold = 1;
    printf ("fold\t");
}
