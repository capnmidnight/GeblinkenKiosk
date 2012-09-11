#include <arduino.h>
#include "kiosk.h"



void ledOff(int p)
{
  // this sets the pin into a high-impedance state, essentially "disconnecting" it from the circuit
  pinMode(p, INPUT);
  digitalWrite(p, LOW);
}
void ledOn(int p)
{
  pinMode(p, OUTPUT);
  digitalWrite(p, p < S1 ? LOW : HIGH);
}
void lightButton(int bank, int button)
{
  ledOn(MIN_INPUT_COL + bank * NUM_TRACKS + button);
}
void dimButton(int bank, int button)
{
  ledOff(MIN_INPUT_COL + bank * NUM_TRACKS + button);
}

void allLightsOff()
{
  static int i;
  for(i = MIN_OUTPUT_PIN; i <= MAX_OUTPUT_PIN; ++i)
  {
    ledOff(i);
  }
}
void setScoreLights(int score)
{
  static int i;
  int last = ((score * 120 / NUM_TRACKS) + 5) / 10;
  for(i = 0; i < last; ++i)
  {
    lightButton(2, i);
  }
  for(i = last; i < 12; ++i)
  {
    dimButton(2, i);
  }
}


boolean buttonWasPressed = false;
boolean wasAnyButtonPressed(){return buttonWasPressed;}

byte buttonState[2][9] = {  {0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0}};
                            
int readBank(int column)
{
  static int voltage;
  voltage = analogRead(column);
  // these need to be selected based on the resistors in the analog button encoders
  if(voltage < 209) return 0;
  else if(voltage < 465) return 1;
  else if(voltage < 577) return 2;
  else if(voltage < 670) return 3;
  else if(voltage < 727) return 4;
  else if(voltage < 766) return 5;
  else if(voltage < 794) return	6;
  return 7;
}

void readInput()
{
  static int state, bank, col_start, col;
  buttonWasPressed = false;
  for(col = MIN_INPUT_COL; col <= MAX_INPUT_COL; ++col)
  {  
    state = readBank(col);
    buttonWasPressed |= (state > 0);
    
    bank = (col - MIN_INPUT_COL) / 3;
    col_start = ((col - MIN_INPUT_COL) % 3) * 3;
    for(int button = 0; button < 3; ++button)
    {
      buttonState[bank][col_start + button] = state & 1;
      state >>= 1;
    }
  }
}

boolean getButtonPressed(int bank, int button)
{
  return buttonState[bank][button];
}
