#include <MM545x.h>
#include <Arduino.h>


/**
 * Delay between clock transitions
 */
#define  CLOCK_USEC_DELAY	2

/**
 *  Count of output bits
 */
#define  OUTPUT_BIT_COUNT		35

MM545x::MM545x(int clock_pin, int data_pin):
	clock_pin(clock_pin),
	data_pin(data_pin)
{
	pinMode(clock_pin, OUTPUT);
        pinMode(data_pin, OUTPUT);
        digitalWrite(clock_pin, LOW);
        digitalWrite(data_pin, LOW);
        
        /* reset the leds */
        setLeds((uint64_t) 0);
}


void MM545x::setLeds(uint64_t leds)
{
        int i;
	
	/* Send the preamble */
        digitalWrite(data_pin, HIGH);
        digitalWrite(clock_pin, LOW);
        delayMicroseconds(CLOCK_USEC_DELAY);
        digitalWrite(clock_pin, HIGH);
        
        /* Then output the bits */
        for (i = 0; i < OUTPUT_BIT_COUNT; i++) {
                delayMicroseconds(CLOCK_USEC_DELAY);
                if ((leds >> i) & 0x1)
                        digitalWrite(data_pin, HIGH);
                else
                        digitalWrite(data_pin, LOW);
                digitalWrite(clock_pin, LOW);
                delayMicroseconds(2);
                digitalWrite(clock_pin, HIGH);
        }

        delayMicroseconds(CLOCK_USEC_DELAY);
        digitalWrite(clock_pin, LOW);
}
