#include "functions.h"

int main(void){
    int frames = 10;
    int references = 1;    //how many references read from every trace
    int window_size = 4;

    virtual_memory(frames, references, window_size);

    return 0;
}