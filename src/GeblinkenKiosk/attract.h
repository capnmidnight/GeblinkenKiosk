#ifndef _ATTRACT_H
#define _ATTRACT_H

#include <arduino.h>
#include <SFEMP3Shield.h>
#include "kiosk.h"

const int ATTRACT_MILLIS_PER_FRAME = 100;
char* enter_ATTRACT(GameState&); 
States body_ATTRACT(int, GameState&); 

#endif

