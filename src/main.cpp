#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <ESP32Encoder.h>

// Définition des broches pour le capteur à effet Hall
#define HALL_PIN_A 27  // GPIO 34 pour la broche A du capteur à effet Hall
#define HALL_PIN_B 26  // GPIO 35 pour la broche B du capteur à effet Hall
const char *ssid = "BTS_CIEL";
const char *password = "ERIR1234";
String message = "";
// Création d'un serveur sur le port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

//------------Configuration du serveur WebSocket-------------------
void notifyClients(String state) {
ws.textAll(state);
}
//////////////// Gérer les messages reçus //////////////////
//------------Fonction callback : elle est appelée chaque fois qu'une donnée a été reçue ---------------------//
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
AwsFrameInfo *info = (AwsFrameInfo*)arg;
if (info->final && info->index == 0 && info->len == len && info->opcode ==
WS_TEXT) {
data[len] = 0;
message = (char*)data;
}
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
AwsEventType type, void *arg, uint8_t *data, size_t len) {
switch (type) {
case WS_EVT_CONNECT:
Serial.printf("Client WebSocket #%u connecté depuis : %s\n", client->id(), client->remoteIP().toString().c_str());
break;
case WS_EVT_DISCONNECT:
Serial.printf("Client WebSocket #%u déconnecté\n", client->id());
break;
case WS_EVT_DATA:
handleWebSocketMessage(arg, data, len);
break;
case WS_EVT_PONG:
case WS_EVT_ERROR:
break;
}
}
void initWebSocket() {
ws.onEvent(onEvent);
server.addHandler(&ws);
}
//---------------------------------------------------------------------

// Créer une instance de l'encodeur
ESP32Encoder encoder;

int lastA = LOW;  // Dernier état de la broche A
int lastB = LOW;  // Dernier état de la broche B
int direction = 0; // Variable pour stocker la direction (1 pour horaire, -1 pour antihoraire)

void setup() {
  Serial.begin(115200);
//---------------------------SPIFFS-------------------
if(!SPIFFS.begin()) /* Démarrage du gestionnaire de fichiers SPIFFS */
{
Serial.println("Erreur SPIFFS...");
return;
}
//-----------------------WIFI-----------------------------
WiFi.begin(ssid, password); /* Connexion au réseau Wifi */
Serial.print("Tentative de connexion...");
while(WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(100);
}
Serial.println("\n");
Serial.println("Connexion etablie!");
Serial.print("Adresse IP: ");
Serial.println(WiFi.localIP());

//--------------------------SERVEUR--------------------------
initWebSocket();
/* Lorsque le serveur est actif , la page index.html est chargée */

server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(SPIFFS, "/style.css", "text/css");
});
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
{
request->send(SPIFFS, "/index.html", "text/html");
});
server.serveStatic("/", SPIFFS, "/");
// Démarrage du serveur
server.begin();
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

  // Si la position atteint 225, réinitialiser l'encodeur à 0
  if (position >= 220) {
    encoder.setCount(0);  // Réinitialiser la position de l'encodeur à 0
    Serial.println("Réinitialisation à 0 (sens horaire)");  // Afficher que la réinitialisation a eu lieu
  }

  // Si la position atteint -225, réinitialiser l'encodeur à 0
  if (position <= -220) {
    encoder.setCount(0);  // Réinitialiser la position de l'encodeur à 0
    Serial.println("Réinitialisation à 0 (sens antihoraire)");  // Afficher que la réinitialisation a eu lieu
  }

  // Lire les états des broches A et B
  int A = digitalRead(HALL_PIN_A);
  int B = digitalRead(HALL_PIN_B);

  // Déterminer la direction
  if (A != lastA || B != lastB) {  // Si un état des broches a changé
    if (A == lastA) {
      // Si A n'a pas changé, cela signifie que nous tournons dans le sens horaire
      direction = 1;
    } else if (B == lastB) {
      // Si B n'a pas changé, cela signifie que nous tournons dans le sens antihoraire
      direction = -1;
    }
  }

  // Afficher la position dans le moniteur série
  Serial.print("Nombre d'impulsions : ");
  Serial.print(position);

  // Afficher la direction (horaire ou antihoraire)
  if (direction == 1) {
    Serial.print(" | Sens : Horaire");
  } else if (direction == -1) {
    Serial.print(" | Sens : Antihoraire");
  }

  // Mettre à jour les derniers états des broches
  lastA = A;
  lastB = B;

  // Afficher la nouvelle ligne
  Serial.println();

  // Attendre un peu pour éviter une surcharge du moniteur série
  delay(100);  // Ajuste cette valeur selon tes besoins pour plus de précision
  notifyClients(String());
}