#include <stdio.h>
#include <stdlib.h>
#include "pss.h"

struct Subscription *L_Insert(struct Subscription *S, int newsId){

    struct Subscription *new = malloc(sizeof(struct Subscription));
    new->sId = newsId;
    new->snext = S;
    S = new;
    return S;
}

struct Subscription *L_Delete(struct Subscription *S, int newsId){

    struct Subscription *tmp = S;
    struct Subscription *prev = NULL;
    while(tmp){
        if(tmp->sId != newsId){
            prev = tmp;
            tmp = tmp->snext;
        }
        else{
            if(prev){
                prev->snext = tmp->snext;
            }
            else{
                S = tmp->snext;
            }
            free(tmp);
        }
    }
    return S;
}

struct Subscription *L_LookUp(struct Subscription *S, int newsId){

    struct Subscription *tmp = S;
    while(tmp){
        if(tmp->sId == newsId){
            return tmp;
        }
        else{
            tmp = tmp->snext;
        }
    }
    return NULL;
}

void L_Print(struct Subscription *S){

    struct Subscription *tmp = S;
    while(tmp){
        printf("%d\n", tmp->sId);
        tmp = tmp->snext;
    }
}

/*int main(){
    struct Subscription *S;
    int i;
    for(i=0; i<10; i++){
        S = L_Insert(S, i+1);
    }
    S = L_Delete(S, 7);
    printf("%d\n", L_LookUp(S, 8)->sId);
    L_Print(S);
    return 0;
}*/