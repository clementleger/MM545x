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


uint8_t digitPattern[] = {
        0x3F,     /* 0 */
        0x06,     /* 1 */
        0x5B,     /* 2 */
        0x4F,     /* 3 */
        0x66,     /* 4 */
        0x6D,     /* 5 */
        0x7D,     /* 6 */
        0x07,     /* 7 */
        0x7F,     /* 8 */
        0x6F,     /* 9 */
};


MM545x::MM545x(int clock_pin, int data_pin):
	clock_pin(clock_pin),
	data_pin(data_pin)
{
        int i;

	pinMode(clock_pin, OUTPUT);
        pinMode(data_pin, OUTPUT);
        digitalWrite(clock_pin, LOW);
        digitalWrite(data_pin, LOW);
        
        for (i = 0; i < MAX_SEGMENTS_PER_MM545X; i++) {
                memset(sevSegPins[i], 0, SEGMENT_COUNT);
                sevSegValue[i] = 0;
        }
        

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

void MM545x::setupSegment(int sevSeg, uint8_t pins[SEGMENT_COUNT])
{
        memcpy(sevSegPins[sevSeg], pins, SEGMENT_COUNT);
}

void MM545x::setSegment(int sevSeg, char value)
{
        if (value >= '0' && value <= '9') {
                setSegmentRaw(sevSeg, digitPattern[value - '0']);
        }
}

void MM545x::setSegmentRaw(int sevSeg, uint8_t segMask)
{
        Serial.print("Setting seg ");
        Serial.print(sevSeg);
        Serial.print(" to mask ");
        Serial.println(segMask, HEX);
        sevSegValue[sevSeg] = segMask;
}

void MM545x::refreshSegments()
{
        uint64_t value = 0;
        int sevSeg, seg;
        for (sevSeg = 0; sevSeg < MAX_SEGMENTS_PER_MM545X; sevSeg++) {
                for (seg = 0; seg < SEGMENT_COUNT; seg++) {
                        /* If the bit is set in the value, then set the pin as high */
                        if ((sevSegValue[sevSeg] >> seg) & 0x1) {
                                value |= ((uint64_t) 0x1 << (uint64_t) sevSegPins[sevSeg][seg]);
                        }
                }
        }

        setLeds(value);
}
