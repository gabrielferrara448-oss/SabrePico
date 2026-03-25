// commandeMove.h
#ifndef COMMANDE_MOVE_H
#define COMMANDE_MOVE_H

#include "globals.h"


void handleMoveCommand(String line);
void moveAuto(long targetX, long targetY, int delayMin, int delayMax);
void moveManu(float speedX, float speedY);

#endif