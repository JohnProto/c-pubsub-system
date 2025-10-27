/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr)
 * @Version 20-10-2020
 *
 * @e-mail       hy240-list@csd.uoc.gr
 *
 * @brief   Implementation of the "pss.h" header file for the Public Subscribe System,
 * function definitions
 *
 *
 ***************************************************************
 */
#include <stdio.h>
#include <stdlib.h>

#include "pss.h"
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
extern struct Group G[MG];
extern struct SubInfo *S;
/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(void){
    int i;
    for(i=0; i<MG; i++){
        G[i].gId = i+1;
        G[i].gfirst = NULL;
        G[i].glast = NULL;
        G[i].ggsub = NULL;
    }
    return 1;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void){
    return EXIT_SUCCESS;
}

void SL_Print(int flag){
    
    struct SubInfo *tmp = S;
    DPRINT("SUBSCRIBERLIST = <");
    while(tmp){
        DPRINT("%d", tmp->sId);
        tmp = tmp->snext;
        
        if(tmp){
            DPRINT(", ");
        }
    }
    DPRINT(">\n");
    if(flag){
        struct SubInfo *tmp2 = S;
        struct Subscription *tmp3;
        struct Group *tmp4;
        int i, group_count = 0, subscriber_count = 0, tmpflag = 1;
        for(i=0; i<MG; i++){
            if(G[i].gfirst && G[i].glast && G[i].ggsub){
                group_count++;
            }
        }
        while(tmp2){
            DPRINT("SUBSCRIBERID = <%d>, GROUPLIST = <", tmp2->sId);
            subscriber_count++;
            for(i=0; i<MG; i++){
                tmp3 = G[i].ggsub;
                while(tmp3){
                    if(tmp2->sId == tmp3->sId){
                        if(!tmpflag){
                            DPRINT(", ");
                        }
                        DPRINT("%d", G[i].gId);
                        tmpflag = 0;
                        break;
                    }
                    tmp3 = tmp3->snext;
                }
            }
            tmpflag = 1;
            tmp2 = tmp2->snext;
            DPRINT(">\n");
        }
        DPRINT("NO_GROUPS = <%d>, NO_SUBSCRIBERS = <%d>", group_count, subscriber_count);
    }
}

void L_Print(int i, int flag){

    struct Subscription *tmp = G[i].ggsub;
    if(!flag){
        DPRINT("GROUPID = <%d>, SUBLIST = <", G[i].gId);
    }
    else{
        DPRINT("SUBLIST = <");
    }
    while(tmp){
        DPRINT("%d", tmp->sId);
        tmp = tmp->snext;
        if(tmp){
            DPRINT(", ");
        }
    }
    DPRINT(">\n");
}

void DL_Print(int i, int flag){    

    struct Info *tmp = G[i].gfirst;
    DPRINT("GROUPID = <%d>, INFOLIST = <",G[i].gId);
    while(tmp){
        DPRINT("%d", tmp->iId);
        tmp = tmp->inext;
        if(tmp){
            DPRINT(", ");
        }
    }
    if(!flag){
        DPRINT(">\n");
    }
    else{
        DPRINT(">, ");
        L_Print(i, 1);
    }
}

void SL_Delete(int newsId){

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
    int i;
    for(i=0; i<MG; i++){
        struct Subscription *tmp2 = G[i].ggsub;
        struct Subscription *prev2 = NULL;
        while(tmp2){
            if(tmp2->sId != newsId){
                prev2 = tmp2;
                tmp2 = tmp2->snext;
            }
            else{
                if(prev2){
                    prev2->snext = tmp2->snext;
                }
                else{
                    G[i].ggsub = tmp2->snext;
                }
                free(tmp2);
                break;
            }
        }
    }
}

struct SubInfo *SL_LookUp(int newsId){

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

/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr){
    int i, flag, j;
    struct Info *tmp;
    struct Info *prev = NULL;
    for(i=0; i<size_of_gids_arr-1; i++){
        if((gids_arr[i] - 1 <= 63) && (gids_arr[i] - 1 >= 0)){
            struct Info *new = malloc(sizeof(struct Info));
            tmp = G[gids_arr[i]-1].gfirst;
            flag = 0;
            if(!tmp){
                new->iId = iId;
                new->itm = iTM;
                G[gids_arr[i]-1].gfirst = new;
                new->iprev = NULL;
                new->inext = NULL;
                G[gids_arr[i]-1].glast = new;
            }
            else{
                while(tmp){
                    if(iTM < tmp->itm){
                        new->iId = iId;
                        new->itm = iTM;
                        new->inext = tmp;
                        if(prev){
                            tmp->iprev->inext = new;
                        }
                        else{
                            G[gids_arr[i]-1].gfirst = new;
                        }
                        new->iprev = tmp->iprev;
                        tmp->iprev = new;
                        flag=1;
                        break;
                    }
                    prev = tmp;
                    tmp = tmp->inext;
                }
                if(!flag && prev){
                    new->iId = iId;
                    new->itm = iTM;
                    prev->inext = new;
                    new->inext = NULL;
                    new->iprev = prev;
                    G[gids_arr[i]-1].glast = new;
                }
            }
            new->igp[gids_arr[i]-1] = 1;
            DL_Print(gids_arr[i]-1, 0);
        }
    }
    return 0;
}

/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr){

    int flag = 0, i;
    struct SubInfo *tmp = S;
    struct SubInfo *new = malloc(sizeof(struct SubInfo));
    struct SubInfo *prev = NULL;
    if(!tmp){
        new->sId = sId;
        new->stm = sTM;
        new->snext = NULL;
        S = new;
    }
    else{
        while(tmp){
            if(sTM < tmp->stm){
                new->sId = sId;
                new->stm = sTM;
                new->snext = tmp;
                if(prev){
                    prev->snext = new;
                }
                else{
                    S = new;
                }
                flag = 1;
                break;
            }
            prev = tmp;
            tmp = tmp->snext;
        }
        if(!flag && prev){
            new->sId = sId;
            new->stm = sTM;
            new->snext = NULL;
            prev->snext = new;
        }
    }
    SL_Print(0);
    for(i=0; i<MG; i++){
        new->sgp[i] = (struct Info *)1;
    }
    for(i=0; i<size_of_gids_arr-1; i++){
        if((gids_arr[i] - 1 <= 63) && (gids_arr[i] - 1 >= 0)){
            struct Subscription *newsub = malloc(sizeof(struct Subscription));
            newsub->sId = sId;
            newsub->snext = G[gids_arr[i]-1].ggsub;
            G[gids_arr[i]-1].ggsub = newsub;
            new->sgp[gids_arr[i]-1] = G[gids_arr[i]-1].glast;  
            L_Print(gids_arr[i]-1, 0);
        }
    }
    return 0;
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId){
    struct SubInfo *tmp = SL_LookUp(sId);
    int i;
    if(tmp){
        for(i=0; i<MG; i++){
            if(tmp->sgp[i] != (struct Info *)1){
                if(!tmp->sgp[i]){
                    if(G[i].gfirst){
                        tmp->sgp[i] = G[i].gfirst;
                        DPRINT("GROUPID = <%d>, INFOLIST<", i+1);
                        DPRINT("%d", tmp->sgp[i]->iId);
                        while(tmp->sgp[i]->inext){
                            DPRINT(", %d", tmp->sgp[i]->inext->iId);
                            if(tmp->sgp[i]->inext){
                                tmp->sgp[i] = tmp->sgp[i]->inext;
                            }
                        }
                        DPRINT(">, ");
                        DPRINT("NEWSGP = <%d>\n", tmp->sgp[i]->iId);
                    }
                }
                else if(tmp->sgp[i]->inext){
                    DPRINT("GROUPID = <%d>, INFOLIST<", i+1);
                    while(tmp->sgp[i]->inext){
                        DPRINT("%d", tmp->sgp[i]->inext->iId);
                        if(tmp->sgp[i]->inext){
                            tmp->sgp[i] = tmp->sgp[i]->inext;
                            DPRINT(", ");
                        }
                    }
                    DPRINT(">, ");
                    DPRINT("NEWSGP = <%d>\n", tmp->sgp[i]->iId);
                }    
            }
        }
    }
    else{
        return 1;
    }
    return 0;
}
/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId){

    if(SL_LookUp(sId)){
        SL_Delete(sId);
        SL_Print(0);
        int i;
        for(i=0; i<MG; i++){
            L_Print(i, 0);
        }
        return 0;
    }
    else{
        return 1;
    }
}
/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void){
    
    int i;
    for(i=0; i<MG; i++){
        DL_Print(i, 1);
    }
    SL_Print(1);
    return 0;
}