#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>
#include "commandeReset.h"
#include "communicationSerie.h"
#include "commandeMove.h"
#include "homingMoteur.h"
#include "gestionLedRGBAdressable.h"
#include "commandeFAN.h"

long lastDelayX = 0;
long lastDelayY = 0;

//...........................................................................
void handleMoveCommand(String line) {

  String mode;
  float valueX = 0;
  float valueY = 0;
  int valueVM = 400;   // vitesse max (delayMin) - valeur par défaut
  int valueVI = 2000;  // vitesse initiale (delayMax) - valeur par défaut

  // Vérifie le type
  if (line.startsWith("move auto")) {
    mode = "auto";
  } else if (line.startsWith("move manu")) {
    mode = "manu";
  } else {
    messageln("Commande inconnue");
    return;
  }

  // Cherche X et Y
  int indexX = line.indexOf('X');
  int indexY = line.indexOf('Y');
  int indexVM = line.indexOf("VM");
  int indexVI = line.indexOf("VI");

  if (indexX == -1 || indexY == -1) {
    messageln("Erreur format: X ou Y manquant");
    return;
  }

  // Extraction X → s'arrête avant Y
  valueX = line.substring(indexX + 1, indexY).toFloat();

  // Extraction Y → s'arrête avant VM si présent, sinon avant VI, sinon fin
  int endY = line.length();
  if (indexVM != -1) endY = min(endY, indexVM);
  if (indexVI != -1) endY = min(endY, indexVI);
  valueY = line.substring(indexY + 1, endY).toFloat();

  // Extraction VM et VI si présents
  if (indexVM != -1) {
    int endVM = line.length();
    if (indexVI != -1 && indexVI > indexVM) endVM = min(endVM, indexVI);
    valueVM = line.substring(indexVM + 2, endVM).toInt();
  }

  if (indexVI != -1) {
    int endVI = line.length();
    if (indexVM != -1 && indexVM > indexVI) endVI = min(endVI, indexVM);
    valueVI = line.substring(indexVI + 2, endVI).toInt();
  }

  Serial.print("Mode: ");
  Serial.println(mode);
  Serial.print("X: ");
  Serial.println(valueX);
  Serial.print("Y: ");
  Serial.println(valueY);
  Serial.print("VM (delayMin): ");
  Serial.println(valueVM);
  Serial.print("VI (delayMax): ");
  Serial.println(valueVI);

  if (mode == "auto") {
    moveAuto((long)valueX, (long)valueY, valueVM, valueVI);
  }

  if (mode == "manu") {
    if (valueX < -1.0 || valueX > 1.0 || valueY < -1.0 || valueY > 1.0) {
      messageln("Erreur: valeur manu doit etre entre -1 et 1");
      return;
    }
    moveManu(valueX, valueY);
  }
}

//--------------------------------------------------------------------

void moveAuto(long targetX, long targetY, int delayMin, int delayMax) {

  if (targetX > POSMAX_X || targetX < 0) {
    messageln("ERR 1");
    messageln("erreur 1 axe X depace pose max ou min");
    return;
  }
  if (targetX > POSMAX_X || targetX < 0) {
    messageln("ERR 2");
    messageln("erreur 2 axe Y depace pose max ou min");
    return;
  }

  messageln("=== MOVE AUTO ===");

  long dx = abs(targetX - posX);
  long dy = abs(targetY - posY);

  int sx = (posX < targetX) ? 1 : -1;
  int sy = (posY < targetY) ? 1 : -1;

  digitalWrite(DIR_X, sx > 0 ? HIGH : LOW);
  digitalWrite(DIR_Y, sy > 0 ? HIGH : LOW);

  long err = dx - dy;
  long steps = max(dx, dy);

  // Rampe adaptative : si steps < 2*ramp, on divise par 2
  // => accél sur 50% du trajet, décél sur les 50% restants
  // => on n'atteint jamais delayMin dans ce cas
  long ramp = 1000;
  if (steps < 2 * ramp) ramp = steps / 2;  // ← ici le fix

  long i = 0;

  while (posX != targetX || posY != targetY) {

    if (comstop()) return;  // commande d'arret "d'urgence"
    // RAMPE
    int stepDelay;

    if (i < ramp)
      stepDelay = map(i, 0, ramp, delayMax, delayMin);  // accélération
    else if (i > steps - ramp)
      stepDelay = map(i, steps - ramp, steps, delayMin, delayMax);  // décélération
    else
      stepDelay = delayMin;  // vitesse max

    // BRESENHAM
    long e2 = 2 * err;

    bool doStepX = false;
    bool doStepY = false;

    if (e2 > -dy) {
      err -= dy;
      posX += sx;
      doStepX = true;
    }
    if (e2 < dx) {
      err += dx;
      posY += sy;
      doStepY = true;
    }

    // STEP
    if (doStepX) digitalWrite(STEP_X, HIGH);
    if (doStepY) digitalWrite(STEP_Y, HIGH);

    delayMicroseconds(5);

    digitalWrite(STEP_X, LOW);
    digitalWrite(STEP_Y, LOW);

    delayMicroseconds(stepDelay);

    i++;
  }
  posX = targetX;
  posY = targetY;

  messageln("POSITION_X " + String(posX));
  messageln("POSITION_Y " + String(posY));

  messageln("Move Auto terminé");
}
//......................................................................
void moveManu(float speedX, float speedY) {


  // Direction
  digitalWrite(DIR_X, speedX >= 0 ? HIGH : LOW);
  digitalWrite(DIR_Y, speedY >= 0 ? HIGH : LOW);

  // Valeur absolue 0.0 à 1.0
  float absX = speedX < 0 ? -speedX : speedX;
  float absY = speedY < 0 ? -speedY : speedY;

  // Calcul du delay (vitesse cible)
  long delayX = (long)map(absX * 2000, 0, 2000, vMax, vMin);
  long delayY = (long)map(absY * 1000, 0, 1000, vMax, vMin);

  // Sauvegarde pour le prochain appel
  lastDelayX = delayX;
  lastDelayY = delayY;

  bool infocom = false;

  while (infocom == false) {

    if (absX > 0.01) {
      bool dirX = speedX >= 0;
      if (dirX && posX < POSMAX_X) {
        digitalWrite(STEP_X, HIGH);
        delayMicroseconds(5);
        digitalWrite(STEP_X, LOW);
        delayMicroseconds(delayX);
        posX++;
      } else if (!dirX && posX > 0) {
        digitalWrite(STEP_X, HIGH);
        delayMicroseconds(5);
        digitalWrite(STEP_X, LOW);
        delayMicroseconds(delayX);
        posX--;
      }
    }

    if (absY > 0.01) {
      bool dirY = speedY >= 0;
      if (dirY && posY < POSMAX_Y) {
        digitalWrite(STEP_Y, HIGH);
        delayMicroseconds(5);
        digitalWrite(STEP_Y, LOW);
        delayMicroseconds(delayY);
        posY++;
      } else if (!dirY && posY > 0) {
        digitalWrite(STEP_Y, HIGH);
        delayMicroseconds(5);
        digitalWrite(STEP_Y, LOW);
        delayMicroseconds(delayY);
        posY--;
      }
    }

    infocom = communication();
  }

  messageln("POSITION_X " + String(posX));
  messageln("POSITION_Y " + String(posY));
  messageln("Move Manu terminé");
}