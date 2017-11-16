# Lab-temperature-monitor
Arduino script to set a ESP32 chip for temperature and humidity monitor. Data are streamed via Wi-Fi and logged to ThingSpeak, an open Internet of Things platform.

"ThingSpeakMultichannel.html" generates the graphs of temperature and humidity, so you can easily embed them in a webpage.


Hardware
--------

  - Chip: ESP32
  - Temperature and humidity sensor: DHT22
    
ESP32's pin 27 can be used for data acquisition. If you change it remember to modify `pinDHT22` in the Arduino's sketches,

Software
-------- 
Three Arduino sketches are provided, depending on the user needs:

  - "Monitor.ino" This version uses the standard Arduino library for Wifi and SimpleDHT as DHT library... 
  - "Monitor_adafruit.ino" This version uses ESP32's wifi library and Adafruit's library to read the DHT22. Seems to be more reliable than the previous on ESP32 platforms
  - "Monitor_adafruit_wpa2ent.ino" Like the previous one but uses different Wifi libraries for the connection with WPA2 Enterprise networks.

Installation
------------

Install [Arduino IDE](https://www.arduino.cc/en/main/software).

Install [Arduino core for the ESP32](https://github.com/espressif/arduino-esp32).

For the Unified Sensors sketch install [Adafruit's Unified Sensor library](https://github.com/adafruit/Adafruit_Sensor)
and Adafruit's [ DHT library ](https://github.com/adafruit/DHT-sensor-library).

Install the Arduino library for the DHT sensor:

  - Open the Arduino IDE
  - Go to Sketch -> #include library -> library manager
  - Search "SimpleDHT" and install it


Insert Wi-Fi credentials and API key:

  - Open Monitor.ino
  - Insert the SSID name and password (WPA2 is supported)
  - Insert the write API key, for streaming to ThingSpeak

Flash the ESP32 chip:

  - Connect the chip via USB
  - Make your ESP32 go into downloading/uploading mode by holding the BOOT button and pressing the EN button ( if present ). This will reboot your ESP32
  - Open the Arduino IDE
  - Go to Tools -> Board and choose "ESP32 Dev Module"
  - Set the port and bauderate
  - Open "Monitor.ino", "Monitor_adafruit.ino" or "Monitor_adafruit_wpa2ent.ino"
  - Press Upload botton in the Arduino IDE

Optionally, embed ThingSpeakMultichannel.html into your webpage:

  - Open "ThingSpeakMultichannel.html"
  - Insert the channel number and the read API key


Test
----

You can test if the sensor is correctly working by running "test_simpleDHT.ino".

If you need the MAC address of the ESP32 for Wi-Fi connection run "get_ESP32_MAC_address.ino".
