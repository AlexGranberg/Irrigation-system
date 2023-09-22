#include <util/delay.h>
#include "yl69.h"
#include "SSD1306.h"

void read_Soil(){
    sensor_power_on();
    _delay_ms(10); // Allow power to settle
    int val = analog_read(SENSOR_ANALOG_PIN);
    sensor_power_off();
    int moisturePercentage = 100 - ((float)val / 1023) * 100;

    GLCD_GotoXY(1, 1);
    GLCD_PrintString("Soil moisture: ");
    GLCD_PrintInteger(moisturePercentage);
    GLCD_PrintString("%");   
}