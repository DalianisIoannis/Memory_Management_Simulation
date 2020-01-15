#include "functions.h"

int main(void){
    int frames              = 3;
    int references          = 1;    //how many references read from every trace
    int window_size         = 2;
    int total_references    = 12;

    virtual_memory(frames, references, window_size, total_references);

    return 0;
}