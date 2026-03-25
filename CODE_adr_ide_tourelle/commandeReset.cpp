#include "commandeReset.h"
#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>
#include "commandeReset.h"
#include "communicationSerie.h"
#include "commandeMove.h"
#include "homingMoteur.h"
#include "gestionLedRGBAdressable.h"
#include "commandeFAN.h"
//.............................................................................
void handleReset(String line) {

  //recet tip: reset tmc stall_valueX 20 courrantX 1000 stall_valueY 20 courantY 1000
  // Vérifie le type
  if (line.startsWith("reset tmc")) {
    messageln("orde de reste tmc recu");
    creationDordreResetTMC(line);
  } else if (line.startsWith("reset rp2040")) {
    ordreResetRP2040();
  } else {
    messageln("ordre de reset inconnue");
    return;
  }
}
//.................................................................................
void ordreResetRP2040() {
  messageln("voulez vous reset le rp 2040 et donc tout le skr pico ?");
  messageln("yes / no");
  int i = 0;  //index led bleu
  bool j = false;
  while (true) {

    String reponse = reponsecom();

    if (reponse == "y" || reponse == "Y" || reponse == "yes" || reponse == "oui") {

      strip.fill(strip.Color(255, 0, 0));  // toutes les LEDs en bleu
      strip.show();
      messageln("3");
      delay(1000);
      strip.fill(strip.Color(0, 0, 0));  // toutes les LEDs en bleu
      strip.show();
      messageln("2");
      delay(1000);
      strip.fill(strip.Color(255, 0, 0));  // toutes les LEDs en bleu
      strip.show();
      messageln("1");
      delay(1000);
      strip.fill(strip.Color(0, 0, 0));  // toutes les LEDs en bleu
      strip.show();
      messageln("0");
      delay(1000);
      strip.fill(strip.Color(255, 0, 0));  // toutes les LEDs en bleu
      strip.show();
      rp2040.restart();
      break;

    } else if (reponse == "n" || reponse == "N" || reponse == "no" || reponse == "non") {

      messageln("annulation de la commande reset RP2040");
      strip.fill(strip.Color(255, 0, 0));  // toutes les LEDs en bleu
      strip.show();
      delay(1000);
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
}
//...........................................................................
void resetTMC(int stall_valueX, int courantX, int stall_valueY, int courantY) {

  messageln("=== Initialisation drivers ===");

  // Initialiser chaque TMC
  initDriver(driverX, "Driver X", stall_valueX, courantX);
  initDriver(driverY, "Driver Y", stall_valueY, courantY);
  initDriver(driverZ, "Driver Z",0,0);
  initDriver(driverE, "Driver E",0,0);

  messageln("=== Initialisation terminée ===");

  strip.clear();
  strip.fill(strip.Color(0, 255, 0));  // toutes les LEDs en vert
  strip.show();                        // applique à toutes les LEDS
}

//...............................................................................................
void creationDordreResetTMC(String line) {


  int SVX = 37;
  int SVY = 30;

  int CX = 1000;
  int CY = 1000;

  int indexSX = line.indexOf("stall_valueX");
  int indexCX = line.indexOf("courantX");
  int indexSY = line.indexOf("stall_valueY");
  int indexCY = line.indexOf("courantY");

  if (indexSX < indexCX && indexCX < indexSY && indexSY < indexCY && indexCY > indexSY) {

    messageln("parametrage tmc en cours");

    // Si index est présent
    if (indexSX != -1) {
      SVX = line.substring(indexSX + 12, indexCX).toInt();
    }

    if (indexSY != -1) {
      CX = line.substring(indexCX + 8, indexSY).toInt();
    }

    if (indexCX != -1) {
      SVY = line.substring(indexSY + 12, indexCY).toInt();
    }

    if (indexCY != -1) {
      CY = line.substring(indexCY + 8).toInt();
    }
    messageln("voulez vous reset les tmc avec les parametre suivant ?");
    message("stall value X : ");
    messageln(String(SVX));
    message("courant X : ");
    messageln(String(CX));
    message("stall value Y : ");
    messageln(String(SVY));
    message("courant Y : ");
    messageln(String(CY));
    messageln("yes / no");
    int i = 0;  //index led bleu
    bool j = false;
    while (true) {

      String reponse = reponsecom();

      if (reponse == "y" || reponse == "Y" || reponse == "yes" || reponse == "oui") {
        resetTMC(SVX, CX, SVY, CY);
        break;
      } else if (reponse == "n" || reponse == "N" || reponse == "no" || reponse == "non") {

        messageln("annulation de la commande reset tmc");
        strip.fill(strip.Color(255, 0, 0));  // toutes les LEDs en bleu
        strip.show();
        delay(1000);
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

    messageln("mauvaise syntaxe des parametre tmc");
    messageln("exemple de syntaxe correct : reset tmc stall_valueX 20 courantX 1000 stall_valueY 20 courantY 1000");
    messageln("voulez vous reset les tmc avec les parametres par defaut ?");
    messageln("yes / no");
    int i = 0;  //index led bleu
    bool j = false;
    while (true) {

      String reponse = reponsecom();

      if (reponse == "y" || reponse == "Y" || reponse == "yes" || reponse == "oui") {
        resetTMC();
        break;
      } else if (reponse == "n" || reponse == "N" || reponse == "no" || reponse == "non") {

        messageln("annulation de la commande reset tmc");
        strip.fill(strip.Color(255, 0, 0));  // toutes les LEDs en bleu
        strip.show();
        delay(1000);
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
  }
}
//-----------------------------------------------------------------
// Initialisation TMC2209
void initDriver(TMC2209Stepper &drv, const char *name, int stall_value, int courant) {

  strip.clear();

  int index_led = 0;
  int index_led1 = 80;
  int largeur = 7;

  drv.begin();                                                                                                                              // SPI: Init CS pins and possible SW SPI pins
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);

  drv.toff(4);                                                                                                                              // Enables driver in software, changed from 5
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);

  drv.blank_time(24);
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);

  drv.rms_current(courant);                                                                                                                 // Set motor RMS current
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);

  drv.microsteps(16);                                                                                                                       // Set microsteps to 1/16th
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);


  //driver.en_spreadCycle(false); // Toggle spreadCycle
  drv.pwm_autoscale(true);                                                                                                                  // Needed for stealthChop
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);

  drv.semin(5);
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);

  drv.semax(2);
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);

  drv.sedn(0b01);
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);

  drv.shaft(shaftVal);
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);


  // TCOOLTHRS needs to be set for stallguard to work //
  drv.TCOOLTHRS(0xFFFFF);                                                                                                                   // 20bit max
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);

  drv.SGTHRS(stall_value);
  allumerZone(index_led, index_led += largeur, strip.Color(135, 206, 235), index_led1, index_led1 += largeur, strip.Color(135, 206, 235));  //animation led
  delay(100);
  //Serial.println(index_led);


  message(name);
  messageln(" OK");
}