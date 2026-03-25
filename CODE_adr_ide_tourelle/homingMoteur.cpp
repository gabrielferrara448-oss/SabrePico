
#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>
#include "commandeReset.h"
#include "communicationSerie.h"
#include "commandeMove.h"
#include "homingMoteur.h"
#include "gestionLedRGBAdressable.h"
#include "commandeFAN.h"

void homingMoteur(int vitesseHoming_X, int vitesseHoming_Y) {

    messageln("=== HOMING START ===");

    noInterrupts();
    stalled_X = false;
    stalled_Y = false;
    interrupts();

    delay(50);

    digitalWrite(DIR_X, LOW);
    digitalWrite(DIR_Y, LOW);

    int index_led = 0;
    int offset = 80;

    unsigned long dernierStepX = 0;
    unsigned long dernierStepY = 0;
    unsigned long derniereLed  = 0;
    int intervalLed = 20000;

    // nombre de steps à ignorer au démarrage
    int ignoreStepsX = 200;
    int ignoreStepsY = 200;

    while (!stalled_X || !stalled_Y) {

        unsigned long maintenant = micros();

        // --- Step X ---
        if (!stalled_X && (maintenant - dernierStepX >= vitesseHoming_X)) {
            digitalWrite(STEP_X, HIGH);
            delayMicroseconds(5);
            digitalWrite(STEP_X, LOW);
            dernierStepX = maintenant;

            // on décrémente jusqu'à 0, ensuite le stall est autorisé
            if (ignoreStepsX > 0) {
                ignoreStepsX--;
                noInterrupts();
                stalled_X = false;  // force reset pendant la phase d'ignorance
                interrupts();
            }
        }

        // --- Step Y ---
        if (!stalled_Y && (maintenant - dernierStepY >= vitesseHoming_Y)) {
            digitalWrite(STEP_Y, HIGH);
            delayMicroseconds(5);
            digitalWrite(STEP_Y, LOW);
            dernierStepY = maintenant;

            if (ignoreStepsY > 0) {
                ignoreStepsY--;
                noInterrupts();
                stalled_Y = false;  // force reset pendant la phase d'ignorance
                interrupts();
            }
        }

        // --- Animation LED ---
        if (maintenant - derniereLed >= intervalLed) {
            strip.clear();
            strip.setPixelColor(index_led, strip.Color(255, 120, 0));
            strip.setPixelColor((index_led + offset) % 160, strip.Color(255, 120, 0));
            strip.show();
            index_led++;
            if (index_led >= 160)
                index_led = 0;
            derniereLed = maintenant;
        }
    }

    digitalWrite(STEP_X, LOW);
    digitalWrite(STEP_Y, LOW);

    posX = 0;
    posY = 0;

    messageln("=== HOMING FINI ===");

    for (int i = 0; i < 3; i++) {
        strip.fill(strip.Color(255, 255, 0));
        strip.show();
        delay(300);
        strip.clear();
        strip.show();
        delay(300);
    }

    strip.fill(strip.Color(0, 255, 0));
    strip.show();

    stalled_X = false;
    stalled_Y = false;
}
//.......................................................................
void handleHomingCommand(String line) {

  int vitesseX = 1000;  // valeur par défaut
  int vitesseY = 1000;

  int indexX = line.indexOf('X');
  int indexY = line.indexOf('Y');

  // Si X est présent
  if (indexX != -1) {
    if (indexY != -1 && indexY > indexX) {
      vitesseX = line.substring(indexX + 1, indexY).toInt();
    } else {
      vitesseX = line.substring(indexX + 1).toInt();
    }
  }

  // Si Y est présent
  if (indexY != -1) {
    vitesseY = line.substring(indexY + 1).toInt();
  }

  if (vitesseX <= 0 || vitesseY <= 0) {
    messageln("Erreur: vitesse doit etre > 0");
    return;
  }

  Serial.print("Homing X = ");
  Serial.println(vitesseX);
  Serial.print("Homing Y = ");
  Serial.println(vitesseY);

  homingMoteur(vitesseX, vitesseY);
}