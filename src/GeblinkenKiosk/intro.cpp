#include <arduino.h>
#include "intro.h"
#include "kiosk.h"

void enter_INTRO(GameState& game)
{
  static int track;
  game.score = 0;
  for(track = 0; track < NUM_TRACKS; ++track)
    game.trackPlayed[track] = false;
  game.tracksPlayed = 0;
#ifndef MP3_DISABLE
  game.mp3.playMP3("intro.mp3");
#endif
}

States body_INTRO(int dt, GameState& game)
{
  if(wasAnyButtonPressed()
#ifndef MP3_DISABLE
  || game.mp3.isPlaying()
#endif
  )
  {
    return SELECT_TRACK;
  }
  return INTRO;
}
