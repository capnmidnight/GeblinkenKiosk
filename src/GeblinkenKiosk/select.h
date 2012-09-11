#ifndef _SELECT_TRACK_H
#define _SELECT_TRACK_H

#include <arduino.h>
#include "kiosk.h"

const int SELECT_REMINDER_DELTA = 10000;
const int SELECT_REMINDER_MAX_COUNT = 3;
void enter_SELECT_TRACK(GameState&);
States body_SELECT_TRACK(int, GameState&);

void enter_SELECT_REGION(GameState&);
States body_SELECT_REGION(int, GameState&);

#endif
