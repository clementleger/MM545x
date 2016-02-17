#include <MM545x.h>
#include <Arduino.h>


/**
 * Delay between clock transitions
 */
#define  CLOCK_USEC_DELAY	1

/**
 *  Count of output bits
 */
#define  OUTPUT_BIT_COUNT	35


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

uint8_t alphaPattern[] = {
        0b01110111,     /* a */
        0b01111100,     /* b */
        0b00111001,     /* c */
        0b01011110,     /* d */
        0b01111001,     /* e */
        0b01110001,     /* f */
        0b01101111,     /* g */
        0b01110110,     /* h */
        0b00110000,     /* i */
        0b00011110,     /* j */
        0b01110110,     /* k */
        0b00111000,     /* l */
        0b00010101,     /* m */
        0b01010100,     /* n */
        0b00111111,     /* o */
        0b01110011,     /* p */
        0b01100111,     /* q */
        0b01010000,     /* r */
        0b01101101,     /* s */
        0b01111000,     /* t */
        0b00111110,     /* u */
        0b00011100,     /* v */
        0b00101010,     /* w */
        0b01110110,     /* x */
        0b01101110,     /* y */
        0b01011011,     /* z */
};

#define COMMA 		0b10000000
#define DASH 		0b01000000
#define UNDERSCORE 	0b00001000
#define QUOTE 		0b00100000
#define EQUAL 		0b01001000


MM545x::MM545x(int clock_pin, int data_pin):
	clock_pin(clock_pin),
	data_pin(data_pin)
{
        int i;
        current_duplex = -1;

	pinMode(clock_pin, OUTPUT);
        pinMode(data_pin, OUTPUT);
        digitalWrite(clock_pin, LOW);
        digitalWrite(data_pin, LOW);
        
        for (i = 0; i < MM545X_MAX_SEVEN_SEGMENTS; i++) {
                memset(sev_seg[i].pins, 0, MM545X_SEVEN_SEGMENT_COUNT);
        }
	duplex_value[0] = 0;
	duplex_value[1] = 0;

        /* reset the leds */
        setLeds((uint64_t) 0);
        
}

void MM545x::setLeds(uint64_t leds)
{
        int i;

	/* Send the preamble */
        digitalWrite(data_pin, HIGH);
        digitalWrite(clock_pin, LOW);
        //~ delayMicroseconds(CLOCK_USEC_DELAY);
        digitalWrite(clock_pin, HIGH);

        /* Then output the bits */
        for (i = 0; i < OUTPUT_BIT_COUNT; i++) {
                //~ delayMicroseconds(CLOCK_USEC_DELAY);
                if ((leds >> i) & 0x1)
                        digitalWrite(data_pin, HIGH);
                else
                        digitalWrite(data_pin, LOW);
                digitalWrite(clock_pin, LOW);
                //~ delayMicroseconds(CLOCK_USEC_DELAY);
                digitalWrite(clock_pin, HIGH);
        }

        //~ delayMicroseconds(CLOCK_USEC_DELAY);
        digitalWrite(clock_pin, LOW);
}

void MM545x::setupSegment(int sevSeg, uint8_t pins[MM545X_SEVEN_SEGMENT_COUNT], uint8_t duplex)
{
        memcpy(sev_seg[sevSeg].pins, pins, MM545X_SEVEN_SEGMENT_COUNT);
        sev_seg[sevSeg].duplex = duplex;
}

void MM545x::setSegment(int sevSeg, char value)
{
	if (value >= '0' && value <= '9') {
			setSegmentRaw(sevSeg, digitPattern[value - '0']);
	} else if (value >= 'a' && value <= 'z') {
			setSegmentRaw(sevSeg, alphaPattern[value - 'a']);
	} else if (value >= 'A' && value <= 'Z') {
			setSegmentRaw(sevSeg, alphaPattern[value - 'A']);
	} else if (value == ' ') {
                setSegmentRaw(sevSeg, 0x0);
	} else if (value == ',' || value == ';' || value == '.') {
                setSegmentRaw(sevSeg, COMMA);
	} else if (value == '_') {
                setSegmentRaw(sevSeg, UNDERSCORE);
	} else if (value == '-') {
                setSegmentRaw(sevSeg, DASH);
	} else if (value == '\'') {
                setSegmentRaw(sevSeg, QUOTE);
	} else if (value == '=') {
                setSegmentRaw(sevSeg, EQUAL);
	} else {
                setSegmentRaw(sevSeg, 0x0);
	}
}

void MM545x::setSegmentRaw(int sevSeg, uint8_t segMask)
{
	struct sevSeg ss = sev_seg[sevSeg];
	uint8_t seg;
	uint64_t value = duplex_value[ss.duplex];
	
	for (seg = 0; seg < MM545X_SEVEN_SEGMENT_COUNT; seg++) {
		/* If the bit is set in the value, then set the pin as high */
		if ((segMask >> seg) & 0x1) {
			value = (uint64_t) value | (uint64_t) ((uint64_t) 0x1 << ss.pins[seg]);
		} else {
			value = ((uint64_t) value & (uint64_t) ~((uint64_t) 0x1 << ss.pins[seg]));
		}
	}
	noInterrupts();
	duplex_value[ss.duplex] = value;
	interrupts();
}


void MM545x::setupSegmentDuplex(uint8_t duplex_pins[2])
{
	duplex_value[0] = (uint64_t) ((uint64_t)0x1 << (uint64_t) duplex_pins[0]);
	duplex_value[1] = (uint64_t) ((uint64_t)0x1 << (uint64_t) duplex_pins[1]);
	current_duplex = 0;
}

void MM545x::print(const char *str)
{
	uint8_t ss = 0;

	while(str[ss] != 0 && ss < MM545X_MAX_SEVEN_SEGMENTS) {
		this->setSegment(ss++, str[ss]);
	}
	while(ss < MM545X_MAX_SEVEN_SEGMENTS)
		this->setSegmentRaw(ss++, 0);
}

void MM545x::refreshSegments()
{
	uint64_t value;

        if (current_duplex >= 0) {
		value = duplex_value[current_duplex];
		current_duplex = (current_duplex == 0 ? 1 : 0);
	} else {
		value = duplex_value[0];
	}

        setLeds(value);
}
