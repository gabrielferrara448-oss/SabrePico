#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>

// --- Pins ---
#define R_SENSE 0.11f
#define STEP_X 11
#define DIR_X  10
#define EN_X   12
#define STOP_X 4
#define STEP_Y 6
#define DIR_Y  5
#define EN_Y   7
#define STOP_Y 3
#define STEP_Z 19
#define DIR_Z  28
#define EN_Z   2
#define STOP_Z 25
#define STEP_E 14
#define DIR_E  13
#define EN_E   15
#define STOP_E 16
#define LED_RGB 24
#define FAN1 17
#define FAN2 18
#define FAN3 20

// --- Variables globales (extern = déclaration sans création) ---
extern long posX;
extern long posY;
extern int vMax;
extern int vMin;
extern bool stalled_X;
extern bool stalled_Y;
extern bool shaftVal;
extern int conteur_de_ping;
extern Adafruit_NeoPixel strip;
extern TMC2209Stepper driverX;
extern TMC2209Stepper driverY;
extern TMC2209Stepper driverZ;
extern TMC2209Stepper driverE;

#endif