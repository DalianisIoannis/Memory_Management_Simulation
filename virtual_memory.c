#define _GNU_SOURCE
#include "functions.h"

int virtual_memory(const int frames){
    // frames = IPT size
    // Address**   IPT;
    int         i, j, k;
    char*       pageMod;
    char*       hexAdr;
    char*       token;
    char*       line = NULL;
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
    j = 0;
    while( read = getline( &line, &len, file1 ) != -1 && j<5 ){
        printf("file line is %s", line);

        token = strtok(line, " \t");   // address
        hexAdr = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(hexAdr, token);
        long decAddr = strtoul(hexAdr, NULL, 16);
        int pageNum = decAddr / FRAMESIZE;

        token = strtok(NULL, " \t");   // operation
        pageMod = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(pageMod, token);


        printf("hexAdr %s and pageMod %s\n", hexAdr, pageMod);
        printf("decAddr is %ld and pageNum %d\n", decAddr, pageNum);
        // MALLON TO PAGEMOD KRATAEI KAI ALLAGI GRAMMIS
        Address** adr = malloc(sizeof(Address*));
        (*adr) = malloc( sizeof(Address) );
        (*adr)->pageNumber = pageNum;

        (*adr)->op = malloc( (strlen(pageMod)+1) * sizeof(char) );
        strcpy( (*adr)->op, pageMod );

        // for(i=0; i<frames; i++){
        //     if(InvTable->Addresses[i]->isEmpty == 1){
        //         InvTable->Addresses[i]->isEmpty = 0;
        //         InvTable->Addresses[i] = adr;
        //     }
        // }
        if(j==0){
            free(InvTable->Addresses[0]);
            (InvTable->Addresses[0]) = (*adr);
            // printf("MPIKE Entrance has pageNum %d\n", InvTable->Addresses[0]->pageNumber);
            InvTable->Addresses[0]->isEmpty = 0;
            free(adr);
        }
        else{
            free( (*adr)->op );
            free( (*adr) );
            free(adr);
        }
        free(pageMod);
        free(hexAdr);
        j++;
    }
    for(i=0; i<frames; i++){
        if(InvTable->Addresses[i]->isEmpty == 0){
            printf("Entrance %d has pageNum %d\n", i, InvTable->Addresses[i]->pageNumber);
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
    // for(i=0; i<frames; i++){
    //     free(IPT[i]);
    // }
    // free(IPT);
    free(line);
    fclose(file1);
    printf("OK\n");
    return 0;
}