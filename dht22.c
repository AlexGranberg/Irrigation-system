#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "bitmacro.h"
#include "dht22.h" 
void beginSendRequest();
void resetDHT();

DHT_Status DHT_Get(uint16_t *temperature, uint16_t *humidity) {

	uint8_t bits[5];
	uint8_t i,j = 0;

	memset(bits, 0, sizeof(bits));

	beginSendRequest();

	BIT_SET(DHT_PORT,DHT_INPUTPIN); // high
	BIT_CLEAR(DHT_DDR,DHT_INPUTPIN); // input

	_delay_us(40);

	//Should now be low
	if(BIT_CHECK(DHT_PIN,DHT_INPUTPIN)){
		return DHT_TimoutError_1;
	}
	_delay_us(80);
	
	//should now be high
	if(!BIT_CHECK(DHT_PIN,DHT_INPUTPIN)){
		return DHT_TimoutError_2;
	}
	_delay_us(80);

	//read-in data
	uint16_t timeoutcounter = 0;
	for (j=0; j<5; j++) { //for each byte (5 total)
		uint8_t result = 0;
		for(i=0; i<8; i++) {//for each bit in each byte (8 total)
			timeoutcounter = 0;
			while(!(BIT_CHECK(DHT_PIN,DHT_INPUTPIN))) { //wait for an high input (non blocking)
				timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					return DHT_TimoutError_3;
				}
			}
			_delay_us(30);
			if(BIT_CHECK(DHT_PIN,DHT_INPUTPIN))
				result |= (1<<(7-i));
			timeoutcounter = 0;
			while(BIT_CHECK(DHT_PIN,DHT_INPUTPIN)) {
				timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					return DHT_TimoutError_4;
				}
			}
		}
		bits[j] = result;
	}

	resetDHT();
	//compare checksum
	if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {
		//return temperature and humidity
		*temperature = bits[2]<<8 | bits[3];
		*humidity = bits[0]<<8 | bits[1];
		
		return DHT_Status_Ok;
	}

	return DHT_Checksum_Error;
}

void resetDHT(){
	BIT_SET(DHT_DDR,DHT_INPUTPIN);
	BIT_SET(DHT_PORT,DHT_INPUTPIN);
}

void beginSendRequest() {

	BIT_SET(DHT_DDR, DHT_INPUTPIN); //output
	BIT_CLEAR(DHT_PORT, DHT_INPUTPIN); //low
	_delay_us(1200);
}
