
#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>
#include "commandeReset.h"
#include "communicationSerie.h"
#include "commandeMove.h"
#include "homingMoteur.h"
#include "gestionLedRGBAdressable.h"
#include "commandeFAN.h"
//...........................................................................
void commandeFan(int fan, int puissance) {
  if (fan <= 3 && puissance >= 0 && puissance <= 1024) {
    if (fan == 1) {
      analogWriteFreq(1000);   // fréquence PWM = 1 kHz
      analogWriteRange(1024);  // résolution 10 bits → 0 à 1023

      analogWrite(FAN1, puissance);  // 50% duty cycle
      message("vantilateur 1, puissance a:");
      messageln(String(puissance));

    } else if (fan == 2) {
      analogWriteFreq(1000);   // fréquence PWM = 1 kHz
      analogWriteRange(1024);  // résolution 10 bits → 0 à 1023

      analogWrite(FAN2, puissance);  // 50% duty cycle
      message("vantilateur 2, puissance a:");
      messageln(String(puissance));

    } else if (fan == 3) {
      analogWriteFreq(1000);   // fréquence PWM = 1 kHz
      analogWriteRange(1024);  // résolution 10 bits → 0 à 1023

      analogWrite(FAN3, puissance);  // 50% duty cycle
      message("vantilateur 3, puissance a:");
      messageln(String(puissance));

    }
  }
  else{
    messageln("commande FAN inconue");
  }
}
void creationCommandeFan(String line){
  
  int idFan = 1;
  int puissance = 0; 

  int indexFan = line.indexOf("FAN");
  int indexPui = line.indexOf("Puissance");

  idFan = line.substring(indexFan + 3, indexPui).toInt();
  puissance = line.substring(indexPui + 9).toInt();

  commandeFan(idFan, puissance);
}