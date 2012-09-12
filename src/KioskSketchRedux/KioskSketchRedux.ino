#include <arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>
#include "KioskSketchRedux.h"

char* trackFiles[] = {
  "arnoldm.mp3",
  "begonial.mp3",
  "celestes.mp3",
  "glorias.mp3",
  "lorraine.mp3",
  "normab.mp3",
  "johnk.mp3",
  "sophyk.mp3",
  "tracym.mp3",
  "barryp.mp3",
  "dianes.mp3",
  "claraw.mp3"
};


static SFEMP3Shield player;
static struct Game game = { 
  NONE,
  ATTRACT,
  0,
  0,
  { 
    0, 0, 0, 0, 0, 0, 0, 0, 0   }
  ,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  { 
    { 
      0, 0, 0, 0, 0, 0, 0, 0, 0     }
    ,
    { 
      0, 0, 0, 0, 0, 0, 0, 0, 0     } 
  }
};

void ledOff(int pin)
{
  pinMode(LED_OUTPUT_MAP[pin], INPUT);
  digitalWrite(LED_OUTPUT_MAP[pin], LOW);
}


void ledOn(int pin)
{
  pinMode(LED_OUTPUT_MAP[pin], OUTPUT);

  if((MIN_SCORE_PIN <= pin && pin <= MAX_SCORE_PIN)
    || pin == GREEN_STROBE_PIN
    || pin == RED_STROBE_PIN)  // the score LEDs are switched by transistors
    digitalWrite(LED_OUTPUT_MAP[pin], HIGH);
  else // the button LEDs are driven to ground through the MCU
    digitalWrite(LED_OUTPUT_MAP[pin], LOW);
}

void rangeLightsOff(int first, int last){
  for( int i = first; i <= last; ++i ){
    ledOff(i);
  }
}

void allLightsOff(){
  rangeLightsOff(MIN_OUTPUT_PIN, MAX_OUTPUT_PIN);
}

void allTrackLightsOff(){
  rangeLightsOff(MIN_TRACK_PIN, MAX_TRACK_PIN);
}

void allRegionLightsOff(){
  rangeLightsOff(MIN_REGION_PIN, MAX_REGION_PIN);
}

void allTrackRegionLightsOff(){
  rangeLightsOff(MIN_TRACK_PIN, MAX_REGION_PIN);
}

void allScoreLightsOff(){
  rangeLightsOff(MIN_SCORE_PIN, MAX_SCORE_PIN);
}

void rangeLightsOn(int first, int last){
  for( int i = first; i <= last; ++i ){
    ledOn(i);
  }
}

void allLightsOn(){
  rangeLightsOn(MIN_OUTPUT_PIN, MAX_OUTPUT_PIN);
}

void allTrackLightsOn(){
  rangeLightsOn(MIN_TRACK_PIN, MAX_TRACK_PIN);
}

void allRegionLightsOn(){
  rangeLightsOn(MIN_REGION_PIN, MAX_REGION_PIN);
}

void allTrackRegionLightsOn(){
  rangeLightsOn(MIN_TRACK_PIN, MAX_REGION_PIN);
}

void allScoreLightsOn(){
  rangeLightsOn(MIN_SCORE_PIN, MAX_SCORE_PIN);
}

void setScoreLights(){
  rangeLightsOn(MIN_SCORE_PIN, MIN_SCORE_PIN + game.score - 1);
}



int readColumn(int column){
  static int readCount[NUM_INPUTS] = {0, 0, 0, 0, 0, 0, 0, 0};
  int voltage = analogRead(column);
  //software debouncing
  if(voltage == 0)
    readCount[column - MIN_INPUT_PIN] = 0;
  else
    ++readCount[column - MIN_INPUT_PIN];
  if(readCount[column - MIN_INPUT_PIN] < 5 || voltage < 203) return 0;
  else if(voltage < 450) return 1;
  else if(voltage < 560) return 2;
  else if(voltage < 649) return 3;
  else if(voltage < 704) return 4;
  else if(voltage < 742) return 5;
  else if(voltage < 769) return 6;
  return 7;
}

boolean scanInput(boolean buffer[2][12])
{
  boolean buttonWasPressed = 0;
  int state;
  
  state = readColumn(BANK1_COL1);
  buffer[0][0] = state & 1;
  buffer[0][1] = (state & 2) >> 1;
  buffer[0][2] = (state & 4) >> 2;
  // buffer[0][3] filled on the BANK1_ROW4
  
  state = readColumn(BANK1_COL2);
  buffer[0][4] = state & 1;
  buffer[0][5] = (state & 2) >> 1;
  buffer[0][6] = (state & 4) >> 2;
  // buffer[0][7] filled on the BANK1_ROW4
  
  state = readColumn(BANK1_COL3);
  buffer[0][8] = state & 1;
  buffer[0][9] = (state & 2) >> 1;
  buffer[0][10] = (state & 4) >> 2;
  // buffer[0][11] filled on the BANK1_ROW4
  
  // this just happens to be how it is wired
  state = readColumn(BANK1_ROW4);
  buffer[0][11] = state & 1;
  buffer[0][7] = (state & 2) >> 1;
  buffer[0][3] = (state & 4) >> 2;
  
  
  state = readColumn(BANK2_COL1);
  buffer[1][0] = state & 1;
  buffer[1][1] = (state & 2) >> 1;
  buffer[1][2] = (state & 4) >> 2;
  // buffer[1][3] filled on the BANK2_ROW4
  
  state = readColumn(BANK2_COL2);
  buffer[1][4] = state & 1;
  buffer[1][5] = (state & 2) >> 1;
  buffer[1][6] = (state & 4) >> 2;
  // buffer[1][7] filled on the BANK2_ROW4
  
  state = readColumn(BANK2_COL3);
  buffer[1][8] = state & 1;
  buffer[1][9] = (state & 2) >> 1;
  buffer[1][10] = (state & 4) >> 2;
  // buffer[1][11] filled on the BANK2_ROW4
  
  // this just happens to be how it is wired
  state = readColumn(BANK2_ROW4);
  buffer[1][11] = state & 1;
  buffer[1][7] = (state & 2) >> 1;
  buffer[1][3] = (state & 4) >> 2;
  
  // just gonna reuse the state variable for no good reason other
  // than "waste not, want not"
  for(state = 0; state < 12; ++state)
    if(buffer[0][state] | buffer[1][state])
      return true;      
  return false;
}

void setup(){
  player.begin();
  player.SetVolume(10, 10);
  allLightsOff();
  Serial.begin(9600);
}

void loop(){
  if( game.nextState != game.currentState ){
    if( player.isPlaying() ){
      player.stopTrack();
    }
    delay(500);
    switch( game.nextState ){
    case ATTRACT:
    case INTRO:
      allLightsOff();
      break;
    case SELECT_TRACK:
    case PLAY_TRACK:
    case GAME_OVER:
      allTrackRegionLightsOff();
      ledOff(GREEN_STROBE_PIN);
      ledOff(RED_STROBE_PIN);
      break;
    case PLAY_REGION:
      allRegionLightsOff();
      break;
    }
    game.currentState = game.nextState;
    game.stateStartTime = millis();
    switch( game.currentState ){
    case ATTRACT:
      game.stateVar1 = MIN_OUTPUT_PIN;
      game.stateVar2 = 0;
      game.stateVar3 = ATTRACT_MUSIC_FREQUENCY;
      //rangeLightsOn(RIGHT_ANSWER, WRONG_ANSWER);
      player.playMP3("attract.mp3");
      break;
    case INTRO:
      game.score = 0;
      for( int i = 0; i < NUM_TRACKS; ++i ){
        game.trackPlayed[i] = 0;
      }
      game.tracksPlayed = 0;
      player.playMP3("intro.mp3");
      break;
    case SELECT_TRACK:
      game.stateVar1 = SELECT_REMINDER_DELTA;
      game.stateVar2 = 1;
      for( int i = 0; i < NUM_TRACKS; ++i ){
        if( !game.trackPlayed[i] ){
          ledOn(MIN_TRACK_PIN + i);
        }
      }
      player.playMP3("seltrack.mp3");
      break;
    case PLAY_TRACK:
      ledOn(MIN_TRACK_PIN + game.selectedTrack);
      player.playMP3(trackFiles[game.selectedTrack]);
      break;
    case SELECT_REGION:
      game.stateVar1 = SELECT_REMINDER_DELTA;
      game.stateVar2 = 1;
      allRegionLightsOn();
      player.playMP3("selreg.mp3");
      break;
    case PLAY_REGION:
      ledOn(MIN_REGION_PIN + game.selectedRegion);
      break;
    case CORRECT_ANSWER:
      game.score++;
      setScoreLights();
      ledOn(GREEN_STROBE_PIN);
      player.playMP3("right.mp3");
      break;
    case INCORRECT_ANSWER:
      ledOn(RED_STROBE_PIN);
      player.playMP3("wrong.mp3");
      break;
    case GAME_OVER:
      if( game.score == NUM_TRACKS )
      {
        ledOn(GREEN_STROBE_PIN);
        player.playMP3("winfull.mp3");
      } 
      else if(game.score > NUM_TRACKS - 4)
      {
        ledOn(GREEN_STROBE_PIN);
        player.playMP3("winpart.mp3");
      }
      else 
      {
        ledOn(RED_STROBE_PIN);
        player.playMP3("lose.mp3");
      }
      break;
    }
  }
  game.buttonWasPressed = scanInput(game.buttonState);
  unsigned long iterationDelta = millis() - game.stateStartTime;
  switch( game.currentState ){
  case ATTRACT:
    if( iterationDelta > game.stateVar3 ){
      game.stateVar3 += ATTRACT_MUSIC_FREQUENCY;
      if( player.isPlaying() ){
        player.stopTrack();
      }
      player.playMP3("attract.mp3");
    }
    if( iterationDelta - game.stateVar2 >= ATTRACT_MILLIS_PER_FRAME ){
      game.stateVar2 = iterationDelta;
      ledOff(game.stateVar1++);
      if( game.stateVar1 > MAX_SCORE_PIN ){
        game.stateVar1 = MIN_OUTPUT_PIN;
      }
      ledOn(game.stateVar1);
    }
    if( game.buttonWasPressed ){
      game.nextState = INTRO;
    }
    break;
  case INTRO:
    if( game.buttonWasPressed || !player.isPlaying() ){
      game.nextState = SELECT_TRACK;
    }
    break;
  case SELECT_TRACK:
    if( iterationDelta < SELECT_REMINDER_DELTA * 6 ){
      if( iterationDelta > game.stateVar1 && game.stateVar2 < SELECT_REMINDER_MAX_COUNT ){
        game.stateVar1 += SELECT_REMINDER_DELTA;
        game.stateVar2++;
        if( player.isPlaying() ){
          player.stopTrack();
        }
        player.playMP3("seltrack.mp3");
      }
      if( game.buttonWasPressed ){
        for( int i = 0; i < NUM_TRACKS; ++i ){
          if( game.buttonState[0][i] && !game.trackPlayed[i] ){
            game.trackPlayed[i] = 1;
            game.tracksPlayed++;
            game.selectedTrack = i;
            game.nextState = PLAY_TRACK;
          }
        }
      }
    } 
    else {
      Serial.println("You take too long... Timeout! Go back to ATTRACT!");
      game.nextState = ATTRACT;
    }
    break;
  case PLAY_TRACK:
    if( game.buttonWasPressed ){
      for( int i = 0; i < NUM_TRACKS; ++i ){
        if( game.buttonState[1][i] ){
          game.selectedRegion = i;
          game.nextState = PLAY_REGION;
        }
      }
      if( game.nextState == PLAY_TRACK ){
        game.nextState = SELECT_REGION;
      }
    } 
    else if( !player.isPlaying() ){
      game.nextState = SELECT_REGION;
    }
    break;
  case SELECT_REGION:
    if( iterationDelta < SELECT_REMINDER_DELTA * 6 ){
      if( iterationDelta > game.stateVar1 && game.stateVar2 < SELECT_REMINDER_MAX_COUNT ){
        game.stateVar1 += SELECT_REMINDER_DELTA;
        game.stateVar2++;
        if( player.isPlaying() ){
          player.stopTrack();
        }
        player.playMP3("selreg.mp3");
      }
      if( game.buttonWasPressed ){
        for( int i = 0; i < NUM_TRACKS; ++i ){
          if( game.buttonState[1][i] ){
            game.selectedRegion = i;
            game.nextState = PLAY_REGION;
          }
        }
      }
    } 
    else {
      game.nextState = ATTRACT;
    }
    break;
  case PLAY_REGION:
    if( game.buttonWasPressed || !player.isPlaying() ){
      if( ANSWER_MAP[game.selectedTrack] == game.selectedRegion ){
        game.nextState = CORRECT_ANSWER;
      } 
      else {
        game.nextState = INCORRECT_ANSWER;
      }
    }
    break;
  case CORRECT_ANSWER:
    if( game.buttonWasPressed || !player.isPlaying() ){
      game.nextState = game.tracksPlayed == NUM_TRACKS ? GAME_OVER : SELECT_TRACK;
    }
    break;
  case INCORRECT_ANSWER:
    if( game.buttonWasPressed || !player.isPlaying() ){
      game.nextState = game.tracksPlayed == NUM_TRACKS ? GAME_OVER : SELECT_TRACK;
    }
    break;
  case GAME_OVER:
    if( game.buttonWasPressed || !player.isPlaying() ){
      game.nextState = ATTRACT;
    }
    break;
  }
}

