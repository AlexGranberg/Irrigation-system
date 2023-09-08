#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "uart.h"
#include "lcd.h"
#include "bitmacro.h"
#include "dht22.h"
#include "yl69.h"


int main() {

    lcd_init();
    lcd_clear();
    //init_serial();
    init_ports();
    init_adc(); // Initialize the ADC
    uint16_t temperature_int = 0;
    uint16_t humidity_int = 0;

    _delay_ms(2000);
    
    while (true) {
        sensor_power_on();
        _delay_ms(10); // Allow power to settle
        int val = analog_read(SENSOR_ANALOG_PIN);
        sensor_power_off();
        lcd_set_cursor(0,0);
        lcd_printf("Analog out: %d", val);
        //printf("Moisture Percentage: %d%%\n", (val * 100) / 1023);
        lcd_set_cursor(0,0);
        int moisturePercentage = 100 - ((float)val / 1023) * 100;
        int test = moisturePercentage * 1.6;
        // lcd_printf("Moisture: %d%%  ", moisturePercentage);
        lcd_set_cursor(0,1);
        lcd_printf("Soil: %d%%", moisturePercentage);
        
        
        // printf("Analog output: %d\n", val);
        // //printf("Moisture Percentage: %d%%\n", (val * 100) / 1023);
        // int moisturePercentage = 100 - ((float)val / 1023) * 100;
        // printf("Moisture Percentage: %d%%\n", moisturePercentage);


        _delay_ms(5000);
        lcd_clear();

        DHT_Status status = DHT_Get(&temperature_int, &humidity_int) ;
            if (status == DHT_Status_Ok) {
			lcd_set_cursor(0,0);
			lcd_printf("Hum: %d%%", humidity_int / 10);
            lcd_set_cursor(0,1);
			lcd_printf("Temp: %dc", temperature_int / 10);
	        }
            else printf("ERROR %d\n",status);
        _delay_ms(5000);
        lcd_clear();
    }

    return 0;
}
