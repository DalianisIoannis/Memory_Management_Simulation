#include "functions.h"

void LRU(int frame_pos, int isEmpty, IPT* Table, long* ser_num, Address** adr){
    int min_ser, min_pos, k;
    if( frame_pos==Table->frames ){                             // frame has not been found
        if(isEmpty!=-1){                                        // there is empty
            // instead of freeing i can replace values
            // free(Table->Addresses[isEmpty]->op);
            Table->Addresses[isEmpty]->pageNumber = (*adr)->pageNumber;
            Table->Addresses[isEmpty]->isEmpty = 0;
            Table->Addresses[isEmpty]->serial_number = (*ser_num)++;
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
            // free(Table->Addresses[min_pos]->op);
            Table->Addresses[min_pos]->pageNumber = (*adr)->pageNumber;
            Table->Addresses[min_pos]->isEmpty = 0;
            Table->Addresses[min_pos]->serial_number = (*ser_num)++;
        }
    }
    else{                                                       // frame has been found
        Table->Addresses[frame_pos]->serial_number = (*ser_num)++;
        // free( (*adr)->op );
        // free( (*adr) );
    }
    return;
}