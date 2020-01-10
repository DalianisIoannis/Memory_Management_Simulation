#include "functions.h"

int main(void){
    int frames = 8;
    int references = 30;    //how many references ready from every trace

    virtual_memory(frames, references);
    return 0;
}