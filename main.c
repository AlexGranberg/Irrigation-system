#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "uart.h"
#include "lcd.h"
#include "bitmacro.h"
#include "dht22.h"
#include "yl69.h"
#include "SSD1306.h"
#include "Font5x8.h"


int main() {


    init_serial();
    init_ports();
    init_adc(); // Initialize the ADC
    uint16_t temperature_int = 0;
    uint16_t humidity_int = 0;


    _delay_ms(2000);
    
    //GLCD_Setup();
	GLCD_SetFont(Font5x8, 5, 8, GLCD_Overwrite);

    while (true) {
        GLCD_Clear();
        sensor_power_on();
        _delay_ms(10); // Allow power to settle
        int val = analog_read(SENSOR_ANALOG_PIN);
        sensor_power_off();
        int moisturePercentage = 100 - ((float)val / 1023) * 100;
        printf("%d          ", val);
        
        // GLCD_GotoXY(1, 1);
        // GLCD_PrintString("Analog Humidity: ");
	    // GLCD_PrintInteger(val);

        GLCD_GotoXY(1, 1);
        GLCD_PrintString("Soil moisture: ");
	    GLCD_PrintInteger(moisturePercentage);
        GLCD_PrintString("%");  

        //int test = moisturePercentage * 1.6;
        // lcd_printf("Moisture: %d%%  ", moisturePercentage);
   
        
        // printf("Analog output: %d\n", val);
        // //printf("Moisture Percentage: %d%%\n", (val * 100) / 1023);
        // int moisturePercentage = 100 - ((float)val / 1023) * 100;
        // printf("Moisture Percentage: %d%%\n", moisturePercentage);

        DHT_Status status = DHT_Get(&temperature_int, &humidity_int) ;
            if (status == DHT_Status_Ok) {
			GLCD_GotoXY(1, 16);
            GLCD_PrintString("Humidity: ");
	        GLCD_PrintInteger(humidity_int / 10);
            GLCD_PrintString("%");

            GLCD_GotoXY(1, 31);
	        GLCD_PrintString("Temperature: ");
            GLCD_PrintInteger(temperature_int / 10);
            GLCD_PrintString("c");
            GLCD_Render();
	        }
            else {GLCD_GotoXY(1, 45);
	         GLCD_PrintInteger(status);
            }
        GLCD_GotoXY(1, 46);
        GLCD_PrintString("Pump: OFF");
        GLCD_Render();
        _delay_ms(5000);
    }

    return 0;
}
