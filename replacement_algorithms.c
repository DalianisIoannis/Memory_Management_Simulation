#include "functions.h"

void LRU(int frame_pos, int isEmpty, IPT* Table, long* ser_num, Address** adr){
    int min_ser, min_pos, k;
    if( frame_pos==Table->frames ){                             // frame has not been found
        if(isEmpty!=-1){                                        // there is empty
            free(Table->Addresses[isEmpty]);
            (Table->Addresses[isEmpty]) = (*adr);
            Table->Addresses[isEmpty]->isEmpty = 0;
            Table->Addresses[isEmpty]->serial_number = (*ser_num)++;
            free(adr);
        }
        else{                                                   // there is no empty frame
            min_ser = Table->Addresses[0]->serial_number;       // LRU replace find smallest serial num
            min_pos = 0;
            for(k=1; k<Table->frames; k++){
                if(Table->Addresses[k]->serial_number < min_ser){
                    min_ser = Table->Addresses[k]->serial_number;
                    min_pos = k;
                }
            }
            free(Table->Addresses[min_pos]->op);
            free(Table->Addresses[min_pos]);
            (Table->Addresses[min_pos]) = (*adr);
            Table->Addresses[min_pos]->isEmpty = 0;
            Table->Addresses[min_pos]->serial_number = (*ser_num)++;
            free(adr);
        }
    }
    else{                                                       // frame has been found
        Table->Addresses[frame_pos]->serial_number = (*ser_num)++;
        free( (*adr)->op );
        free( (*adr) );
        free(adr);
    }
    return;
}

void WS(int frame_pos, int isEmpty, IPT* Table, long* ser_num, Address** adr, Wrk_Set* WSet){
    // check if new page Num is in IPT
    // we suppose IPT is our memory
    int k, empty_window, j, min_ser, rep_wind;
    if(Table->frames!=frame_pos){       // memory is not full - frame has not been found
        if(isEmpty!=-1){                // memory IPT is not full and pageNum not in Table
            // add new adr in IPT mem
            free(Table->Addresses[isEmpty]);
            (Table->Addresses[isEmpty]) = (*adr);
            Table->Addresses[isEmpty]->isEmpty = 0;
            Table->Addresses[isEmpty]->serial_number = (*ser_num)++;
            // inform Working Set
            // find if pageNum is in Working Set
            k = 0;
            empty_window = -1;
            while(k < WSet->window_size){                  // where to put new address-does the pageNum already exist
                if(WSet->Addresses[k]->isEmpty == 1 && empty_window == -1){   // first empty frame if exists
                    empty_window = k;
                }
                if(WSet->Addresses[k]->isEmpty == 0 && WSet->Addresses[k]->pageNumber == Table->Addresses[isEmpty]->pageNumber){//(*adr)->pageNumber){   // found pageNum in WSet
                    break;
                }
                k++;                            // if i don't find pageNum k returns equal to frames
            }
            if(k!=WSet->window_size){           // pageNum in WSet
                if(WSet->window_size==WSet->current_windows){   // all inserts
                    // find and delete smallest serial
                    min_ser = WSet->Addresses[0]->serial_number;
                    rep_wind = 0;
                    for(j=1; j<WSet->window_size; j++){
                        if(WSet->Addresses[j]->serial_number<min_ser){
                            min_ser = WSet->Addresses[j]->serial_number;
                            rep_wind = j;
                        }
                    }
                    // return;
                }
            }
            else{                               // pageNum not in WSet
                if(WSet->window_size==WSet->current_windows){   // all inserts
                    // page fault
                    // replace smallest signature with new adr
                    min_ser = WSet->Addresses[0]->serial_number;
                    rep_wind = 0;
                    j = 1;
                    while(j<WSet->window_size){
                        if(WSet->Addresses[j]->serial_number<min_ser){
                            min_ser = WSet->Addresses[j]->serial_number;
                            rep_wind = j;
                        }
                        j++;
                    }
                    free(WSet->Addresses[rep_wind]->op);
                    free(WSet->Addresses[rep_wind]);
                    (WSet->Addresses[rep_wind]) = (*adr);
                    WSet->Addresses[rep_wind]->isEmpty = 0;
                    WSet->Addresses[rep_wind]->serial_number = (*ser_num)++;
                    // free(adr);
                    // return;
                }
                else{       // not all inserts are made
                    WSet->Addresses[WSet->current_windows] = (*adr);
                    WSet->current_windows++;
                }
            }
            free(adr);
            return;
        }
        else{                           // no empty memory
            return;
        }
    }
    else{                               // memory is full - frame has been found
        return;
    }
    return;
}