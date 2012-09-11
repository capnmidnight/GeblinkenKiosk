#ifndef _CORRECT_ANSWER_H
#define _CORRECT_ANSWER_H

#include <arduino.h>
#include "kiosk.h"

char* enter_CORRECT_ANSWER(GameState&);
States body_CORRECT_ANSWER(int, GameState&);

char* enter_INCORRECT_ANSWER(GameState&);
States body_INCORRECT_ANSWER(int, GameState&);

#endif
