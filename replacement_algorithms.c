#include "functions.h"

void LRU(int frame_pos, int isEmpty, IPT* Table, long* ser_num, Address** adr){
    int min_ser, min_pos, k;
    if( frame_pos==Table->frames ){                             // frame has not been found
        if(isEmpty!=-1){                                        // there is empty
            // instead of freeing i can replace values
            free(Table->Addresses[isEmpty]->op);
            free(Table->Addresses[isEmpty]);
            (Table->Addresses[isEmpty]) = (*adr);
            Table->Addresses[isEmpty]->isEmpty = 0;
            Table->Addresses[isEmpty]->serial_number = (*ser_num)++;
            // free(adr);
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
            // free(adr);
        }
    }
    else{                                                       // frame has been found
        Table->Addresses[frame_pos]->serial_number = (*ser_num)++;
        free( (*adr)->op );
        free( (*adr) );
        // free(adr);
    }
    return;
}