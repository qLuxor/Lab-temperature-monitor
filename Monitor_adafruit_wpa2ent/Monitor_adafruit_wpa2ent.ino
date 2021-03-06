/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include "esp_wpa2.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
// for DHT22, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
#define DHTPIN  13
int analogInPin = A6;
int light = 0;


DHT_Unified dht(DHTPIN, DHTTYPE);

const char* ssid = "SSID"; // your ssid
#define EAP_ID ""
#define EAP_USERNAME ""
#define EAP_PASSWORD ""

const char* host = "api.thingspeak.com";
const char* privateKey = "APIKEY";

bool wifiSetup()
{
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    delay(10);
    
    WiFi.disconnect(true);      
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ID, strlen(EAP_ID));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
    esp_wifi_sta_wpa2_ent_enable(&config);
    // WPA2 enterprise magic ends here
    WiFi.begin(ssid);
    
    int connection_timeout = 0;
    while (WiFi.status() != WL_CONNECTED && connection_timeout < 60) {
        delay(500);
        ++connection_timeout;
        Serial.print(".");
    }

    if (connection_timeout < 60) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    else {
        Serial.println("");
        Serial.println("WiFi not connected");
        return false;
    }
}

void setup()
{
    Serial.begin(115200);
    delay(10);
    pinMode( analogInPin, INPUT);
    // We start by connecting to a WiFi network
    while(!wifiSetup());

    dht.begin();
    Serial.println("DHTxx Unified Sensor Example");
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Temperature");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
    Serial.println("------------------------------------");
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Humidity");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");
    Serial.println("------------------------------------");  
}


void loop()
{
    delay(100);
    sensors_event_t event;
    // read without samples.
    // @remark We use read2 to get a float data, such as 10.1*C
    //    if user doesn't care about the accurate data, use read to get a byte data, such as 10*C.
    float temperature = 0;
    float humidity = 0;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.print("Read DHT22 tempearture failed");delay(2000);
      return;
    }
    temperature=event.temperature;
    
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println("Error reading humidity!");
      return;
    }
    humidity=event.relative_humidity;
    Serial.print("Sample OK: ");
    Serial.print((float)temperature); Serial.print(" *C, ");
    Serial.print((float)humidity); Serial.println(" RH%");

    light = analogRead(analogInPin);            
    if (isnan(light)) {
      Serial.println("Error reading light!");
      return;
    }
    Serial.print("Light = " );
    Serial.println(light);  
    
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        while(!wifiSetup());
        return;
    }

    String url = "/update";
    url += "?api_key=";
    url += privateKey;
    url += "&field1=";
    url += (float)temperature;
    url += "&field2=";
    url += (float)humidity;
    url += "&field3=";
    url += (float)light;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
    
    delay(60000);
}

