#include <stdio.h>
#include <stdlib.h>
#include "pss.h"

struct Info *glast;
struct Info *gfirst;

void DL_Insert(int newitm, int newiId, int newigp[], unsigned int num_of_igp){

    struct Info *tmp = gfirst;
    struct Info *new = malloc(sizeof(struct Info));
    struct Info *tmpprev = NULL;
    while(tmp){
        if(newitm > tmp->itm){
            tmpprev = tmp;
            tmp = tmp->inext;
        }
        else{
            new->iId = newiId;
            new->itm = newitm;
            *new->igp = newigp;
            new->inext = tmp;
            if(tmp->iprev){
                tmp->iprev->inext = new;
                new->iprev = tmp->iprev;
            }
            else{
                gfirst = new;
                new->iprev = NULL;
            }
            tmp->iprev = new;
            return;
        }
    }
    new->iId = newiId;
    new->itm = newitm;
    *new->igp = newigp;
    new->inext = NULL;
    if(tmpprev){
        new->iprev = tmpprev;
        tmpprev->inext = new;
    }
    else{
        new->iprev = NULL;
        gfirst = new;
    }
    glast = new;
}

void DL_Delete(int newiId){

    struct Info *tmp = gfirst;
    while(tmp){
        if(tmp->iId != newiId){
            tmp = tmp->inext;
        }
        else{
            if(tmp->iprev){
                tmp->iprev->inext = tmp->inext;
                if(tmp != glast){
                    tmp->inext->iprev = tmp->iprev;
                }
                else{
                    glast = tmp->iprev;
                }
            }
            else{
                gfirst = tmp->inext;
                if(tmp != glast){
                    tmp->inext->iprev = NULL;
                }
                else{
                    glast = NULL;
                }
            }
            free(tmp);
        }
    }
}

struct Info *DL_LookUp(int newiId){

    struct Info *tmp = gfirst;
    while(tmp){
        if(tmp->iId != newiId){
            tmp = tmp->inext;
        }
        else{
            return tmp;
        }
    }
    return NULL;
}

void DL_Print(){    

    struct Info *tmp = gfirst;
    while(tmp){
        printf("%d %d\n", tmp->iId, tmp->itm);
        tmp = tmp->inext;
    }
}

/*int main(){
    int i;
    int xd[MG];
    for(i=0; i<10; i++){
        DL_Insert(i+1, 2000+i, xd, );
    }
    DL_Delete(10);
    struct Info *tmp = DL_LookUp(7);
    printf("%d %d\n", tmp->iId, tmp->itm);
    printf("%d\n", glast->iId);
    DL_Print();
    return 0;
}*/