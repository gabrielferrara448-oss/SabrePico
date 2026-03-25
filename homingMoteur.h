#ifndef HOMING_MOTEUR_H
#define HOMING_MOTEUR_H

#include "globals.h"

void homingMoteur(int vitesseHoming_X, int vitesseHoming_Y);
//.......................................................................
void handleHomingCommand(String line);
#endif