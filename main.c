#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "uart.h"
#include "lcd.h"

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b)))) 

#define SENSOR_POWER_PIN 0
#define SENSOR_ANALOG_PIN 0 // Connect the AO pin to the appropriate analog pin

void init_ports() {
    // Set the sensor power pin as an output
    DDRB |= (1 << SENSOR_POWER_PIN);
}

void sensor_power_on() {
    // Turn the sensor ON
    PORTB |= (1 << SENSOR_POWER_PIN);
    _delay_ms(10); // Allow power to settle
}

void sensor_power_off() {
    // Turn the sensor OFF
    PORTB &= ~(1 << SENSOR_POWER_PIN);
}

int analog_read(uint8_t pin) {
    // Configure ADC to use 'pin' as input
    ADMUX = (1 << REFS0) | (pin & 0x07);

    // Start ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC)) {
        _delay_us(10); // Short delay to allow time for conversion
    }

    // Read ADC result
    return ADC;
}

void init_adc() {
    // Set the ADC Enable bit and configure the ADC Prescaler to 64
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
    
    // Set the reference voltage to AVCC with external capacitor at AREF pin
    ADMUX |= (1 << REFS0);
}

int main() {

    lcd_init();
    lcd_clear();
    //init_serial();
    init_ports();
    init_adc(); // Initialize the ADC

    _delay_ms(2000);
    
    while (true) {
        sensor_power_on();
        _delay_ms(10); // Allow power to settle
        int val = analog_read(SENSOR_ANALOG_PIN);
        sensor_power_off();
        lcd_set_cursor(0,0);
        //lcd_printf("Analog output: %d\n", val);
        //printf("Moisture Percentage: %d%%\n", (val * 100) / 1023);
        lcd_set_cursor(0,0);
        int moisturePercentage = 100 - ((float)val / 1023) * 100;
        int test = moisturePercentage * 1.6;
        lcd_printf("Moisture: %d%%\n", moisturePercentage);
        lcd_set_cursor(0,1);
        lcd_printf("Moist * 1.6: %d%%", test);
        
        
        
        
        // printf("Analog output: %d\n", val);
        // //printf("Moisture Percentage: %d%%\n", (val * 100) / 1023);
        // int moisturePercentage = 100 - ((float)val / 1023) * 100;
        // printf("Moisture Percentage: %d%%\n", moisturePercentage);


        _delay_ms(5000);
        lcd_clear();
    }

    return 0;
}
