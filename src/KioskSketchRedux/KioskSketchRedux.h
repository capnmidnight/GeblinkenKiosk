#define NUM_TRACKS 12
#define ATTRACT_MILLIS_PER_FRAME 100
#define ATTRACT_MUSIC_FREQUENCY 30000UL
#define SELECT_REMINDER_DELTA 10000UL
#define SELECT_REMINDER_MAX_COUNT 3
const int ANSWER_MAP[NUM_TRACKS] = {4, 11, 3, 10, 9, 6, 2, 5, 0, 1, 8, 7};

enum States {
	NONE,			// initial value
	ATTRACT,		// a mode where the game board animates and tries to draw a player in
	INTRO,			// plays a track that explains what the kiosk is about
	SELECT_TRACK,		// tells the user to select a track to play, and waits for them to hit a button
	PLAY_TRACK,		// plays the track selection the user made
	SELECT_REGION,		// tells the user to select a region to match to the track
	PLAY_REGION,		// plays the name of the city that the user selected
	CORRECT_ANSWER,		// rings the bell and blinks the green light if the user made the right choice
	INCORRECT_ANSWER,	// plays a buzzer sound and blinks the red light if the user made the wrong choice
	GAME_OVER		// plays a message relevant to the users total score, then kicks out to the ATTRACT mode
};


const int NUM_LED_OUTPUTS = 36;
const int LED_OUTPUT_MAP[NUM_LED_OUTPUTS + 2] = {
  14, 15, 16, A0, 
  17, 18, 19, A1,
  20, 21, 22, A2,
  
  23, 24, 25, A3,
  26, 27, 28, A4,
  29, 30, 31, A5,
  
  32, 33, 34, 35, 36, 37,
  38, 39, 40, 41, 42, 43,
  
  44, 45
};

const int NUM_INPUTS = 8;
enum InputPins
{
  BANK1_COL1 = 8,
  BANK1_COL2,
  BANK1_COL3, 
  
  BANK2_COL1,
  BANK2_COL2,
  BANK2_COL3,
  
  BANK2_ROW4,
  BANK1_ROW4
};


const int MIN_INPUT_PIN = BANK1_COL1;
const int MAX_INPUT_PIN = BANK1_ROW4;
const int MIN_OUTPUT_PIN = 0;
const int MAX_OUTPUT_PIN = 35;
const int MIN_TRACK_PIN = 0;
const int MAX_TRACK_PIN = 11;
const int MIN_REGION_PIN = 12;
const int MAX_REGION_PIN = 23;
const int MIN_SCORE_PIN = 24;
const int MAX_SCORE_PIN = 35;
const int GREEN_STROBE_PIN = 36;
const int RED_STROBE_PIN = 37;

struct Game {
	enum States currentState;
	enum States nextState;
	unsigned long stateStartTime;
	int score;
	boolean trackPlayed[NUM_TRACKS];
	int tracksPlayed;
	int selectedTrack;
	int selectedRegion;
	unsigned long stateVar1;
	unsigned long stateVar2;
	unsigned long stateVar3;
	boolean buttonWasPressed;
	boolean buttonState[2][NUM_TRACKS];
};
