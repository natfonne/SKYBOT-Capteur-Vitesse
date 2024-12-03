#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Encoder.h>
#include <ArduinoJson.h>

// Configuration Wi-Fi
const char* ssid = "BTS_CIEL";
const char* password = "ERIR1234";
// Set LED GPIO
const int ledPin = 2; // Led intégrée à l'ESP32
// Stores LED state
String ledState;
// Potentiomètre relié à GPIO 34 (Analog ADC1_CH6)
const int potPin = 34;
String message = "";

// Pins pour les capteurs Hall
#define HALL_SENSOR_PIN1 34  // Capteur Hall 1
#define HALL_SENSOR_PIN2 35  // Capteur Hall 2

// Initialisation de l'encodeur
ESP32Encoder encoder;

// Serveur Web Asynchrone sur le port 80
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
// sans JSON :
if (message == "ON") digitalWrite(ledPin, HIGH);
else digitalWrite(ledPin, LOW);
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

void setup() {
  // Initialisation de la communication série
  Serial.begin(9600);
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
}
void loop(){
delay(500);
notifyClients(String(analogRead(potPin)));
// ws.cleanupClients();
}