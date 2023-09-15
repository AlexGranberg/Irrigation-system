#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

void timer_init();
uint8_t timer_elapsed(uint32_t interval_ms);

#endif
