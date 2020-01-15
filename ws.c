#include "functions.h"

void WSet_Inserts(int window_pos, Wrk_Set* WSet, Address** adr, long* WSserial_num, int empty_window){
    int rep_wind, j;
    long min_ser;
    if(window_pos >= WSet->window_size){    // pageNumber not in WSet
        if(WSet->window_size == WSet->current_windows){   // all inserts have been made
            // pagefault++ find smallest signature
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
            // smallest signature at place rep_wind
            // free(WSet->Addresses[rep_wind]->op);
            WSet->entry[rep_wind]->pageNumber = (*adr)->pageNumber;
            WSet->entry[rep_wind]->serial_number = (*WSserial_num)++;
        }
        else{                                          // not all inserts have been made
            // first empty window is given with empty_window
            WSet->entry[empty_window]->pageNumber = (*adr)->pageNumber;
            WSet->entry[empty_window]->serial_number = (*WSserial_num)++;
            WSet->current_windows++;
        }
    }
    else{                       	        // pageNumber already in WSet
        if(WSet->window_size == WSet->current_windows){   // all inserts have been made
            // find smallest signature
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
            // smallest signature at place rep_wind
            // free(WSet->Addresses[rep_wind]->op);
            WSet->current_windows--;
            WSet->entry[rep_wind]->pageNumber = -1;
            WSet->entry[rep_wind]->serial_number = -1;
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
                WSet->entry[rep_wind]->pageNumber = (*adr)->pageNumber;
                WSet->entry[rep_wind]->serial_number = (*WSserial_num)++;
            }
            else{    // den eisagetai
            }
        }
        else{                                           // not all inserts have been made
            // change ser_num
            j = 0;
            while(j<WSet->window_size){
                if(WSet->entry[j]->pageNumber == (*adr)->pageNumber){
                    break;
                }
            }
            // pou einai to pageNumber
            WSet->entry[j]->serial_number = (*WSserial_num)++;
        }
    }
    return;
}

void WS(int frame_pos, int isEmpty, IPT* Table, long* IPTserial_num, Address** adr, Wrk_Set* WSet, long* WSserial_num){
    int empty_window, k, j, empty_frame;
    if(Table->frames != Table->current_frames){ // not full IPT
        if(Table->frames <= frame_pos){         // pageNumber not in IPT
            // insert pageNumber in IPT
            // there is empty frame in IPT and pageNumber is not in IPT
            // isEmpty has the first empty frame

            // free(Table->Addresses[isEmpty]->op);
            Table->Addresses[isEmpty]->isEmpty = 0;
            Table->Addresses[isEmpty]->pageNumber = (*adr)->pageNumber;
            Table->Addresses[isEmpty]->serial_number = (*IPTserial_num)++;
            Table->current_frames++;
            // check WS find if pageNumber is in Working Set
            k = 0;
            empty_window = -1;
            while(k < WSet->window_size){                  // where to put new address-does the pageNum already exist
                if(WSet->entry[k]->pageNumber==-1 && empty_window == -1){  // first empty frame if exists
                    empty_window = k;
                }
                if(WSet->entry[k]->pageNumber == Table->Addresses[isEmpty]->pageNumber){   // found pageNumber in WSet
                    break;
                }
                k++;                            // if i don't find pageNumber k returns equal to frames
            }
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
            // same WS
            // check WS
            // find if pageNumber is in Working Set
            k = 0;
            empty_window = -1;
            while(k < WSet->window_size){                  // where to put new address-does the pageNum already exist
                if(WSet->entry[k]->pageNumber == -1 && empty_window == -1){   // first empty frame if exists
                    empty_window = k;
                }
                if(WSet->entry[k]->pageNumber == Table->Addresses[isEmpty]->pageNumber){//(*adr)->pageNumber){   // found pageNumber in WSet
                    break;
                }
                k++;                            // if i don't find pageNumber k returns equal to frames
            }
            // WSet function
            WSet_Inserts(k, WSet, adr, WSserial_num, empty_window);
            // menoun h oxi?
            // free( (*adr)->op );
            // free( (*adr) );
        }
    }
    else{   // IPT is full
        // delete IPT pages not in WS
        int deleted = 0, del;
        for(k=0; k<Table->frames; k++){
            del = 0;
            for(j=0; j<WSet->window_size; j++){
                if(Table->Addresses[k]->pageNumber == WSet->entry[j]->pageNumber){
                    // found in WS
                    del = 1;
                    // break;
                }
            }
            if(del==0){
                // free(Table->Addresses[k]->op);
                Table->Addresses[k]->isEmpty = 1;                    // empty at first
                // Table->Addresses[k]->op = malloc( sizeof(char) );
                Table->Addresses[k]->op = NULL;                      // no string

                deleted++;
            }
        }
        Table->current_frames-=deleted;
        // eisagogi kainouriou
        // iparxei idi?
        long min_ser = Table->Addresses[0]->serial_number;
        int rep_wind = 0;
        k = 1;
        empty_frame = -1;
        while(k<Table->frames){
            if(Table->Addresses[k]->isEmpty == 1 && empty_frame == -1){   // first empty frame if exists
                empty_frame = k;
            }
            if(Table->Addresses[k]->serial_number < min_ser){
                min_ser = Table->Addresses[k]->serial_number;
                rep_wind = k;
            }
            if( (*adr)->pageNumber == Table->Addresses[k]->pageNumber){
                break;
            }
            k++;
        }
        // an k==frames de vrika
        if(k>=Table->frames){
            if(empty_frame!=-1){    //there is empty
                // (Table->Addresses[empty_frame]) = (*adr);
                Table->Addresses[empty_frame]->isEmpty = 0;
                Table->Addresses[empty_frame]->pageNumber = (*adr)->pageNumber;
                Table->Addresses[empty_frame]->serial_number = (*IPTserial_num)++;
                Table->current_frames++;
            }
            else{// replace
                Table->Addresses[rep_wind]->isEmpty = 0;
                Table->Addresses[rep_wind]->pageNumber = (*adr)->pageNumber;
                Table->Addresses[rep_wind]->serial_number = (*IPTserial_num)++;
                Table->current_frames++;
            }
        }
    }
    return;
}