#include "type.h"

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
