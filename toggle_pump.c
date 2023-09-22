#include <avr/io.h>
#include "bitmacro.h"
#include <stdbool.h>

#define LED_PIN PB1

void togglePump(bool active){
    if(active){
        BIT_SET(PORTB, LED_PIN);
    } else{
        BIT_CLEAR(PORTB, LED_PIN);
    }
}