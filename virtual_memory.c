#define _GNU_SOURCE
#include "functions.h"

int virtual_memory(int frames){
    // frames = IPT size
    // int**       IPT;
    Address**   IPT;
    int         i, j, k;
    char*       pageMod;
    char*       pageNum;
    char*       token;
    char*       line = NULL;
    size_t      len = 0;
    ssize_t     read;
    FILE*       file1;
    
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
    // IPT = malloc( frames*sizeof(int *) );
    IPT = malloc( frames*sizeof(Address*) );
    if(IPT==NULL){
        fprintf(stderr, "Didn't allocate IPT1.\n");
        return -1;
    }
    for(i=0; i<frames; i++){
        // IPT[i] = malloc( 4*sizeof(int) );   //4 ints
        IPT[i] = malloc( sizeof(Address) );
        // IPT[i] = NULL;
        if(IPT[i]==NULL){
            fprintf(stderr, "Didn't allocate IPT2.\n");
            return -1;
        }
        // probably needs to initialize with NULL
    }
    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // read file lines
    // // // // // // // // // // // // // // // // // // // // // // // // //
    j = 0;
    while( read = getline( &line, &len, file1 ) != -1 && j<5 ){
        // printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);

        token = strtok(line, " \t");   // address
        pageNum = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(pageNum, token);

        token = strtok(NULL, " \t");   // operation
        pageMod = malloc( (strlen(token)+1)*sizeof(char) );
        strcpy(pageMod, token);

        token = strtok(NULL, " \t");

        printf("To 1 einai %s kai to 2 %s\n", pageNum, pageMod);
        j++;
        free(pageNum);
        free(pageMod);
    }

    // // // // // // // // // // // // // // // // // // // // // // // // //
    // // // // // // // // // // // // // // // bzip.trace close and empty IPT
    // // // // // // // // // // // // // // // // // // // // // // // // //
    for(i=0; i<frames; i++){
        free(IPT[i]);
    }
    free(IPT);
    free(line);
    fclose(file1);
    printf("OK\n");
    return 0;
}