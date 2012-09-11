#ifndef _KIOSK_H
#define _KIOSK_H

#include <arduino.h>
#include <SFEMP3Shield.h>

const unsigned int NUM_TRACKS = 9;

/*
  These are all of the states that the game goes through, to make
  a state machine.
  */
enum States
{
  NONE = -1,          // initial value
  ATTRACT,            // a mode where the game board animates and tries to draw a player in
  INTRO,              // plays a track that explains what the kiosk is about
  SELECT_TRACK,       // tells the user to select a track to play, and waits for them to hit a button
  PLAY_TRACK,         // plays the track selection the user made
  SELECT_REGION,      // tells the user to select a region to match to the track
  PLAY_REGION,        // plays the name of the city that the user selected
  CORRECT_ANSWER,     // rings the bell and blinks the green light if the user made the right choice
  INCORRECT_ANSWER,   // plays a buzzer sound and blinks the red light if the user made the wrong choice
  GAME_OVER           // plays a message relevant to the users total score, then kicks out to the ATTRACT mode
};
const int NUM_STATES = GAME_OVER+1;

struct GameState
{
  int score;
  boolean trackPlayed[9];
  int tracksPlayed;
  int selectedTrack;
  int selectedRegion;
  SFEMP3Shield mp3;
};

// function pointer type definitions for the FSM
typedef char*(*enter_function)(GameState&);
typedef States(*body_function)(int, GameState&);

// Digital I/O pins for each of the status lights
enum OutputPins
{
  T1 = 14,  T2,  T3,
       T4,  T5,  T6,
       T7,  T8,  T9,

       R1,  R2,  R3,
       R4,  R5,  R6,
       R7,  R8,  R9,

       S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12,

       RIGHT_ANSWER, WRONG_ANSWER
};
const int MIN_OUTPUT_PIN = T1;
const int MAX_OUTPUT_PIN = WRONG_ANSWER;

// Analog input pins for each of the analog button encoders
enum InputPins
{
  BANK1_COL1 = 8,
  BANK1_COL2,
  BANK1_COL3,
  BANK2_COL1,
  BANK2_COL2,
  BANK2_COL3
};
const int MIN_INPUT_COL = BANK1_COL1;
const int MAX_INPUT_COL = BANK2_COL3;

// turn lights on or off
void ledOn(int);
void ledOff(int);
void lightButton(int, int);
void dimButton(int, int);
void allLightsOff(void);

void setScoreLights(int);

// read the analog button encoder pins and do something about them
int readBank(int);
void readInput();
boolean getButtonPressed(int, int);
boolean wasAnyButtonPressed();

#endif

