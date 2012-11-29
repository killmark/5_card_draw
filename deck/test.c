#include<stdlib.h>
#include<stdio.h>
#include "deck.h"

char *Suit[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};
char *Face[13] = {"Deuce", "Three", "Four",
                  "Five", "Six", "Seven", "Eight", "Nine",
                  "Ten", "Jack", "Queen", "King", "Ace"};
char *type[10] = {"High Card", "One Pair", "Two Pair", "Three of A Kind",
                  "Straight", "Flush", "Full House", "Four Of A Kind", "Straight Flush",
                  "Royal Straight Flush"};

typedef struct{
    int cards[5];
    int hand_type;
    int hand_value;
}Player;

void deal(Deck*, Player*);
void cal_value(Player*);
void sort_by_val(Player*);
void sort_by_type(Player*);
int check_type(Player*);
void swap(Player*, int, int);
void printHand(Player*);

/* hand type */
int isRoyalSF(int a[4][13]);
int isSF(int a[4][13]);
int isFour(int a[4][13]);
int isFullHouse(int a[4][13]);
int isFlush(int a[4][13]);
int isStraight(int a[4][13]);
int isThree(int a[4][13]);
int isTwoPair(int a[4][13]);
int isOnePair(int a[4][13]);
int isHigh(int a[4][13]);


int main(){
    Deck* deck;
    Card* tmp;
    Player* p1;
    int i;
    int val;
    
    deck = (Deck*) malloc (sizeof(Deck));
    deck_shuffle(deck);

    p1 = (Player*) malloc (sizeof(Player));

    /* deal(deck, p1); */

    /* for (i = 0; i < 5; i++){ */
    /*     val = p1->cards[i]; */
    /*     printf ("%s %s\n",Suit[val/13], Face[val%13]); */
    /* } */

    /* printf ("***************************\n"); */
    /* sort(p1); */
    
    /* for (i = 0; i < 5; i++){ */
    /*     val = p1->cards[i]; */
    /*     printf ("%s %s\n",Suit[val/13], Face[val%13]); */
    /* } */
    
    /* test flush, straight, SF, RoyalSF */
    p1->cards[0] = 0;
    p1->cards[1] = 1;
    p1->cards[2] = 15;
    p1->cards[3] = 17;
    p1->cards[4] = 39;
    
    cal_value(p1);
    printHand(p1);
    printf ("%s\n",type[p1->hand_type]);

    printf ("%d\n",p1->hand_value);
    
    return 0;
}

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

    /* printf ("test straight and Flush\n"); */
    /* printf ("%d\n",isStraight(a)); */
    /* printf ("%d\n",isFlush(a)); */
    /* printf ("%d\n",isSF(a)); */
    /* printf ("%d\n",isRoyalSF(a)); */

    /* printf ("test pair, three and four\n"); */
    /* printf ("%d\n",isOnePair(a)); */
    /* printf ("%d\n",isTwoPair(a)); */
    /* printf ("%d\n",isThree(a)); */
    /* printf ("%d\n",isFullHouse(a)); */
    /* printf ("%d\n",isFour(a)); */
    
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

}

int isOnePair(int a[4][13]){
    int countC, countP;
    int i,j;
    countC = countP = 0;

    for (j = 0; j < 13; j++){
        for (i = 0; i < 4; i++){
            if(a[i][j] == 1)
                countC++;
        }
        if(countC == 2){
            countP++;
        }
        countC = 0;
    }
    if(countP == 1)
        return 1;
    else
        return 0;
}

int isTwoPair(int a[4][13]){
    
    int countC, countP;
    int i,j;
    countC = countP = 0;

    for (j = 0; j < 13; j++){
        for (i = 0; i < 4; i++){
            if(a[i][j] == 1)
                countC++;
        }
        if(countC == 2){
            countP++;
        }
        countC = 0;
    }
    if(countP == 2)
        return 1;
    else
        return 0;
}

int isThree(int a[4][13]){
    int countC, countT, countP;
    int i,j;
    countC = countT = 0;
    
    for (j = 0; j < 13; j++){
        for (i = 0; i < 4; i++){
            if(a[i][j] == 1)
                countC++;
        }
        if(countC == 3){
            countT++;
        }
        countC = 0;
    }
    if(countT == 1)
        return 1;
    else
        return 0;
}

int isStraight(int a[4][13]){
    int countStr, countC, count;
    int i,j;

    countStr = countC = count = 0;
    
    for (j = 0; j < 13; j++){
        for (i = 0; i < 4; i++){
            if(a[i][j] == 1)
                countC ++;
        }
        if(countC == 1){
            countStr++;
            count = countStr;
        }
        else{
            if (countStr > 0){
                countStr--;
            }
        }
        countC = 0;
    }

    if(count == 5)
        return 1;
    else
        return 0;
}

int isFlush(int a[4][13]){
    int countF, countC, count;  /* check flush */
    int i,j;

    countF = countC = count = 0;
    
    for (i = 0; i < 4; i++){
        for (j = 0; j < 13; j++){
            if(a[i][j] == 1)
                countC ++;
        }
        if(countC == 5){
            count = 5;
        }
        countC = 0;
    }
    
    if(count == 5)
        return 1;
    else
        return 0;
}

int isFullHouse(int a[4][13]){
    if(isThree(a) && isOnePair(a))
        return 1;
    else
        return 0;
}

int isFour(int a[4][13]){
    int countC;
    int i,j;

    countC = 0;
    for (j = 0; j < 13; j++){
        for (i = 0; i < 4; i++){
            if(a[i][j] == 1)
                countC++;
        }
        if(countC == 4){
            return 1;
        }
        countC = 0;
    }
    return 0;
}

int isSF(int a[4][13]){
    int i;
    
    if(isStraight(a) && isFlush(a))
        return 1;
    else
        return 0;
}

int isRoyalSF(int a[4][13]){
    int isAce;
    int i;
    for (i = 0; i < 4; i++){
        if(a[i][12] == 1)
            isAce = 1;
    }
    
    if(isStraight(a) && isFlush(a) && isAce == 1)
        return 1;
    else
        return 0;
}
