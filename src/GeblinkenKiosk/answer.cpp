#include <arduino.h>
#include "answer.h"
#include "kiosk.h"

char* enter_CORRECT_ANSWER(GameState& game)
{
  game.score++;
  setScoreLights(game.score);
  lightButton(2, 12);
  return "right.mp3";
}

States body_CORRECT_ANSWER(int dt, GameState& game)
{
  if(game.mp3.isPlaying())
    return CORRECT_ANSWER;
  else if(game.tracksPlayed == NUM_TRACKS)
    return GAME_OVER;
  return SELECT_TRACK;
}

char* enter_INCORRECT_ANSWER(GameState& game)
{
  setScoreLights(game.score);
  lightButton(2, 13);
  return "wrong.mp3";
}

States body_INCORRECT_ANSWER(int dt, GameState& game)
{
  if(game.mp3.isPlaying())
    return INCORRECT_ANSWER;
  else if(game.tracksPlayed == NUM_TRACKS)
    return GAME_OVER;
  return SELECT_TRACK;
}
