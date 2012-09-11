#include <arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>
#include "KioskSketchRedux.h"

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

void ledOff(int pin){
  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);
}

void ledOn(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, (pin < S1) ? LOW : HIGH);
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
  rangeLightsOn(S1, S1 + game.score);
}

int readColumn(int column){
  static int readCount[MAX_INPUT_PIN + 1 - MIN_INPUT_PIN] = {0, 0, 0, 0, 0, 0};
  int voltage = analogRead(column + MIN_INPUT_PIN);
  //software debouncing
  if(voltage == 0)
    readCount[column] = 0;
  else
    ++readCount[column];
  if(readCount[column] < 5 || voltage < 203) return 0;
  else if(voltage < 450) return 1;
  else if(voltage < 560) return 2;
  else if(voltage < 649) return 3;
  else if(voltage < 704) return 4;
  else if(voltage < 742) return 5;
  else if(voltage < 769) return 6;
  return 7;
}

void readInput(){
  game.buttonWasPressed = 0;
  for(int column = 0; column <= MAX_INPUT_PIN - MIN_INPUT_PIN; ++column)
  {
    int state = readColumn(column);
    //Serial.print("column: ");
    //Serial.println(column);
    //Serial.print("state: ");
    //Serial.println(state);
    game.buttonWasPressed |= state;
    //Serial.print("button was pressed: ");
    //Serial.println(game.buttonWasPressed);

    int bank = column / 3;
    //Serial.print("bank: ");
    //Serial.println(bank);
    int columnStart = (column % 3) * 3;
    //Serial.print("columnStart: ");
    //Serial.println(columnStart);
    for(int button = 0; button < 3; ++button, state >>= 1)
    {
      game.buttonState[bank][columnStart + button] = state & 1;
    }
  }
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
      rangeLightsOff(RIGHT_ANSWER, WRONG_ANSWER);
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
      player.playTrack(game.selectedTrack);
      break;
    case SELECT_REGION:
      game.stateVar1 = SELECT_REMINDER_DELTA;
      game.stateVar2 = 1;
      allRegionLightsOn();
      player.playMP3("selreg.mp3");
      break;
    case PLAY_REGION:
      ledOn(MIN_REGION_PIN + game.selectedRegion);
      //player.playTrack(game.selectedRegion + NUM_TRACKS);
      break;
    case CORRECT_ANSWER:
      game.score++;
      setScoreLights();
      ledOn(RIGHT_ANSWER);
      player.playMP3("right.mp3");
      break;
    case INCORRECT_ANSWER:
      ledOn(WRONG_ANSWER);
      player.playMP3("wrong.mp3");
      break;
    case GAME_OVER:
      if( game.score == NUM_TRACKS )
      {
        ledOn(RIGHT_ANSWER);
        player.playMP3("win.mp3");
      } 
      else if(game.score > NUM_TRACKS - 4)
      {
        ledOn(RIGHT_ANSWER);
        player.playMP3("win.mp3");
      }
      else 
      {
        ledOn(WRONG_ANSWER);
        player.playMP3("lose.mp3");
      }
      break;
    }
  }
  readInput();
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

