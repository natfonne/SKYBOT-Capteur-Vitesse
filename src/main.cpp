#include <Arduino.h>

// Déclaration des variables
const int boutonPin = 4;  // Le bouton poussoir est connecté à la broche 2
int etatBouton = 0;       // Variable pour enregistrer l'état actuel du bouton
int compteurImpulsions = 0; // Compteur des impulsions du bouton

void setup() {
  // Initialiser la broche du bouton poussoir en entrée avec résistance pull-up
  pinMode(boutonPin, INPUT_PULLUP);
  
  // Initialiser la communication série
  Serial.begin(115200);
}

void loop() {
  // Lire l'état du bouton poussoir
  etatBouton = digitalRead(boutonPin);
  
  // Si le bouton est appuyé (état bas, car la broche est en pull-up)
  if (etatBouton == LOW) {
    // Incrémenter le compteur d'impulsions
    compteurImpulsions++;
    
    // Attendre que le bouton soit relâché (pour éviter plusieurs comptages pour une seule pression)
    while (digitalRead(boutonPin) == LOW) {
      delay(10); // Petites pauses pour éviter de trop saturer la boucle
    }
  }
  
  // Afficher le nombre d'impulsions dans le moniteur série
  Serial.print("Nombre d'impulsions : ");
  Serial.println(compteurImpulsions);
  
  // Petite pause pour la lisibilité
  delay(150);
}