#include "game_over.h"
#include "kiosk.h"

char* enter_GAME_OVER(GameState& game)
{
  setScoreLights(game.score);
  if(game.score == NUM_TRACKS)
    return "win.mp3";
  else
    return "lose.mp3";
}

States body_GAME_OVER(int dt, GameState& game)
{
  if(game.mp3.isPlaying())
    return GAME_OVER;
  return ATTRACT;
}
