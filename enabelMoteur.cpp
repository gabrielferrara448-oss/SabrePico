#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>
#include "commandeReset.h"
#include "communicationSerie.h"
#include "commandeMove.h"
#include "homingMoteur.h"
#include "gestionLedRGBAdressable.h"
#include "commandeFAN.h"
#include "enabelMoteur.h"

void enabelMoteur(String line, int i, bool j) {

  if (line.startsWith("enabel moteur")) {
    // Desactiver les drivers
    digitalWrite(EN_X, HIGH);
    digitalWrite(EN_Y, HIGH);
    digitalWrite(EN_Z, HIGH);
    digitalWrite(EN_E, HIGH);
    messageln("puissance moteur couper");
    messageln("pour reactiver la puissance moteur --> stop");

    while (true) {

      String reponse = reponsecom();

      if (reponse == "s" || reponse == "S" || reponse == "stop" || reponse == "STOP") {
        // Activer les drivers
        digitalWrite(EN_X, LOW);
        digitalWrite(EN_Y, LOW);
        digitalWrite(EN_Z, LOW);
        digitalWrite(EN_E, LOW);
        strip.fill(strip.Color(0, 255, 0));  // toutes les LEDs en bleu
        strip.show();
        break;
      }
      strip.fill(strip.Color(0, 0, i));  // toutes les LEDs en bleu
      strip.show();

      if (i == 255) j = false;
      if (i == 0) j = true;

      if (j == true) i++;
      if (j == false) i--;
    }
  } else {
    messageln("commande inconue");
  }
}
