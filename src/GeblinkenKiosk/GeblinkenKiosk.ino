#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h> 
#include <SFEMP3Shield.h>
#include "kiosk.h"
#include "attract.h"
#include "intro.h"
#include "select.h"
#include "play.h"
#include "answer.h"
#include "game_over.h"

States currentState, nextState;
GameState game;
int stateStarted, currentFrame;

enter_function enter[NUM_STATES] = {&enter_ATTRACT, &enter_INTRO, &enter_SELECT_TRACK, &enter_PLAY_TRACK, &enter_SELECT_REGION, &enter_PLAY_REGION, &enter_CORRECT_ANSWER, &enter_INCORRECT_ANSWER, &enter_GAME_OVER};
body_function body[NUM_STATES] = { &body_ATTRACT, &body_INTRO, &body_SELECT_TRACK, &body_PLAY_TRACK, &body_SELECT_REGION, &body_PLAY_REGION, &body_CORRECT_ANSWER, &body_INCORRECT_ANSWER, &body_GAME_OVER};


void setup() 
{
  static int i;
  currentState = NONE;
  nextState = ATTRACT;
  game.mp3.begin();
  for(i = MIN_OUTPUT_PIN; i < MAX_OUTPUT_PIN; ++i)
  {
    pinMode(i, OUTPUT);
  }	
}

char* enterState()
{
  if(enter[currentState] != NULL)
    return (*enter[currentState])(game);
  return NULL;
}
States updateState(int dt)
{
  if(body[currentState] != NULL)
    return (*body[currentState])(dt, game);
  return currentState;
}
void loop() 
{
  if(nextState != currentState)
  {
    if(game.mp3.isPlaying())
    {
      game.mp3.stopTrack();
    }
    allLightsOff();
    delay(500); // give the user some time to let go of the button
    currentState = nextState;
    stateStarted = millis();
    game.mp3.playMP3(enterState());
  }
  readInput();
  currentFrame = millis();  
  nextState = updateState(currentFrame - stateStarted);
}

