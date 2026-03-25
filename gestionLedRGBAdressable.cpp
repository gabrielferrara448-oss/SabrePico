
#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>
#include "commandeReset.h"
#include "communicationSerie.h"
#include "commandeMove.h"
#include "homingMoteur.h"
#include "gestionLedRGBAdressable.h"
#include "commandeFAN.h"
//............................................................................
void allumerZone(int debut, int fin, uint32_t couleur, int debut1, int fin1, uint32_t couleur1) {

  for (int i = debut; i <= fin; i++) {
    strip.setPixelColor(i, couleur);
  }

  for (int i = debut1; i <= fin1; i++) {
    strip.setPixelColor(i, couleur1);
  }
  strip.show();
}
void gestionAllumerZone(String line){
  
}