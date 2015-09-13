#include <stdint.h>
#include <MM545x.h>

#define MM_CLOCK_PIN        10
#define MM_DATA_PIN         9

MM545x mm(MM_CLOCK_PIN, MM_DATA_PIN);

/* Define some segments connected to the MM545x */
uint8_t sev1[8] = {5, 4, 1, 2, 3, 6, 7, 0};
uint8_t sev2[8] = {13, 12, 9, 10, 11, 14, 15, 8};
uint8_t sev3[8] = {21, 20, 17, 18, 19, 22, 23, 16};

void setup()
{
        Serial.begin(115200);
        /* Setup the segments pins */
        mm.setupSegment(0, sev1);
        mm.setupSegment(1, sev2);
        mm.setupSegment(2, sev3);
}

 
void loop()
{       
        int i, j, k;

        for (i = 0; i < 10; i++) {
                mm.setSegment(2, '0' + i);
                for (j = 0; j < 10; j++) {
                        mm.setSegment(1, '0' + j);
                        for (k = 0; k < 10; k++) {
                                mm.setSegment(0, '0' + k);
                                mm.refreshSegments();
                                delay(50);
                        }
                }
        }
}
