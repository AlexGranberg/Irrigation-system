#include <avr/io.h>
#include "bitmacro.h"
#include <stdbool.h>

#define PUMP PB1

void togglePump(bool active){
    if(active){
        BIT_SET(PORTB, PUMP);
    } else{
        BIT_CLEAR(PORTB, PUMP);
    }
}