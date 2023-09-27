#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdio.h>
#include <stdbool.h>
#include "uart.h"
#include "lcd.h"
#include "bitmacro.h"
#include "dht22.h"
#include "yl69.h"
#include "SSD1306.h"
#include "Font5x8.h"
#include "Font4x7.h"
#include "millis.h"
#include "toggle_pump.h"
//#include "timers.h"
#define PUMP PB1

int main() {
    millis_init();
    sei();
    unsigned long current_millis = millis_get();
    unsigned long aliveCounter = millis_get();
    unsigned long soil_sensor_read_time = current_millis;
    unsigned long temp_sensor_read_time = current_millis;
    wdt_enable(WDTO_2S);
    //init_serial();
    bool pumpActive = false;
    init_ports();
    init_adc(); // Initialize the ADC
    uint16_t temperature_int = 0;
    uint16_t humidity_int = 0;
    uint16_t counter = 0;
    _delay_ms(2000);
    
    GLCD_Setup();
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Overwrite);
    BIT_SET(DDRB, PUMP);
    BIT_CLEAR(DDRB, PUMP);

    while (true) {
        wdt_reset();
        current_millis = millis_get();
        aliveCounter = millis_get();
        if (current_millis - soil_sensor_read_time >= 6000){
            //GLCD_ClearLine(41);
            //GLCD_Clear();
            uint16_t moisturePercentage = read_Soil();
           togglePump(moisturePercentage);
            if(moisturePercentage <= 40 && !pumpActive){
                 BIT_SET(DDRB, PUMP);
                pumpActive = true;
            }else if(moisturePercentage > 60 && pumpActive){
                 BIT_CLEAR(DDRB, PUMP);
                pumpActive = false;
            }
            //read_Soil();
            soil_sensor_read_time = current_millis;
        }
        if (current_millis - temp_sensor_read_time >= 3000){
            DHT_Status status = DHT_Get(&temperature_int, &humidity_int);
            if (status == DHT_Status_Ok) {
                GLCD_ClearLine(16);
                GLCD_ClearLine(32);
                GLCD_GotoXY(1, 32);
                GLCD_PrintString("Humidity: ");
                GLCD_PrintInteger(humidity_int / 10);
                GLCD_PrintString("%  ");
                counter++; 
                GLCD_PrintInteger(counter);

                GLCD_GotoXY(1, 16);
                GLCD_PrintString("Temperature: ");
                GLCD_PrintInteger(temperature_int / 10);
                GLCD_PrintString("c");
                GLCD_Render();
            }
            else {
                GLCD_GotoXY(1, 16);
                GLCD_PrintInteger(status);
            }
            temp_sensor_read_time = current_millis;
        }
        if (current_millis - aliveCounter > 3000){
            wdt_reset();
            while(1){}
        }

            
            // GLCD_GotoXY(1, 41);
            // GLCD_PrintString("Pump: OFF");
            // GLCD_GotoXY(60, 41);
            // GLCD_PrintString("- Tics: ");
            // counter++; 
            // GLCD_PrintInteger(counter);
            // GLCD_Render();
    }

    return 0;
}