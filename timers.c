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
