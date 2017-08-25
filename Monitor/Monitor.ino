/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include <SimpleDHT.h>

// for DHT22, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT22 = 27;
SimpleDHT22 dht22;

const char* ssid     = "SSIDNAME";
const char* password = "PWD";

const char* host = "api.thingspeak.com";
const char* privateKey = "PRIVATEKEY";

bool wifiSetup()
{
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    int connection_timeout = 0;
    while (WiFi.status() != WL_CONNECTED && connection_timeout < 20) {
        delay(500);
        ++connection_timeout;
        Serial.print(".");
    }

    if (connection_timeout < 20) {
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

    // We start by connecting to a WiFi network
    while(!wifiSetup());
}


void loop()
{
    delay(100);
    // read without samples.
    // @remark We use read2 to get a float data, such as 10.1*C
    //    if user doesn't care about the accurate data, use read to get a byte data, such as 10*C.
    float temperature = 0;
    float humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht22.read2(pinDHT22, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT22 failed, err="); Serial.println(err);delay(2000);
      return;
    }
    Serial.print("Sample OK: ");
    Serial.print((float)temperature); Serial.print(" *C, ");
    Serial.print((float)humidity); Serial.println(" RH%");
    
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

