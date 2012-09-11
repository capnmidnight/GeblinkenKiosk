#include "game_over.h"
#include "kiosk.h"

void enter_GAME_OVER(GameState& game)
{
  setScoreLights(game.score);
#ifndef MP3_DISABLE
  if(game.score == NUM_TRACKS)
  {
    game.mp3.playMP3("win.mp3");
  }
  else
  {
    game.mp3.playMP3("lose.mp3");
  }
#endif
}

States body_GAME_OVER(int dt, GameState& game)
{
#ifndef MP3_DISABLE
  if(game.mp3.isPlaying())
    return GAME_OVER;
#endif
  return ATTRACT;
}
