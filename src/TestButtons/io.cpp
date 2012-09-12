#include "io.h"
#include <arduino.h>

void ledOff(int p){
  pinMode(LED_OUTPUT_MAP[p], INPUT);
  digitalWrite(LED_OUTPUT_MAP[p], LOW);
}



void ledOn(int p)
{
  pinMode(LED_OUTPUT_MAP[p], OUTPUT);

  if(MIN_SCORE_PIN <= p && p <= MAX_STROBE_PIN)  // the score LEDs are switched by transistors
    digitalWrite(LED_OUTPUT_MAP[p], HIGH);
  else // the button LEDs are driven to ground through the MCU
  digitalWrite(LED_OUTPUT_MAP[p], LOW);
}

int readColumn(int column){
  static int readCount[NUM_INPUTS] = {0, 0, 0, 0, 0, 0, 0, 0};
  int voltage = analogRead(column);
  //software debouncing
  if(voltage == 0)
    readCount[column - MIN_INPUT_PIN] = 0;
  else
    ++readCount[column - MIN_INPUT_PIN];
  if(readCount[column - MIN_INPUT_PIN] < 5 || voltage < 203) return 0;
  else if(voltage < 450) return 1;
  else if(voltage < 560) return 2;
  else if(voltage < 649) return 3;
  else if(voltage < 704) return 4;
  else if(voltage < 742) return 5;
  else if(voltage < 769) return 6;
  return 7;
}

boolean scanInput(boolean buffer[2][12])
{
  boolean buttonWasPressed = 0;
  int state;
  
  state = readColumn(BANK1_COL1);
  buffer[0][0] = state & 1;
  buffer[0][1] = (state & 2) >> 1;
  buffer[0][2] = (state & 4) >> 2;
  // buffer[0][3] filled on the BANK1_ROW4
  
  state = readColumn(BANK1_COL2);
  buffer[0][4] = state & 1;
  buffer[0][5] = (state & 2) >> 1;
  buffer[0][6] = (state & 4) >> 2;
  // buffer[0][7] filled on the BANK1_ROW4
  
  state = readColumn(BANK1_COL3);
  buffer[0][8] = state & 1;
  buffer[0][9] = (state & 2) >> 1;
  buffer[0][10] = (state & 4) >> 2;
  // buffer[0][11] filled on the BANK1_ROW4
  
  // this just happens to be how it is wired
  state = readColumn(BANK1_ROW4);
  buffer[0][11] = state & 1;
  buffer[0][7] = (state & 2) >> 1;
  buffer[0][3] = (state & 4) >> 2;
  
  
  state = readColumn(BANK2_COL1);
  buffer[1][0] = state & 1;
  buffer[1][1] = (state & 2) >> 1;
  buffer[1][2] = (state & 4) >> 2;
  // buffer[1][3] filled on the BANK2_ROW4
  
  state = readColumn(BANK2_COL2);
  buffer[1][4] = state & 1;
  buffer[1][5] = (state & 2) >> 1;
  buffer[1][6] = (state & 4) >> 2;
  // buffer[1][7] filled on the BANK2_ROW4
  
  state = readColumn(BANK2_COL3);
  buffer[1][8] = state & 1;
  buffer[1][9] = (state & 2) >> 1;
  buffer[1][10] = (state & 4) >> 2;
  // buffer[1][11] filled on the BANK2_ROW4
  
  // this just happens to be how it is wired
  state = readColumn(BANK2_ROW4);
  buffer[1][11] = state & 1;
  buffer[1][7] = (state & 2) >> 1;
  buffer[1][3] = (state & 4) >> 2;
  
  // just gonna reuse the state variable for no good reason other
  // than "waste not, want not"
  for(state = 0; state < 12; ++state)
    if(buffer[0][state] | buffer[1][state])
      return true;
  return false;
}

