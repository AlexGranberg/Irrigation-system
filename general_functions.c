#include "SSD1306.h"
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "bitmacro.h"
#include "yl69.h"
#include "Font5x8.h"

#define PUMP PB1

void initialize(){
    millis_init();
    sei();
    wdt_enable(WDTO_2S);
    init_ports();
    init_adc();
    GLCD_Setup();
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Overwrite);
    BIT_SET(DDRB, PUMP);
    BIT_CLEAR(DDRB, PUMP);
    _delay_ms(2000);
}