#define _GNU_SOURCE
#include "functions.h"

void printStatistics(Stats* statisticsInfo){
    printf("Total page requests %ld\n", statisticsInfo->pageRequests);
}

int virtual_memory(const int frames, const int references, const int window_size){
    long        decAddr, IPT_serial_num = 0, WS_serial_num = 0;         
    int         i, j, k, num_of_address_read, pageNum, empty_frame;
    char        *pageMod, *hexAdr, *token, *line = NULL, *line1 = NULL, *line2 = NULL;
    Address**   adr;
    size_t      len = 0;
    ssize_t     read;
    FILE        *file1, *file2, *file_ref;
    IPT*        InvTable;
    Wrk_Set*    WSet;
    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // statisticsInfo initialize values
    Stats*      statisticsInfo = malloc( sizeof(Stats) );
    statisticsInfo->pageFaults = 0;
    statisticsInfo->pageRequests = 0;
    statisticsInfo->pagesLoaded = 0;
    statisticsInfo->pagesRead = 0;
    statisticsInfo->pagesWrite = 0;
    statisticsInfo->pagesReplaced = 0;
    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // open traces
    file1 = fopen("bzip.trace", "r");
    if(file1==NULL){
        fprintf(stderr, "Didn't open bzip.trace.\n");
        return -1;
    }
    file2 = fopen("gcc.trace", "r");
    if(file2==NULL){
        fprintf(stderr, "Didn't open gcc.trace.\n");
        return -1;
    }
    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // read file lines
    srand(time(NULL));                  // randomly choose first trace
    if( rand() % 2 + 0 ==0 ){
        printf("Starting with file bzip.trace.\n");
        file_ref = file1;
        line = line1;
    }
    else{
        printf("Starting with file gcc.trace.\n");
        file_ref = file2;
        line = line2;
    }

    num_of_address_read = 0;
    read = getline( &line, &len, file_ref );
    // int limit = references*3;
    int limit = 5;
    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // IPT initialization
    InvTable = malloc( sizeof(IPT) );
    InvTable->frames = frames;
    InvTable->current_frames = 0;
    InvTable->Addresses = malloc( frames*sizeof(Address*) );    // frames: IPT size
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
        InvTable->Addresses[i]->isEmpty = 1;                    // empty at first
        InvTable->Addresses[i]->op = NULL;                      // no string
    }
    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // Working Set creation
    WSet = malloc( sizeof(Wrk_Set) );
    WSet->window_size = window_size;
    WSet->current_windows = 0;
    WSet->entry = malloc( window_size*sizeof(WSentry*) );
    if(WSet->entry==NULL){
        fprintf(stderr, "Didn't allocate WSet Address array.\n");
        return -1;
    }
    for(i=0; i<window_size; i++){
        WSet->entry[i] = malloc( sizeof(WSentry) );
        if(WSet->entry[i]==NULL){
            fprintf(stderr, "Didn't allocate WSet Address.\n");
            return -1;
        }
        WSet->entry[i]->pageNum = -1;
        // WSet->Addresses[i]->isEmpty = 1;                    // empty at first
        // WSet->Addresses[i]->op = NULL;                      // no string
    }
    ////////////////////////////////////////////////////////////////
    while( read != -1 && num_of_address_read<limit ){
    // while( read != -1 ){
        // printf("file line is %s", line);
        token   = strtok(line, " \t");            // address
        hexAdr  = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(hexAdr, token);
        decAddr = strtoul(hexAdr, NULL, 16);
        pageNum = decAddr / FRAMESIZE;
        token   = strtok(NULL, " \t\n");          // operation
        pageMod = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(pageMod, token);                 // pageMod is only W or R + \0
        // printf("hexAdr %s and pageMod %s\n", hexAdr, pageMod);
        printf("decAddr is %ld and pageNum %d\n", decAddr, pageNum);


        adr = malloc(sizeof(Address*));     // make address with the items taken
        (*adr) = malloc( sizeof(Address) );
        (*adr)->pageNumber = pageNum;
        (*adr)->op = malloc( (strlen(pageMod)+1) * sizeof(char) );
        strcpy( (*adr)->op, pageMod );

        statisticsInfo->pageRequests++;

        i = 0;
        empty_frame = -1;
        while(i < frames){                  // where to put new address-does the pageNum already exist
            if(InvTable->Addresses[i]->isEmpty == 1 && empty_frame == -1){   // first empty frame if exists
                empty_frame = i;
            }
            if(InvTable->Addresses[i]->isEmpty == 0 && InvTable->Addresses[i]->pageNumber == (*adr)->pageNumber){   // found pageNum in IPT
                break;
            }
            i++;                            // if i don't find pageNum i returns equal to frames
        }
        // if i >= frames, pageNum hasn't been found 
        // LRU(i, empty_frame, InvTable, &IPT_serial_num, adr);
        // those two only if no lru called
        WS(i, empty_frame, InvTable, &IPT_serial_num, adr, WSet, &WS_serial_num);
        // free( (*adr)->op );
        // free( (*adr) );
        // 
        // if use WS create working set outside of while
        // WS needs other serial number
        // WS(i, empty_frame, InvTable, &IPT_serial_num, adr, WSet, &WS_serial_num);
        free(adr);

        free(pageMod);
        free(hexAdr);
        num_of_address_read++;

        if(num_of_address_read%references == 0){                    // change trace
            free(line);
            if(file_ref==file1){
                file_ref = file2;
                line = line2;
            }
            else{
                file_ref = file1;
                line = line2;
            }
        }

        if(num_of_address_read<limit){                              // read next line only if haven't reached limit
            printf("DIABASA GRAMMI\n");
            read = getline( &line, &len, file_ref );
        }
    }
    printf("VGKE ME %d\n", num_of_address_read);

    for(i=0; i<frames; i++){
        if(InvTable->Addresses[i]->isEmpty == 0){
            printf("Entrance %d has pageNum %d and serial %ld\n", i, InvTable->Addresses[i]->pageNumber, InvTable->Addresses[i]->serial_number);
        }
    }
    for(i=0; i<WSet->window_size; i++){
        printf("TO WSET %d einai %d me signature %ld\n", i, WSet->entry[i]->pageNum, WSet->entry[i]->serial_number);
    }
    printStatistics(statisticsInfo);

    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // close traces and free memory
    for(i=0; i<frames; i++){
        if(InvTable->Addresses[i]->isEmpty == 0){
            free(InvTable->Addresses[i]->op);
        }
        free(InvTable->Addresses[i]);
    }
    free(InvTable->Addresses);
    free(InvTable);
    for(i=0; i<window_size; i++){
        free(WSet->entry[i]);
    }
    free(WSet->entry);
    free(WSet);
    free(statisticsInfo);
    free(line);
    fclose(file1);
    fclose(file2);
    return 0;
}