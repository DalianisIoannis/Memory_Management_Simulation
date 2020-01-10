#define _GNU_SOURCE
#include "functions.h"

void printStatistics(Stats* statisticsInfo){
    printf("Total page requests %ld\n", statisticsInfo->pageRequests);
}

int virtual_memory(const int frames, const int references){
    long        decAddr, serial_num = 0;         
    int         i, j, k, num_of_address_read, pageNum, empty_frame, min_ser, min_pos;
    char        *pageMod, *hexAdr, *token, *line = NULL, *line1 = NULL, *line2 = NULL;
    Address**   adr;
    size_t      len = 0;
    ssize_t     read;
    FILE        *file1, *file2, *file_ref;
    IPT*        InvTable;
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
    // // // // // // // // // // // // // // // IPT initialization
    InvTable = malloc( sizeof(IPT) );
    InvTable->frames = frames;
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
    int limit = references*3;
    while( read != -1 && num_of_address_read<limit ){
        // printf("file line is %s", line);
        token = strtok(line, " \t");            // address
        hexAdr = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(hexAdr, token);
        decAddr = strtoul(hexAdr, NULL, 16);
        pageNum = decAddr / FRAMESIZE;
        token = strtok(NULL, " \t\n");          // operation
        pageMod = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(pageMod, token);                 // pageMod is only W or R + \0
        // printf("hexAdr %s and pageMod %s\n", hexAdr, pageMod);
        // printf("decAddr is %ld and pageNum %d\n", decAddr, pageNum);


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

        if( i==frames ){                                            // frame has not been found
            if(empty_frame != -1){                                  // there is empty
                free(InvTable->Addresses[empty_frame]);
                (InvTable->Addresses[empty_frame]) = (*adr);
                InvTable->Addresses[empty_frame]->isEmpty = 0;
                InvTable->Addresses[empty_frame]->serial_number = serial_num++;
                free(adr);
            }
            else{                                                   // there is no empty frame
                min_ser = InvTable->Addresses[0]->serial_number;    // LRU replace find smallest serial num
                min_pos = 0;
                for(k=1; k<frames; k++){
                    if(InvTable->Addresses[k]->serial_number < min_ser){
                        min_ser = InvTable->Addresses[k]->serial_number;
                        min_pos = k;
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
        else{                                                       // frame has been found
            InvTable->Addresses[i]->serial_number = serial_num++;
            free( (*adr)->op );
            free( (*adr) );
            free(adr);
        }

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

        if(num_of_address_read<limit){
            read = getline( &line, &len, file_ref );
        }
    }





    for(i=0; i<frames; i++){
        if(InvTable->Addresses[i]->isEmpty == 0){
            printf("Entrance %d has pageNum %d and serial %ld\n", i, InvTable->Addresses[i]->pageNumber, InvTable->Addresses[i]->serial_number);
        }
    }

    printStatistics(statisticsInfo);

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
    free(statisticsInfo);
    free(InvTable);
    free(line);
    fclose(file1);
    fclose(file2);
    return 0;
}