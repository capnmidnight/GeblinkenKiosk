#ifndef __IO_H
#define __IO_H
#include <arduino.h>

const int NUM_LED_OUTPUTS = 36;
const int LED_OUTPUT_MAP[NUM_LED_OUTPUTS+2] = {
  14, 15, 16, A0, 
  17, 18, 19, A1,
  20, 21, 22, A2,
  
  23, 24, 25, A3,
  26, 27, 28, A4,
  29, 30, 31, A5,
  
  32, 33, 34, 35, 36, 37,
  38, 39, 40, 41, 42, 43,
  
  44, 45
};

const int NUM_INPUTS = 8;
enum InputPins
{
  BANK1_COL1 = 8,
  BANK1_COL2,
  BANK1_COL3, 
  
  BANK2_COL1,
  BANK2_COL2,
  BANK2_COL3,
  
  BANK2_ROW4,
  BANK1_ROW4
};

const int MIN_INPUT_PIN = BANK1_COL1;
const int MAX_INPUT_PIN = BANK1_ROW4;
const int MIN_OUTPUT_PIN = 0;
const int MAX_OUTPUT_PIN = 35;
const int MIN_TRACK_PIN = 0;
const int MAX_TRACK_PIN = 11;
const int MIN_REGION_PIN = 12;
const int MAX_REGION_PIN = 23;
const int MIN_SCORE_PIN = 24;
const int MAX_SCORE_PIN = 35;
const int MAX_STROBE_PIN = 37;

void ledOff(int);
void ledOn(int);
int readColumn(int);
boolean scanInput(boolean[2][12]);

#endif
