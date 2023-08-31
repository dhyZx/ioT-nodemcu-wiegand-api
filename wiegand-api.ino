#include <Wiegand.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "nodemcu";//SSID YOUR WIFI 
const char *pass = "12345678";//PASSWORD YOUR SSID
const char* serverName = "http://absensi.connectis.my.id/api/absen";//YOUR API

WiFiClient client;
WIEGAND wg0;
HTTPClient http;

void setup() {
    Serial.begin(9600);  
    Serial.print(" Connect to : ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("….");
    }

    Serial.print("\n");
    Serial.print("IP address : ");
    Serial.print(WiFi.localIP());
    Serial.print("\n");
    Serial.print("Connect to : ");
    Serial.println(ssid);
    //PIN OF D0,D1 WIEGAND
    wg0.begin(D4,D3);
}

void loop() {
    //WIEGAND READY
    if(wg0.available()){
    //GETTING ID FOR NFC CARD
    unsigned long wiegandData = wg0.getCode();
    //HTTP
    http.begin(client, serverName);
    //GET DATA HEX FROM WIEGAND
    String UIDD = "{\"uid\":\"" + String(wiegandData, HEX) + "\"}";
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(UIDD);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    http.end();
    }
}