#include <arduino.h>
#include "select.h"
#include "kiosk.h"

int SELECT_nextReminder;
int SELECT_reminderCount;
void enter_SELECT_TRACK(GameState& game)
{
  static int i;
  SELECT_nextReminder = 0;
  SELECT_reminderCount = 0;
  for(i = 0; i < NUM_TRACKS; ++i)
  {
    if(!game.trackPlayed[i])
      lightButton(0, i);
  }
  setScoreLights(game.score);
#ifndef MP3_DISABLE
  game.mp3.playMP3("seltrack.mp3");
#endif
}

States body_SELECT_TRACK(int dt, GameState& game)
{
  static int i;
#ifndef MP3_DISABLE
  if(game.mp3.isPlaying())
  {
    return SELECT_TRACK;
  }
  else 
#endif
  if(dt > SELECT_REMINDER_DELTA * 6)
    return ATTRACT;
  else if(wasAnyButtonPressed())
  {
    for(i = 0; i < NUM_TRACKS; ++i)
    {
      if(getButtonPressed(0, i) && !game.trackPlayed[i])
      {
        game.selectedTrack = i;
        ++game.tracksPlayed;
        game.trackPlayed[i] = true;
        return PLAY_TRACK;
      }
    }
  }
  return SELECT_TRACK;
}

void enter_SELECT_REGION(GameState& game)
{
  static int i;
  SELECT_nextReminder = 0;
  SELECT_reminderCount = 0;
  lightButton(0, game.selectedTrack);
  for(i = 0; i < NUM_TRACKS; ++i)
  {
    lightButton(1, i);
  }
  setScoreLights(game.score);
#ifndef MP3_DISABLE
  game.mp3.playMP3("selreg.mp3");
#endif
}

States body_SELECT_REGION(int dt, GameState& game)
{
  static int i;
#ifndef MP3_DISABLE
  if(game.mp3.isPlaying())
    return SELECT_REGION;
  else
#endif
  if(dt > SELECT_REMINDER_DELTA * 6)
    return ATTRACT;
  else if(wasAnyButtonPressed())
  {
    for(i = 0; i < NUM_TRACKS; ++i)
    {
      if(getButtonPressed(1, i))
      {
        game.selectedRegion = i;
        return PLAY_REGION;
      }
    }
  }
  return SELECT_REGION;
}

