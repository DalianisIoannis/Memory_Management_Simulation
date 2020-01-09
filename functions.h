#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mem_addr{
    int processId;
    int pageNumber;

} Address;

int virtual_memory(int frames);