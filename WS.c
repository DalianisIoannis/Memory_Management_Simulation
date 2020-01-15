#include "functions.h"

int min_priority(Wrk_Set* WSet){    // returns position in WSet with min priority
    int rep_wind, j;
    long min_ser;
    min_ser = WSet->entry[0]->serial_number;
    rep_wind = 0;
    j = 1;
    while(j<WSet->window_size){
        if(WSet->entry[j]->serial_number<min_ser){
            min_ser = WSet->entry[j]->serial_number;
            rep_wind = j;
        }
        j++;
    }
    return rep_wind;
}

// return WSet first empty and k indicates the window the while exited
int is_pageNum_in_WS(IPT* Table, const int table_frame, Wrk_Set* WSet, int* empty_window){
    int k=0;
    while(k < WSet->window_size){       // where to put new address-does the pageNum already exist
        if(WSet->entry[k]->pageNumber == -1 && (*empty_window) == -1){     // first empty frame if exists
            (*empty_window) = k;
        }
        if(WSet->entry[k]->pageNumber == Table->Addresses[table_frame]->pageNumber){    // found pageNumber in WSet
            break;
        }
        k++;    // if i don't find pageNumber k returns equal to frames
    }
    return k;
}

void InsertWSet(const int window_place, Wrk_Set* WSet, Address** adr, long* WSserial_num){
    WSet->entry[window_place]->pageNumber       = (*adr)->pageNumber;
    WSet->entry[window_place]->serial_number    = (*WSserial_num)++;
    if(WSet->current_windows < WSet->window_size){
        WSet->current_windows++;
    }
}

void RemoveWSet(const int window_place, Wrk_Set* WSet, Address** adr, long* WSserial_num){
    // free(WSet->Addresses[rep_wind]->op);
    WSet->entry[window_place]->pageNumber       = -1;
    WSet->entry[window_place]->serial_number    = -1;
    WSet->current_windows--;
}

void WSet_Inserts(int window_pos, Wrk_Set* WSet, Address** adr, long* WSserial_num, int empty_window){
    int rep_wind, j;
    long min_ser;
    if(window_pos >= WSet->window_size){    // pageNumber not in WSet
        if(WSet->window_size == WSet->current_windows){   // all inserts have been made
            // pagefault++ and find smallest signature at place rep_wind
            rep_wind = min_priority(WSet);
            // free(WSet->Addresses[rep_wind]->op);
            InsertWSet(rep_wind, WSet, adr, WSserial_num);
        }
        else{                                          // not all inserts have been made
            // first empty window is given with empty_window
            InsertWSet(empty_window, WSet, adr, WSserial_num);
        }
    }
    else{                       	        // pageNumber already in WSet
        if(WSet->window_size == WSet->current_windows){   // all inserts have been made
            // find smallest signature at place rep_wind
            rep_wind = min_priority(WSet);
            RemoveWSet(rep_wind, WSet, adr, WSserial_num);
            // vlepo an tora to pageNumber einai sto WSet
            // rep_wind place of first empty
            j = 0;
            while(j<WSet->window_size){
                // the != is for not comparing empty insert
                if(j!=rep_wind && WSet->entry[j]->pageNumber == (*adr)->pageNumber){
                    break;
                }
                j++;
            }
            if(j>=WSet->window_size){   // have not found
                InsertWSet(rep_wind, WSet, adr, WSserial_num);
            }
            else{}    // den eisagetai
        }
        else{                                           // not all inserts have been made
            // change ser_num at pageNum position
            j = 0;
            while(j<WSet->window_size){
                if(WSet->entry[j]->pageNumber == (*adr)->pageNumber){
                    break;
                }
                j++;
            }
            WSet->entry[j]->serial_number = (*WSserial_num)++;
        }
    }
    return;
}

void InsertIPT(const int frame_place, IPT* Table, Address** adr, long* IPTserial_num){
    // free(Table->Addresses[isEmpty]->op);
    Table->Addresses[frame_place]->isEmpty          = 0;
    Table->Addresses[frame_place]->pageNumber       = (*adr)->pageNumber;
    Table->Addresses[frame_place]->serial_number    = (*IPTserial_num)++;
    Table->current_frames++;
}

void RemoveIPT(const int frame_place, IPT* Table, Address** adr){
    // free(Table->Addresses[k]->op);
    Table->Addresses[frame_place]->isEmpty          = 1;        // empty at first
    Table->Addresses[frame_place]->pageNumber       = -1;
    Table->Addresses[frame_place]->serial_number    = -1;
    // Table->Addresses[k]->op = malloc( sizeof(char) );
    Table->Addresses[frame_place]->op               = NULL;     // no string
}

void WS(int frame_pos, int isEmpty, IPT* Table, long* IPTserial_num, Address** adr, Wrk_Set* WSet, long* WSserial_num){
    int empty_window, k, j, empty_frame, deleted, del, rep_wind;
    long min_ser;
    if(Table->frames != Table->current_frames){ // not full IPT
        if(Table->frames <= frame_pos){         // pageNumber not in IPT
            printf("oxi gemato kai den einai mesa\n");
            // there is empty frame in IPT and pageNumber is not in IPT
            // isEmpty has the first empty frame

            InsertIPT(isEmpty, Table, adr, IPTserial_num);
            // check WS find if pageNumber is in Working Set

            empty_window = -1;
            k = is_pageNum_in_WS(Table, isEmpty, WSet, &empty_window);
            printf("prin to WS einai k %d kai empty_window %d\n", k, empty_window);
            // WSet function
            WSet_Inserts(k, WSet, adr, WSserial_num, empty_window);
        }
        else{                                   // pageNumber in IPT
            k = 0;
            while(k<Table->frames){
                if(Table->Addresses[k]->pageNumber == (*adr)->pageNumber){
                    break;
                }
                k++;
            }
            Table->Addresses[k]->serial_number = (*IPTserial_num)++;
            isEmpty = k;    // where found
            // check WS find if pageNumber is in Working Set
            empty_window = -1;
            k = is_pageNum_in_WS(Table, isEmpty, WSet, &empty_window);
            // WSet function
            WSet_Inserts(k, WSet, adr, WSserial_num, empty_window);
            // menoun h oxi?
            // free( (*adr)->op );
            // free( (*adr) );
        }
    }
    else{   // IPT is full
        // delete IPT pages not in WS
        deleted = 0;
        for(k=0; k<Table->frames; k++){
            del = 0;
            for(j=0; j<WSet->window_size; j++){
                if(Table->Addresses[k]->pageNumber == WSet->entry[j]->pageNumber){
                    del = 1;    // found in WS
                    break;
                }
            }
            if(del==0){
                RemoveIPT(k, Table, adr);
                deleted++;
            }
        }
        Table->current_frames-=deleted;
        // eisagogi kainouriou
        // iparxei idi?
        min_ser = Table->Addresses[0]->serial_number;
        rep_wind = 0;
        k = 0;
        empty_frame = -1;
        while(k<Table->frames){
            if(Table->Addresses[k]->isEmpty == 1 && empty_frame == -1){   // first empty frame if exists
                empty_frame = k;
                printf("keno %d\n", empty_frame);
            }
            if(Table->Addresses[k]->serial_number < min_ser){
                min_ser = Table->Addresses[k]->serial_number;
                rep_wind = k;
            }
            printf("gia k %d einai %d and %d\n", k, (*adr)->pageNumber, Table->Addresses[k]->pageNumber);
            if( (*adr)->pageNumber == Table->Addresses[k]->pageNumber){
                printf("same %d\n", k);
                break;
            }
            k++;
        }
        if(k>=Table->frames){   // an k==frames de vrika
            if(empty_frame!=-1){    //there is empty
                InsertIPT(empty_frame, Table, adr, IPTserial_num);

                isEmpty = empty_frame;
            }
            else{// replace
                Table->Addresses[rep_wind]->isEmpty = 0;
                Table->Addresses[rep_wind]->pageNumber = (*adr)->pageNumber;
                Table->Addresses[rep_wind]->serial_number = (*IPTserial_num)++;
                Table->current_frames++;
                isEmpty = rep_wind;
            }
        }
        else{   // iparxei
            isEmpty = k;
        }
        // WS find if pageNumber is in Working Set
        // isEmpty where found
        printf("EDO\n");
        empty_window = -1;
        k = is_pageNum_in_WS(Table, isEmpty, WSet, &empty_window);
        // WSet function
        WSet_Inserts(k, WSet, adr, WSserial_num, empty_window);
    }
    return;
}