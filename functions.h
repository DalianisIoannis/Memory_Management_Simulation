#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

typedef struct statistics{
    long pageRequests;      // all requests regardless if the page is in IPT
    long pageFaults;        // pageNum not in IPT
    long pagesLoaded;       // pages loaded back to disk
    long pagesReplaced;     // had to replace page due to lack of space for newly fetched page
    long pagesRead;
    long pagesWrite; 
} Stats;

int virtual_memory(const int frames, const int references);