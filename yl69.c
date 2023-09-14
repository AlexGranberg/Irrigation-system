#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "uart.h"
#include "bitmacro.h"
#include "yl69.h"

void init_ports() {
    // Set the sensor power pin as an output
    DDRD |= (1 << SENSOR_POWER_PIN);
}

void sensor_power_on() {
    // Turn the sensor ON
    PORTD |= (1 << SENSOR_POWER_PIN);
    _delay_ms(10); // Allow power to settle
}

void sensor_power_off() {
    // Turn the sensor OFF
    PORTD &= ~(1 << SENSOR_POWER_PIN);
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