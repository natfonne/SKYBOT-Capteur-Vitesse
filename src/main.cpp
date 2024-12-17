#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <AsyncWebSocket.h>

// Remplace par tes informations WiFi
const char* ssid = "Iphonenathan";
const char* password = "NaThAnWifi";

// GPIO pour le bouton poussoir
#define BUTTON_PIN 14

int compteur = 0;
int lastButtonState = HIGH;
int buttonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void notifyClients() {
  String message = "Compteur : " + String(compteur);
  ws.textAll(message);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("Initialisation du SPIFFS...");
  if (!SPIFFS.begin(true)) {
    Serial.println("Erreur SPIFFS...");
    return;
  }
  Serial.println("SPIFFS monté avec succès.");

  Serial.println("Connexion au WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Tentative de connexion au WiFi...");
  }
  Serial.println("WiFi connecté");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  ws.onEvent([](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
    }
  });

  server.addHandler(&ws);
  server.begin();
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN) == LOW;

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        compteur++;
        Serial.print("Compteur : ");
        Serial.println(compteur);
        notifyClients();
      }
    }
  }
  lastButtonState = reading;
  delay(50);
}
