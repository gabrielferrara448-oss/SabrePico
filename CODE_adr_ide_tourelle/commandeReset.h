#ifndef COMMANDE_RESET_H
#define COMMANDE_RESET_H

#include "globals.h"

//.............................................................................
void handleReset(String line);
//.................................................................................
void ordreResetRP2040();
//...............................................................................................
void creationDordreResetTMC(String line);
//............................................................................................
void resetTMC(int stall_valueX = STALLX, int courantX = COURANTX, int stall_valueY = STALLY, int courantY = COURANTY);
//-----------------------------------------------------------------
// Initialisation TMC2209
void initDriver(TMC2209Stepper &drv, const char *name, int stall_value, int courant);

#endif