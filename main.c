#include "functions.h"

int main(void){
    int frames = 2;
    int references = 1;    //how many references read from every trace
    int window_size = 1;

    virtual_memory(frames, references, window_size);

    return 0;
}