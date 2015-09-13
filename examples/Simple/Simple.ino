#include <stdint.h>
#include <MM545x.h>

#define MM_CLOCK_PIN		10
#define MM_DATA_PIN		9

#define OUTPUT_COUNT		34

MM545x mm(MM_CLOCK_PIN, MM_DATA_PIN);

void setup()
{
}

void loop()
{        
        int i;
        for (i = 0; i < OUTPUT_COUNT; i++) {
                mm.setLeds((uint64_t) 1 << i);
                delay(200);
        }
}
