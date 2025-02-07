#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

const char* ssid = "Tung Duong";
const char* password = "1234567890";

const int ledPin = 5;
int brightness = 128;

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {
    Serial.begin(115200);
    
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

    pinMode(ledPin, OUTPUT);
    ledcSetup(0, 5000, 8);
    ledcAttachPin(ledPin, 0);
    ledcWrite(0, brightness);

    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("value")) {
            brightness = request->getParam("value")->value().toInt();
            ledcWrite(0, brightness);
        }
        request->send(200, "text/plain", "OK");
    });

    server.onNotFound(notFound);
    server.begin();
}

void loop() {}
