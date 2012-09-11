#include "play.h"
#include "kiosk.h"

char* TRACK_MAP[NUM_TRACKS] = {"track002.mp3", "track007.mp3", "track004.mp3", "track006.mp3", "track008.mp3", "track000.mp3", "track001.mp3", "track005.mp3", "track003.mp3"};
char* REGION_MAP[NUM_TRACKS] = {"track014.mp3", "track009.mp3", "track015.mp3", "track013.mp3", "track017.mp3", "track016.mp3", "track012.mp3", "track011.mp3", "track010.mp3"};

char* enter_PLAY_TRACK(GameState& game)
{
  lightButton(0, game.selectedTrack);
  setScoreLights(game.score);
  return TRACK_MAP[game.selectedTrack];
}

States body_PLAY_TRACK(int dt, GameState& game)
{
  if(game.mp3.isPlaying() && !wasAnyButtonPressed())
    return PLAY_TRACK;
  return SELECT_REGION;
}

char* enter_PLAY_REGION(GameState& game)
{
  lightButton(0, game.selectedTrack);
  lightButton(1, game.selectedRegion);
  setScoreLights(game.score);
  return REGION_MAP[game.selectedRegion];
}

States body_PLAY_REGION(int dt, GameState& game)
{
  if(game.mp3.isPlaying() && wasAnyButtonPressed())
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
