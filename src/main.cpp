#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WIFIClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#include "config.h"

#define BUILTIN_LED 2

ESP8266WebServer server{80};

namespace view{
    void index(){
        digitalWrite(BUILTIN_LED, LOW);
        server.send(200, "text/plain", "ESP-12 Web server.");
        digitalWrite(BUILTIN_LED, HIGH);
    }

    void notFound(){
        digitalWrite(BUILTIN_LED, LOW);
        server.send(404, "text/plain", "Not found.");
        digitalWrite(BUILTIN_LED, HIGH);
    }
};


void setupServer(){
    server.on("/", view::index);
    server.onNotFound(view::notFound);

}

void setup() {
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);

    Serial.begin(9600);

    WiFi.mode(WIFI_STA); // Activate station mode
    WiFi.begin(cfg::SSID, cfg::PASSWORD);
    Serial.println("");

    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print('.');
    }

    Serial.println("");
    Serial.print("Connected to net: ");
    Serial.println(cfg::SSID);
    Serial.print("IP:");
    Serial.println(WiFi.localIP());

    setupServer();
    server.begin();
    Serial.println("Initialized HTTP sever");
}

void loop() {
    server.handleClient();
}