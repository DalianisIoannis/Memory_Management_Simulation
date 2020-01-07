#include "functions.h"

int virtual_memory(){


    FILE* file1 = fopen("bzip.trace", "r");
    if(file1==NULL){
        fprintf(stderr, "Didn't open file1.\n");
        return -1;
    }







    fclose(file1);



    printf("OK\n");

    return 0;
}