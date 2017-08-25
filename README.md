# Lab-temperature-monitor
Arduino script to set a ESP32 chip for temperature and humidity monitor. Data are streamed via Wi-Fi and logged to ThingSpeak, an open Internet of Things platform.

"ThingSpeakMultichannel.html" generates the graphs of temperature and humidity, so you can easily embed them in a webpage.


Hardware
--------

  - chip: ESP32
  - temperature and humidity sensor: DHT22
    
ESP32's pin 27 can be used for data acquisition. If you change it remember to modify `pinDHT22` in "Monitor.ino".

Installation
------------

Install [Arduino IDE](https://www.arduino.cc/en/main/software).

Install [Arduino core for the ESP32](https://github.com/espressif/arduino-esp32).

Install the Arduino library for the DHT sensor:

  - Open the Arduino IDE
  - Go to Sketch -> #include library -> library manager
  - Search "SimpleDHT" and install it

Insert Wi-Fi credentials and API key:

  - Open Monitor.ino
  - Insert the SSID name and password (WPA2 is not supported)
  - Insert the write API key, for streaming to ThingSpeak

Flash the ESP32 chip:

  - Connect the chip via USB
  - Make your ESP32 go into downloading/uploading mode by holding the BOOT button and pressing the EN button. This will reboot your ESP32
  - Open the Arduino IDE
  - Go to Tools -> Board and choose "ESP32 Dev Module"
  - Set the port and bauderate
  - Press Upload botton in the Arduino IDE

Optionally, embed ThingSpeakMultichannel.html into your webpage:

  - Open "ThingSpeakMultichannel.html"
  - Insert the channel number and the read API key


Test
----

You can test if the sensor is correctly working by running "Test_simpleDHT.ino".

If you need the MAC address of the ESP32 for Wi-Fi connection run "get_ESP32_MAC_address.ino".
