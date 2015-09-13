#ifndef MM545X_H
#define MM545X_H

#include <stdint.h>

/**
 * Maximum number of seven segments per chip
 */
#define MAX_SEGMENTS_PER_MM545X	4

/**
 * Number of segment per display
 */
#define SEGMENT_COUNT	8

class MM545x {
private:
	int clock_pin;
	int data_pin;
	uint8_t sevSegPins[MAX_SEGMENTS_PER_MM545X][SEGMENT_COUNT];
	uint8_t sevSegValue[MAX_SEGMENTS_PER_MM545X];
public:
	/**
	 * Initialize a MM545x object
	 * @param clock_pin Pin connected to the clock input of MM545x chip
	 * @param data_pin Pin connected to the serial data input of MM545x chip
	 * @return a MM545x object
	 */
	MM545x(int clock_pin, int data_pin);
	
	/**
	 * Set the mask for all leds, useful when working with other leds than 7 segments
	 * @param bits A bitmask of all output (only the first 35 bits are significant)
	 * 		The lower bit correspond to output 0.
	 */
	void setLeds(uint64_t bits);
	
	/**
	 *  When using the chip as a seven segment controller, setup segments
	 * @param sevSeg Seven sigment display index
	 * @param pins, pin mapping for the seven segment
	 * 		Pins are given in the following order: [a, b, c, d ,e , f, g, dp]
	 */
	void setupSegment(int sevSeg, uint8_t pins[SEGMENT_COUNT]);
	
	/**
	 * Set a segment value
	 * @param sevSeg Seven sigment display index
	 * @param value Character value to display 
	 */
	void setSegment(int sevSeg, char value);
	
	/**
	 * Set a raw value for a segment
	 * @param sevSeg Seven sigment display index
	 * @param segMask Mask of segment to set (a, b, c, d ,e , f, g, dp)
	 * 		Note that the lower bit correspond to segment a.
	 */
	void setSegmentRaw(int sevSeg, uint8_t segMask);
	
	/**
	 *  Update the display and apply the previously set segments value. 
	 */
	void refreshSegments();
};


#endif
