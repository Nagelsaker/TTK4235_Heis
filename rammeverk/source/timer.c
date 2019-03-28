#include "timer.h"
#include <time.h>

void timer(){
    int sec = 0, trigger = 3;
    clock_t before clock();
    do{
        clock_t difference = clock() - before;
        sec = difference / CLOCKS_PER_SEC;
    } while ( sec < trigger);
}
