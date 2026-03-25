#ifndef COMMUNICATION_SERIE_H
#define COMMUNICATION_SERIE_H

#include "globals.h"


bool communication();
//.........................................................................
String reponsecom();
//.........................................................................
void treteMessageRecu(String line);
//..............................................................................
void message(String message);
//..............................................................................
void messageln(String message);
#endif