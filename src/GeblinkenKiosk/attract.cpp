#include <arduino.h>
#include <SFEMP3Shield.h>
#include "attract.h"
#include "kiosk.h"

unsigned int ATTRACT_currentFrame;
unsigned int ATTRACT_lastFrame;

void enter_ATTRACT(GameState& game)
{
  ATTRACT_currentFrame = MIN_OUTPUT_PIN;
  ATTRACT_lastFrame = 0;
  // strobes run on their own, just turn them on.
  lightButton(2, 12);
  lightButton(2, 13);
#ifndef MP3_DISABLE
  game.mp3.playMP3("attract.mp3");
#endif
}

States body_ATTRACT(int dt, GameState& game)
{
  States next = ATTRACT;
  if((dt - ATTRACT_lastFrame) >= ATTRACT_MILLIS_PER_FRAME)
  {
    ATTRACT_lastFrame = dt;
    ledOff(ATTRACT_currentFrame);
    ++ATTRACT_currentFrame;
    if(ATTRACT_currentFrame > S12)
      ATTRACT_currentFrame = MIN_OUTPUT_PIN;
    ledOn(ATTRACT_currentFrame);
  }
  if(wasAnyButtonPressed())
  {
    next = INTRO;
  }
  return next;
}

