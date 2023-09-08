#ifndef __YL69_H__
#define __YL69_H__

#include <stdio.h>

#define SENSOR_POWER_PIN 0
#define SENSOR_ANALOG_PIN 0 // Connect the AO pin to the appropriate analog pin

void init_ports();
void sensor_power_on();
void sensor_power_off();
int analog_read(uint8_t pin);
void init_adc();

#endif