#include <arduino.h>
#include "answer.h"
#include "kiosk.h"

void enter_CORRECT_ANSWER(GameState& game)
{
  game.score++;
  setScoreLights(game.score);
  lightButton(2, 12);
#ifndef MP3_DISABLE
  game.mp3.playMP3("right.mp3");
#endif
}

States body_CORRECT_ANSWER(int dt, GameState& game)
{
#ifndef MP3_DISABLE
  if(game.mp3.isPlaying())
    return CORRECT_ANSWER;
#endif    
  return game.tracksPlayed == NUM_TRACKS ? GAME_OVER : SELECT_TRACK;
}

void enter_INCORRECT_ANSWER(GameState& game)
{
  setScoreLights(game.score);
  lightButton(2, 13);
#ifndef MP3_DISABLE
  game.mp3.playMP3("wrong.mp3");
#endif
}

States body_INCORRECT_ANSWER(int dt, GameState& game)
{
#ifndef MP3_DISABLE
  if(game.mp3.isPlaying())
    return INCORRECT_ANSWER;
#endif
  return game.tracksPlayed == NUM_TRACKS ? GAME_OVER : SELECT_TRACK;
}
