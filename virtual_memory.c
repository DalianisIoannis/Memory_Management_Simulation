#define _GNU_SOURCE
#include "functions.h"

int virtual_memory(const int frames){
    // frames = IPT size
    // Address**   IPT;
    long        decAddr, serial_num = 0;         
    int         i, j, k, num_of_address_read, pageNum, found, empty_frame, min_ser, min_pos;
    char*       pageMod;
    char*       hexAdr;
    char*       token;
    char*       line = NULL;
    Address**   adr;
    size_t      len = 0;
    ssize_t     read;
    FILE*       file1;
    IPT*        InvTable;
    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // bzip.trace open
    // // // // // // // // // // // // // // // // // // // // // // // // //
    file1 = fopen("bzip.trace", "r");
    if(file1==NULL){
        fprintf(stderr, "Didn't open file1.\n");
        return -1;
    }
    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // IPT initialization
    // // // // // // // // // // // // // // // // // // // // // // // // //
    InvTable = malloc( sizeof(IPT) );
    InvTable->frames = frames;
    InvTable->Addresses = malloc( frames*sizeof(Address*) );
    if(InvTable->Addresses==NULL){
        fprintf(stderr, "Didn't allocate IPT Address array.\n");
        return -1;
    }
    for(i=0; i<frames; i++){
        InvTable->Addresses[i] = malloc( sizeof(Address) );
        if(InvTable->Addresses[i]==NULL){
            fprintf(stderr, "Didn't allocate IPT Address.\n");
            return -1;
        }
        InvTable->Addresses[i]->isEmpty = 1;
        InvTable->Addresses[i]->op = NULL;
    }





    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // read file lines
    // // // // // // // // // // // // // // // // // // // // // // // // //
    num_of_address_read = 0;
    while( read = getline( &line, &len, file1 ) != -1 && num_of_address_read<30 ){
        // printf("file line is %s", line);
        token = strtok(line, " \t");   // address
        hexAdr = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(hexAdr, token);
        decAddr = strtoul(hexAdr, NULL, 16);
        pageNum = decAddr / FRAMESIZE;
        token = strtok(NULL, " \t");   // operation
        pageMod = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(pageMod, token);
        // printf("hexAdr %s and pageMod %s\n", hexAdr, pageMod);
        printf("decAddr is %ld and pageNum %d\n", decAddr, pageNum);
        // MALLON TO PAGEMOD KRATAEI KAI ALLAGI GRAMMIS



        adr = malloc(sizeof(Address*));
        (*adr) = malloc( sizeof(Address) );
        (*adr)->pageNumber = pageNum;

        (*adr)->op = malloc( (strlen(pageMod)+1) * sizeof(char) );
        strcpy( (*adr)->op, pageMod );

        // find empty frame if i >= frames there is no empty
        i = 0;
        found = 0;
        empty_frame = -1;
        while(i < frames){
            if(InvTable->Addresses[i]->isEmpty == 1 && empty_frame == -1){   // first empty frame
                empty_frame = i;
            }
            if(InvTable->Addresses[i]->isEmpty == 0 && InvTable->Addresses[i]->pageNumber == (*adr)->pageNumber){   // found pageNum
                found = 1;
                break;
            }
            i++;
        }

        if( found==0 ){             // frame has not been found
            if(empty_frame != -1){  // there is empty
                free(InvTable->Addresses[empty_frame]);
                (InvTable->Addresses[empty_frame]) = (*adr);
                InvTable->Addresses[empty_frame]->isEmpty = 0;
                InvTable->Addresses[empty_frame]->serial_number = serial_num++;
                free(adr);
            }
            else{   // there is no empty frame
                // LRU replace find smallest serial num
                min_ser = InvTable->Addresses[0]->serial_number;
                min_pos = 0;
                for(i=1; i<frames; i++){
                    if(InvTable->Addresses[i]->serial_number < min_ser){
                        min_ser = InvTable->Addresses[i]->serial_number;
                        min_pos = i;
                    }
                }
                free(InvTable->Addresses[min_pos]->op);
                free(InvTable->Addresses[min_pos]);
                (InvTable->Addresses[min_pos]) = (*adr);
                InvTable->Addresses[min_pos]->isEmpty = 0;
                InvTable->Addresses[min_pos]->serial_number = serial_num++;

                // free( (*adr)->op );
                // free( (*adr) );
                free(adr);
            }
        }
        else{       // frame has been found
            InvTable->Addresses[i]->serial_number = serial_num++;
            free( (*adr)->op );
            free( (*adr) );
            free(adr);
        }

        free(pageMod);
        free(hexAdr);
        num_of_address_read++;
    }
















    for(i=0; i<frames; i++){
        if(InvTable->Addresses[i]->isEmpty == 0){
            printf("Entrance %d has pageNum %d and serial %ld\n", i, InvTable->Addresses[i]->pageNumber, InvTable->Addresses[i]->serial_number);
        }
    }
    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // bzip.trace close and empty IPT
    // // // // // // // // // // // // // // // // // // // // // // // // //
    for(i=0; i<frames; i++){
        if(InvTable->Addresses[i]->isEmpty == 0){
            free(InvTable->Addresses[i]->op);
        }
        free(InvTable->Addresses[i]);
    }
    free(InvTable->Addresses);
    free(InvTable);
    free(line);
    fclose(file1);
    printf("OK\n");
    return 0;
}