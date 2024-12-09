#include <Arduino.h>
#include <ESP32Encoder.h>

// Définition des broches pour le capteur à effet Hall
#define HALL_PIN_A 27  // GPIO 34 pour la broche A du capteur à effet Hall
#define HALL_PIN_B 26 // GPIO 35 pour la broche B du capteur à effet Hall

// Créer une instance de l'encodeur
ESP32Encoder encoder;

void setup() {
  // Initialiser le moniteur série pour afficher les valeurs
  Serial.begin(115200);

  // Initialiser l'encodeur avec les deux signaux A et B (mode quadrature)
  encoder.attachFullQuad(HALL_PIN_A, HALL_PIN_B);  // Mode quadrature pour détecter la direction

  // Afficher un message d'introduction
  Serial.println("Capteur à Effet Hall - ESP32 Encoder");
  Serial.println("En attente de la rotation du capteur...");
}

void loop() {
  // Lire la valeur actuelle de l'encodeur (compte des impulsions)
  int32_t position = encoder.getCount(); // Position = nombre d'impulsions détectées

  // Lire les états des broches A et B
  int A = digitalRead(HALL_PIN_A);
  int B = digitalRead(HALL_PIN_B);

  // Afficher la position dans le moniteur série
  Serial.print("Position (nombre d'impulsions) : ");
  Serial.print(position);

  // Déterminer la direction en fonction des états des broches A et B
  if (A == B) {
    Serial.print(" | Sens : Horaire");
  } else {
    Serial.print(" | Sens : Anti-Horaire");
  }

  // Afficher la nouvelle ligne
  Serial.println();

  // Attendre un peu pour éviter une surcharge du moniteur série
}