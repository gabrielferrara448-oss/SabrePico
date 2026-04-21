
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
void gestionAllumerZone(String line) {

  int debut1, fin1, debut2, fin2;
  int r1, g1, b1, r2, g2, b2;

  int idxZone1 = line.indexOf("zone1");
  int idxZone2 = line.indexOf("zone2");

  if (idxZone1 == -1 || idxZone2 == -1) return;

  // --- Parser zone1 ---
  String bloc1 = line.substring(idxZone1, idxZone2);

  int aCouleur1 = bloc1.indexOf("couleur");
  String positions1 = bloc1.substring(6, aCouleur1);
  String couleurStr1 = bloc1.substring(aCouleur1 + 7);

  // Extraire debut1 et fin1
  int idxA1 = positions1.indexOf(" a ");
  String s_debut1 = positions1.substring(0, idxA1);
  String s_fin1   = positions1.substring(idxA1 + 3);
  s_debut1.trim();
  s_fin1.trim();
  debut1 = s_debut1.toInt();
  fin1   = s_fin1.toInt();

  // Extraire r1, g1, b1
  int virgule1a = couleurStr1.indexOf(',');
  int virgule1b = couleurStr1.indexOf(',', virgule1a + 1);
  String s_r1 = couleurStr1.substring(0, virgule1a);
  String s_g1 = couleurStr1.substring(virgule1a + 1, virgule1b);
  String s_b1 = couleurStr1.substring(virgule1b + 1);
  s_r1.trim(); s_g1.trim(); s_b1.trim();
  r1 = s_r1.toInt();
  g1 = s_g1.toInt();
  b1 = s_b1.toInt();

  // --- Parser zone2 ---
  String bloc2 = line.substring(idxZone2);

  int aCouleur2 = bloc2.indexOf("couleur");
  String positions2 = bloc2.substring(6, aCouleur2);
  String couleurStr2 = bloc2.substring(aCouleur2 + 7);

  // Extraire debut2 et fin2
  int idxA2 = positions2.indexOf(" a ");
  String s_debut2 = positions2.substring(0, idxA2);
  String s_fin2   = positions2.substring(idxA2 + 3);
  s_debut2.trim();
  s_fin2.trim();
  debut2 = s_debut2.toInt();
  fin2   = s_fin2.toInt();

  // Extraire r2, g2, b2
  int virgule2a = couleurStr2.indexOf(',');
  int virgule2b = couleurStr2.indexOf(',', virgule2a + 1);
  String s_r2 = couleurStr2.substring(0, virgule2a);
  String s_g2 = couleurStr2.substring(virgule2a + 1, virgule2b);
  String s_b2 = couleurStr2.substring(virgule2b + 1);
  s_r2.trim(); s_g2.trim(); s_b2.trim();
  r2 = s_r2.toInt();
  g2 = s_g2.toInt();
  b2 = s_b2.toInt();

  // --- Appel final ---
  uint32_t couleur1 = strip.Color(r1, g1, b1);
  uint32_t couleur2 = strip.Color(r2, g2, b2);
  allumerZone(debut1, fin1, couleur1, debut2, fin2, couleur2);
}