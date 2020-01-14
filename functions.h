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
    int current_frames; // how many frames are used
    Address** Addresses;
} IPT;

typedef struct Working_Set_entry{
    int     pageNum;
    long    serial_number;
} WSentry;

typedef struct working_set{
    int current_windows;
    int window_size;
    // Address** Addresses;
    // int isEmpty;
    // int* pageNum;  // pageNum array empty with -1
    WSentry** entry;
} Wrk_Set;

typedef struct statistics{
    long pageRequests;      // all requests regardless if the page is in IPT
    long pageFaults;        // pageNum not in IPT
    long pagesLoaded;       // pages loaded back to disk
    long pagesReplaced;     // had to replace page due to lack of space for newly fetched page
    long pagesRead;
    long pagesWrite; 
} Stats;

int virtual_memory(const int, const int, const int);

void LRU(int, int, IPT*, long*, Address**);

void WS(int, int, IPT*, long*, Address**, Wrk_Set*, long*); // first long IPTserial_num and second WSserial_num

void WSet_Inserts(int, Wrk_Set*, Address**, long*, int);