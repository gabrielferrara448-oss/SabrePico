#ifndef COMMANDE_RESET_H
#define COMMANDE_RESET_H

#include "globals.h"

//.............................................................................
void handleReset(String line);
//.................................................................................
void ordreResetRP2040();

//...............................................................................................
void creationDordreResetTMC(String line);
//-----------------------------------------------------------------
// Initialisation TMC2209
void initDriver(TMC2209Stepper &drv, const char *name, int stall_value = 20, int courant = 1000);

#endif