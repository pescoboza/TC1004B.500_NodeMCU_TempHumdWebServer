#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WIFIClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#include "config.h"

#define BUILTIN_LED 2
#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
ESP8266WebServer server{80}; // Object for web server on port 80

// View endpoint functions for the server
namespace view{
    void index(){
        digitalWrite(BUILTIN_LED, LOW);
        server.send(200, "text/plain", "ESP-12 Web server.");
        digitalWrite(BUILTIN_LED, HIGH);
    }

    // Returns message for humidity and temparature
    void read(){
        float humd{dht.readHumidity()};
        float temp{dht.readTemperature()};
        String msg{"The temperature is "};
        msg += temp;
        msg += " C and the humidity is ";
        msg += humd;
        msg += "%.";
        server.send(200, "text/plain", msg);
    }

    void notFound(){
        digitalWrite(BUILTIN_LED, LOW);
        server.send(404, "text/plain", "Not found.");
        digitalWrite(BUILTIN_LED, HIGH);
    }
};

// Setting the views to the endpoint uris
void setupServer(){
    server.on("/", view::index);
    server.on("/read", view::read);
    server.onNotFound(view::notFound);

}

void setup() {
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);
    
    // Begin the sensor
    dht.begin();
    
    // Begin the serial
    Serial.begin(9600);

    // Connect to WiFi net
    WiFi.mode(WIFI_STA); // Activate station mode
    WiFi.begin(cfg::SSID, cfg::PASSWORD);
    Serial.println("");
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print('.');
    }

    // Print net information
    Serial.println("");
    Serial.print("Connected to net: ");
    Serial.println(cfg::SSID);
    Serial.print("IP:");
    Serial.println(WiFi.localIP());

    // Begin the HTTP server
    setupServer();
    server.begin();
    Serial.println("Initialized HTTP server");
}

void loop() {
    // Handle client requests
    server.handleClient();
}