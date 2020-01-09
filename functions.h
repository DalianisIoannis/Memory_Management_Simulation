#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FRAMESIZE 4096

typedef struct mem_addr{
    long    serial_number;  // how early did it enter. The smallest num is the earliest
    int     processId;
    int     pageNumber;
    int     isEmpty;
    int     dirty;          // bool
    char*   op;             // W or R
} Address;

typedef struct InvertedPageTable{
    int frames;
    Address** Addresses;
} IPT;

int virtual_memory(const int frames);