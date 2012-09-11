#ifndef _PLAY_H
#define _PLAY_H

#include <arduino.h>
#include "kiosk.h"

char* enter_PLAY_TRACK(GameState&);
States body_PLAY_TRACK(int, GameState&);

const int ANSWER_MAP[NUM_TRACKS] = {5, 4, 6, 7, 2, 3, 8, 1, 0};
char* enter_PLAY_REGION(GameState&);
States body_PLAY_REGION(int, GameState&);

#endif
