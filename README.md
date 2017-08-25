# Lab-temperature-monitor
Arduino script to set a ESP32 chip for temperature and humidity monitor. Data are streamed via Wi-Fi and logged to ThingSpeak, an open Internet of Things platform.

ThingSpeakMultichannel.html generates the graphs of temperature and humidity, so you can easily embed them in a webpage.


Hardware
--------

    * chip: ESP32
    * temperature and humidity sensor: DHT22
    

Install
-------

Install [Arduino IDE](https://www.arduino.cc/en/main/software).

Install [Arduino core for the ESP32](https://github.com/espressif/arduino-esp32).

Install the arduino library for the DHT sensor:
Open Arduino, go to Sketch -> #include library -> library manager and install "SimpleDHT".

Insert Wi-Fi credentials and API key:
Open Monitor.ino and insert SSID name and password. For streaming to ThingSpeakInsert, insert the write API key.

Flash the ESP32 chip:
Connect the chip via USB. Open Arduino IDE, go to Tools -> Scheda and choose "ESP32 Dev Module". Set the port and bauderate, then flash the script.

Optionally, embed ThingSpeakMultichannel.html into your webpage:
Open ThingSpeakMultichannel.html and insert channel number and read API key.



