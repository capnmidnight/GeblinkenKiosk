#include "play.h"
#include "kiosk.h"

void enter_PLAY_TRACK(GameState& game)
{
  lightButton(0, game.selectedTrack);
  setScoreLights(game.score);
#ifndef MP3_DISABLE
  game.mp3.playTrack(TRACK_MAP[game.selectedTrack]);
#endif
}

States body_PLAY_TRACK(int dt, GameState& game)
{
#ifndef MP3_DISABLE
  if(game.mp3.isPlaying())
    return PLAY_TRACK;
#else
  if(!wasAnyButtonPressed())
    return PLAY_TRACK;
  else
#endif
    return SELECT_REGION;
}

void enter_PLAY_REGION(GameState& game)
{
  lightButton(0, game.selectedTrack);
  lightButton(1, game.selectedRegion);
  setScoreLights(game.score);
#ifndef MP3_DISABLE
  game.mp3.playTrack(REGION_MAP[game.selectedRegion]);
#endif
}

States body_PLAY_REGION(int dt, GameState& game)
{
  if(
#ifndef MP3_DISABLE
  game.mp3.isPlaying()
#else
  !wasAnyButtonPressed()
#endif
  )
    return PLAY_REGION;
  else  
  {
    if(ANSWER_MAP[game.selectedTrack] == game.selectedRegion)
    {
      return CORRECT_ANSWER;
    }
    else
    {
      return INCORRECT_ANSWER;
    }
  }
}
