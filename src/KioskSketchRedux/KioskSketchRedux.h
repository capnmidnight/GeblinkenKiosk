#define NUM_TRACKS 9
#define ATTRACT_MILLIS_PER_FRAME 100
#define ATTRACT_MUSIC_FREQUENCY 30000UL
#define SELECT_REMINDER_DELTA 10000UL
#define SELECT_REMINDER_MAX_COUNT 3
const int ANSWER_MAP[NUM_TRACKS] = {5, 4, 6, 7, 2, 3, 8, 1, 0};

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

enum Pins {
	BANK1_COL1 = 8,
	BANK1_COL2,
	BANK1_COL3,
	BANK2_COL1,
	BANK2_COL2,
	BANK2_COL3,
	
	T1, T2, T3,
	T4, T5, T6,
	T7, T8, T9,
	
	R1, R2, R3,
	R4, R5, R6,
	R7, R8, R9,
	
	S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12,
	
	RIGHT_ANSWER, WRONG_ANSWER
};
#define MIN_INPUT_PIN BANK1_COL1
#define MAX_INPUT_PIN BANK2_COL3
#define MIN_OUTPUT_PIN T1
#define MAX_OUTPUT_PIN WRONG_ANSWER
#define MIN_TRACK_PIN T1
#define MAX_TRACK_PIN T9
#define MIN_REGION_PIN R1
#define MAX_REGION_PIN R9
#define MIN_SCORE_PIN S1
#define MAX_SCORE_PIN S12

struct Game {
	enum States currentState;
	enum States nextState;
	unsigned long stateStartTime;
	int score;
	char trackPlayed[NUM_TRACKS];
	int tracksPlayed;
	int selectedTrack;
	int selectedRegion;
	unsigned long stateVar1;
	unsigned long stateVar2;
	unsigned long stateVar3;
	int buttonWasPressed;
	int buttonState[2][NUM_TRACKS];
};
