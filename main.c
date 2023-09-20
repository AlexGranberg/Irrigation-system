#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
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
//#include "timers.h"

void setupTimer1() {
  cli(); // Disable interrupts during configuration
  TCCR1A = 0;     // Set control register A to 0
  TCCR1B = 0;     // Set control register B to 0

  // Configure timer to trigger every 1 second
  // Arduino Uno clock speed is 16MHz
  // Prescale by 1024
  // Timer value = (16e6 / 1024) - 1
  // For 1-second interval:
  // Timer value = 15624
  TCNT1 = 15624;  // Set the initial timer value
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024
  TIMSK1 |= (1 << TOIE1); // Enable timer overflow interrupt

  sei(); // Re-enable interrupts
}

void setupTimer2() {
  cli(); // Disable interrupts during configuration
  TCCR2A = 0; // Set control register A to 0
  TCCR2B = 0; // Set control register B to 0

  // Configure timer to trigger every 60 seconds
  // Arduino Uno clock speed is 16MHz
  // Prescale by 1024
  // Timer value = (16e6 / 1024) * 60 - 1
  // For 60-second interval:
  // Timer value = 93749
  OCR2A = 93749; // Set the compare value for 60-second interval
  TCCR2B |= (1 << WGM21); // Configure timer mode: CTC mode
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Set prescaler to 1024
  TIMSK2 |= (1 << OCIE2A); // Enable timer compare match A interrupt
  sei(); // Re-enable interrupts
}


ISR(TIMER1_OVF_vect) {
  // Your code here
  sensor_power_on();
  _delay_ms(10); // Allow power to settle
  int val = analog_read(SENSOR_ANALOG_PIN);
  sensor_power_off();
  int moisturePercentage = 100 - ((float)val / 1023) * 100;

  GLCD_ClearLine(1);
  GLCD_GotoXY(1, 1);
  GLCD_PrintString("Soil moisture: ");
  GLCD_PrintInteger(moisturePercentage);
  GLCD_PrintString("%");
  GLCD_Render();
}

int main() {

    //init_serial();
    init_ports();
    init_adc(); // Initialize the ADC
    uint16_t temperature_int = 0;
    uint16_t humidity_int = 0;
    uint16_t counter = 0;
    setupTimer1(); // Initialize Timer1
    _delay_ms(2000);
    
    GLCD_Setup();
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Overwrite);

    while (true) {        
            GLCD_ClearLine(16);
            GLCD_ClearLine(31);
            //GLCD_ClearLine(41);
            //GLCD_Clear();
            // sensor_power_on();
            // _delay_ms(10); // Allow power to settle
            // int val = analog_read(SENSOR_ANALOG_PIN);
            // sensor_power_off();
            // int moisturePercentage = 100 - ((float)val / 1023) * 100;

            // GLCD_GotoXY(1, 1);
            // GLCD_PrintString("Soil moisture: ");
            // GLCD_PrintInteger(moisturePercentage);
            // GLCD_PrintString("%");  

            DHT_Status status = DHT_Get(&temperature_int, &humidity_int);
            if (status == DHT_Status_Ok) {
                GLCD_GotoXY(1, 16);
                GLCD_PrintString("Humidity: ");
                GLCD_PrintInteger(humidity_int / 10);
                GLCD_PrintString("%   ");
                counter++; 
                GLCD_PrintInteger(counter);

                GLCD_GotoXY(1, 31);
                GLCD_PrintString("Temperature: ");
                GLCD_PrintInteger(temperature_int / 10);
                GLCD_PrintString("c");
                GLCD_Render();
            }
            else {
                GLCD_GotoXY(1, 16);
                GLCD_PrintInteger(status);
            }
            
            // GLCD_GotoXY(1, 41);
            // GLCD_PrintString("Pump: OFF");
            // GLCD_GotoXY(60, 41);
            // GLCD_PrintString("- Tics: ");
            // counter++; 
            // GLCD_PrintInteger(counter);
            // GLCD_Render();
            _delay_ms(10000);
    }

    return 0;
}