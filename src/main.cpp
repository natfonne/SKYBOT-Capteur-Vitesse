#include <ESP32Encoder.h>
#include <Arduino.h>

// Pin pour le signal du capteur Hall (vous pouvez utiliser GPIO 32 ou tout autre GPIO disponible)
const int hallSensorPinA = 32;  // Signal du capteur Hall (A)

ESP32Encoder encoder;  

void setup() {
  // Initialisation du port série pour afficher les résultats
  Serial.begin(115200);

  // Attacher le signal du capteur Hall à l'encodeur (utilisation de GPIO 32)
  encoder.attachHalfQuad(hallSensorPinA, -1);  // Le signal A sur GPIO 32

  // Configurer le pin du capteur Hall comme entrée
  pinMode(hallSensorPinA, INPUT);

  // Affichage de message initial sur le moniteur série
  Serial.println("Capteur à effet Hall prêt !");
}

void loop() {
  // Récupérer le comptage des impulsions
  int count = encoder.getCount();  // Nombre d'impulsions lues

  // Afficher le comptage des impulsions sur le moniteur série
  Serial.print("Comptage des impulsions : ");
  Serial.println(count);

  // Ajouter un petit délai avant de lire à nouveau
  delay(500);  // Attendre 500ms
}
