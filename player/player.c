#include<stdlib.h>
#include<stdio.h>
#include "player.h"
#include "type.h"

char *Suit[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};
char *Face[13] = {"Deuce", "Three", "Four",
                  "Five", "Six", "Seven", "Eight", "Nine",
                  "Ten", "Jack", "Queen", "King", "Ace"};
char *type[10] = {"High Card", "One Pair", "Two Pair", "Three of A Kind",
                  "Straight", "Flush", "Full House", "Four Of A Kind", "Straight Flush",
                  "Royal Straight Flush"};


void deal(Deck* deck, Player* player){
    Card* tmp;
    int i;
    
    for (i = 0; i < 5; i++){
        tmp = (Card*) malloc (sizeof(Card));
        deck_pop(deck, (const void**) &tmp);
        player->cards[i] = tmp->val;
    }
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
    
    if(isFlush(a)){
        return 5;
    }
    
    if(isFour(a)){
        return 7;
    }
    
    if(isTwoPair(a)){
        return 2;
    }
    
    if(isThree(a)){
        if(isOnePair(a))
            return 6;
        else
            return 3;
    }

    if(isOnePair(a))
        return 1;
    
    return 0;
}

void printHand(Player* player){
    int i;
    int suit;
    int face;
    for (i = 0; i < 5; i++){
        suit = player->cards[i]/13;
        face = player->cards[i]%13;
        printf ("%s %s\n", Suit[suit], Face[face]);
    }
    printf ("%s\n",type[player->hand_type]);

}
