#ifndef CLEAR
#define CLEAR
#include <stdio.h>
static inline void clear_buff(void){
    int c;
    while((c=getchar())!='\n'&& c!=EOF);
}
#endif