#include <arduino.h>
#include "intro.h"
#include "kiosk.h"

char* enter_INTRO(GameState& game)
{
  static int track;
  game.score = 0;
  for(track = 0; track < NUM_TRACKS; ++track)
    game.trackPlayed[track] = false;
  game.tracksPlayed = 0;
  return "intro.mp3";
}

States body_INTRO(int dt, GameState& game)
{
  if(wasAnyButtonPressed() || game.mp3.isPlaying())
    return SELECT_TRACK;
  return INTRO;
}
