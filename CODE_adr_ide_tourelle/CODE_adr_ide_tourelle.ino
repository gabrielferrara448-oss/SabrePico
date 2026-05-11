#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>
#include "commandeReset.h"
#include "communicationSerie.h"
#include "commandeMove.h"
#include "homingMoteur.h"
#include "gestionLedRGBAdressable.h"
#include "commandeFAN.h"
#include "enabelMoteur.h"

//--------------------------------------------------------------
// SET LED RGB
const int ledCount = 160;  //nombre de leds
int ledBrightness = 255;   // default full brightness
Adafruit_NeoPixel strip(ledCount, LED_RGB, NEO_GRB + NEO_KHZ800);

//positon moteur
long posX = 0;
long posY = 0;

int vMax = 3000;
int vMin = 100;


bool stalled_X = false;
bool stalled_Y = false;
bool shaftVal = false;

int conteur_de_ping = 0;
//-------------------------------------------------------nnnn-------
// Drivers TMC

// UART addresses (déjà câblé sur SKR Pico)
TMC2209Stepper driverX(&Serial2, R_SENSE, 0b00);
TMC2209Stepper driverY(&Serial2, R_SENSE, 0b10);
TMC2209Stepper driverZ(&Serial2, R_SENSE, 0b01);
TMC2209Stepper driverE(&Serial2, R_SENSE, 0b11);

//-------------------------------------------------------------------

void setup() {


  // UART0 pour X, Y et com->pc
  Serial1.setTX(UART2_TX);
  Serial1.setRX(UART2_RX);
  Serial1.begin(9600);  // com entre pc et pico skr

  Serial2.setTX(UART_TX);
  Serial2.setRX(UART_RX);

  Serial2.begin(9600);  // com entre TMC2209 et controleur RP2040 (donc en intrerne du pico)
  delay(1000);

  //------------------------------------------------------------------
  // Pin LED et animation

  pinMode(LED_RGB, OUTPUT);

  strip.begin();                       // initialise la bande
  strip.setBrightness(ledBrightness);  // règle la luminosité
  strip.show();                        // applique (éteint tout au départ)

  messageln("=== Initialisation SKR Pico + TMC2209 ===");
  delay(1000);

  for (int i = 0; i < 255; i++) {
    // allumer les leds en red
    strip.fill(strip.Color(i, 0, 0));  // toutes les LEDs en rouge
    strip.show();                      // applique à toutes les LEDs
    delay(5);
  }
  //---------------------------------------------------------------------
  //gestion des pin
  messageln("=== uart1 ===");
  messageln("=== uart1 démarré ===");
  delay(1000);

  // STEP/DIR/EN en sortie
  pinMode(EN_X, OUTPUT);
  pinMode(EN_Y, OUTPUT);
  pinMode(EN_Z, OUTPUT);
  pinMode(EN_E, OUTPUT);

  pinMode(STEP_X, OUTPUT);
  pinMode(STEP_Y, OUTPUT);
  pinMode(STEP_Z, OUTPUT);
  pinMode(STEP_E, OUTPUT);

  pinMode(DIR_X, OUTPUT);
  pinMode(DIR_Y, OUTPUT);
  pinMode(DIR_Z, OUTPUT);
  pinMode(DIR_E, OUTPUT);

  pinMode(STOP_X, INPUT);
  pinMode(STOP_Y, INPUT);
  pinMode(STOP_Z, INPUT);
  pinMode(STOP_E, INPUT);

  // PIN FAN 1
  pinMode(FAN1, OUTPUT);

  digitalWrite(FAN1, HIGH);

  // PIN FAN 2
  pinMode(FAN2, OUTPUT);

  digitalWrite(FAN2, HIGH);

  // PIN FAN 3
  pinMode(FAN3, OUTPUT);

  digitalWrite(FAN3, LOW);

  // Activer les drivers
  digitalWrite(EN_X, LOW);
  digitalWrite(EN_Y, LOW);
  digitalWrite(EN_Z, LOW);
  digitalWrite(EN_E, LOW);


  // pour assurer la prise d'info des tmc (ne pas reater le sgnal diag)
  attachInterrupt(digitalPinToInterrupt(STOP_X), stallInterruptX, RISING);
  attachInterrupt(digitalPinToInterrupt(STOP_Y), stallInterruptY, RISING);


  //----------------------------------------------------------------
  //gestion du pwm fan

  analogWriteResolution(10);  // resolution sur 10bits donc 0-1024
  analogWriteFreq(1000);      // frequence des puls

  messageln("=== Initialisation drivers ===");


  //---------------------------------------------------------------
  // Initialiser chaque TMC
  resetTMC();

  //----------------------------------------------------------------
  //led vert fin setup
  strip.clear();
  strip.fill(strip.Color(0, 255, 0));  // toutes les LEDs en vert
  strip.show();                        // applique à toutes les LEDS
  messageln("=== Initialisation terminée ===");
  delay(1000);
}
///pour gere les pin diag des tmc avec le rp 2040
void stallInterruptX() {
  stalled_X = true;
}
//..................................................................
void stallInterruptY() {
  stalled_Y = true;
}
//.........................................................................
void loop() {

  communication();
}