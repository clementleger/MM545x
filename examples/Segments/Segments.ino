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
        mm.setupSegment(0, sev1, 0);
        mm.setupSegment(1, sev2, 0);
        mm.setupSegment(2, sev3, 0);
}

 
void loop()
{       
  mm.setSegmentRaw(0,0b00000001);
  mm.refreshSegments();
  delay(200);
  mm.setSegmentRaw(0,0b00000010);
  mm.refreshSegments();
  delay(200);
  mm.setSegmentRaw(0,0b00000100);
  mm.refreshSegments();
  delay(200);
  mm.setSegmentRaw(0,0b00001000);
  mm.refreshSegments();
  delay(200);
  mm.setSegmentRaw(0,0);
  mm.setSegmentRaw(1,0b00001000);
  mm.refreshSegments();
  delay(200);
  mm.setSegmentRaw(1,0);
  mm.setSegmentRaw(2,0b00001000);
  mm.refreshSegments();
  delay(200);
}
