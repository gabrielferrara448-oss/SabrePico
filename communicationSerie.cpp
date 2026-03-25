
#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>
#include "commandeReset.h"
#include "communicationSerie.h"
#include "commandeMove.h"
#include "homingMoteur.h"
#include "gestionLedRGBAdressable.h"
#include "commandeFAN.h"
#include "enabelMoteur.h"

bool communication() {

  //gere la com recu par serial donc l'usb
  if (Serial.available()) {

    String line = Serial.readStringUntil('\n');
    treteMessageRecu(line);
    return true;
  } else if (Serial1.available()) {  //gere la com recu par serilal1 danc l'uart raspberry

    String line = Serial1.readStringUntil('\n');
    treteMessageRecu(line);
    return true;
  } else {
    return false;
  }
}
//.........................................................................
String reponsecom() {

  //gere la com recu par serial donc l'usb
  if (Serial.available()) {

    String line = Serial.readStringUntil('\n');
    return line;
  }

  //gere la com recu par serilal1 danc l'uart raspberry
  if (Serial1.available()) {

    String line = Serial1.readStringUntil('\n');
    return line;
  }
  return "ras";
}
void ping() {
  messageln("pong");
  conteur_de_ping++;
  if (conteur_de_ping == 5) {
    messageln("BON !!! maintenant tu as compris je repones donc c'est juste toi qui en nul donc appelle gab ou max sinon je vais divser par 0 et ca vas te faire tout drole");
  }
  if (conteur_de_ping == 6) {
    messageln("ok");
    delay(500);
    messageln("OK !!!");
    delay(500);
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
  }
}
//.........................................................................
void treteMessageRecu(String line) {

  line.trim();
  if (line.startsWith("ping")) {
    ping();
  } else if (line.startsWith("homing")) {
    handleHomingCommand(line);
  } else if (line.startsWith("move")) {
    handleMoveCommand(line);
  } else if (line.startsWith("reset")) {
    handleReset(line);
  } else if (line.startsWith("enabel moteur")) {
    enabelMoteur(line);
  } else if (line.startsWith("FAN")) {
    creationCommandeFan(line);
  }
}
//..............................................................................
void message(String message) {
  Serial.print(message);
  Serial1.print(message);
}
//..............................................................................
void messageln(String message) {
  Serial.println(message);
  Serial1.println(message);
}