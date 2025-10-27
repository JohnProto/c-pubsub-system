#include <stdio.h>
#include <stdlib.h>
#include "pss.h"

struct SubInfo *SL_Insert(int newsId, int newstm, struct Info *newsgp[]){    //NEWSGP[] TSEKARE GIA *

    struct SubInfo *new = malloc(sizeof(struct SubInfo));
    struct SubInfo *tmp = S;
    struct SubInfo *prev = NULL;
    while(tmp){
        if(newstm > tmp->stm){
            prev = tmp;
            tmp = tmp->snext;
        }
        else{
            new->sId = newsId;
            new->stm = newstm;
            *(new->sgp) = *newsgp;
            new->snext = tmp;
            if(prev){
                prev->snext = new;               
            }
            else{
                S = new;
            }
            return S;
        }
    }
    new->sId = newsId;
    new->stm = newstm;
    *(new->sgp) = *newsgp;
    new->snext = NULL;
    if(prev){
        prev->snext = new;
    }
    else{
        S = new;
    }
    return S;
}

struct SubInfo *SL_Delete(struct SubInfo *S, int newsId){

    struct SubInfo *tmp = S;
    struct SubInfo *prev = NULL;
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
            break;
        }
    }
    return S;
}

struct SubInfo *SL_LookUp(struct SubInfo *S, int newsId){

    struct SubInfo *tmp = S;
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

void SL_Print(struct SubInfo *S){

    struct SubInfo *tmp = S;
    while(tmp){
        printf("%d %d\n", tmp->sId, tmp->stm);
        tmp = tmp->snext;
    }
}

/*int main(){
    struct SubInfo *S;
    struct Info *xd[MG];    //NEWSGP[] TSEKARE GIA *
    int i;
    for(i=0; i<10; i++){
        S = SL_Insert(S, i+1, 2000+i, xd);
    }
    S = SL_Insert(S, 69, 1999, xd);
    S = SL_Delete(S, 1);
    struct SubInfo *tmp = SL_LookUp(S, 69);
    printf("%d %d\n", tmp->sId, tmp->stm);
    SL_Print(S);
    return 0;
}*/