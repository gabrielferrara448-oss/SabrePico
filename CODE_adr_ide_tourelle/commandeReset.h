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
void resetTMC(int stall_valueX = 37, int courantX = 1000, int stall_valueY = 35, int courantY = 1000);
//-----------------------------------------------------------------
// Initialisation TMC2209
void initDriver(TMC2209Stepper &drv, const char *name, int stall_value, int courant);

#endif