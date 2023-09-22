#include <avr/io.h>
#include <avr/interrupt.h>
#include "timers.h"

static volatile uint32_t timer_ticks = 0;

ISR(TIMER1_OVF_vect) {
    timer_ticks++;
}

void timer_init() {
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

uint8_t timer_elapsed(uint32_t interval_ms) {
    if (timer_ticks * 1000 >= interval_ms) {
        timer_ticks = 0; // Reset timer
        return 1; // Interval has elapsed
    }
    return 0; // Interval has not elapsed
}

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