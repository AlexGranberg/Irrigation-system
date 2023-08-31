#ifndef __DHT_H__
#define __DHT_H__


#include <stdio.h>
#include <avr/io.h>

#define DHT_DDR DDRD
#define DHT_PORT PORTD
#define DHT_PIN PIND
#define DHT_INPUTPIN 2

#define DHT_TIMEOUT 200

typedef enum {
	DHT_Status_Ok = 0, // 0
	DHT_Status_Error = 1,
	DHT_Checksum_Error = 2,
	DHT_TimoutError_1 = 3,
	DHT_TimoutError_2 = 4,
	DHT_TimoutError_3 = 5,
	DHT_TimoutError_4= 6
} DHT_Status;

DHT_Status DHT_Get(uint16_t *temperature, uint16_t *humidity);

#endif