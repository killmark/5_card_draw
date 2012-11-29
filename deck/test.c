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
    int hand_value;
}Player;

void deal(Deck*, Player*);
void cal_value(Player*);
void sort(Player*);
int check_type(Player*);

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
    p1->cards[0] = 14;
    p1->cards[1] = 13;
    p1->cards[2] = 26;
    p1->cards[3] = 39;
    p1->cards[4] = 0;
    
    cal_value(p1);
    printf ("%d\n",p1->hand_value);
    
    check_type(p1);

    
    /* cal_value(p1); */

    /* printf ("%d\n",p1->hand_value); */
    
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
    int type;
    int val;
    int tmp;
    int tmp2;
    int i;
    /*
      type = check_type(player);
      sort(player); according type;
      now assuming it is 0.
    */
    
    type = 0;
    /* first byte is not used, set 0 */
    val = 0;

    /* set type */
    val |= type << 20;

    /* set cards */
    
    tmp2 = 16;
    for (i = 0; i < 5; i++, tmp2 -= 4){
        tmp = player->cards[i];
        tmp = tmp%13;
        val |= tmp << tmp2;
    }

    player->hand_value = val;
}
void sort(Player* player){
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
int check_type(Player* player){
    int suit, face;
    int type;
    int a[4][13] = {0};
    int i,j;

    for (i = 0; i < 5; i++){
        suit = (player->cards[i])/13;
        face = (player->cards[i])%13;
        a[suit][face] = 1;
    }

    sort(player);
    
    /* printf ("test straight and Flush\n"); */
    /* printf ("%d\n",isStraight(a)); */
    /* printf ("%d\n",isFlush(a)); */
    /* printf ("%d\n",isSF(a)); */
    /* printf ("%d\n",isRoyalSF(a)); */

    printf ("test pair, three and four\n");
    printf ("%d\n",isOnePair(a));
    printf ("%d\n",isTwoPair(a));
    printf ("%d\n",isThree(a));
    printf ("%d\n",isFullHouse(a));
    printf ("%d\n",isFour(a));
    
    return type;
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
