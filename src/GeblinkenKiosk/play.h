#ifndef _PLAY_H
#define _PLAY_H

#include <arduino.h>
#include "kiosk.h"

const int TRACK_MAP[NUM_TRACKS] = {2, 7, 4, 6, 8, 0, 1, 5, 3};
void enter_PLAY_TRACK(GameState&);
States body_PLAY_TRACK(int, GameState&);

const int ANSWER_MAP[NUM_TRACKS] = {5, 4, 6, 7, 2, 3, 8, 1, 0};
const int REGION_MAP[NUM_TRACKS] = {14, 9, 15, 13, 17, 16, 12, 11, 10};
void enter_PLAY_REGION(GameState&);
States body_PLAY_REGION(int, GameState&);

#endif
