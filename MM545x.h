#ifndef MM545X_H
#define MM545X_H

#include <stdint.h>

class MM545x {
private:
	int clock_pin;
	int data_pin;
public:
	MM545x(int clock_pin, int data_pin);
	void setLeds(uint64_t bits);
};


#endif
