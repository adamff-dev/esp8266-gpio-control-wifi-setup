# ESP8266 WiFi Configuration Web Server

This is an Arduino sketch for the ESP8266 microcontroller that creates a simple web server for configuring WiFi settings and controling all NodeMCU's GPIOs. It allows you to specify the SSID, password, IP address, and subnet mask for connecting the ESP8266 to a WiFi network. The code is written in C++ and uses the ESP8266WiFi and ESP8266WebServer libraries, as well as EEPROM for storing WiFi credentials.

## Features

- Configures WiFi settings (SSID, password, IP address, and subnet mask) via a web interface.
- Stores the WiFi credentials in EEPROM for persistence across power cycles.
- Supports turning GPIO pins on and off via HTTP requests.

## Prerequisites

Before using this code, ensure you have the following:

- Arduino IDE or PlatformIO with ESP8266 support installed.
- ESP8266 development board (e.g., NodeMCU).
- Required libraries: ESP8266WiFi, ESP8266WebServer, EEPROM, ArduinoJson.

## Hardware Setup

1. Connect your ESP8266 board to your computer.
2. Open the Arduino IDE or PlatformIO.
3. Install the required libraries mentioned in the code.
4. Upload the code to your ESP8266 board.

## Usage

1. Flash the code onto your ESP8266 board.
2. Connect to the ESP8266's WiFi access point (AP) named "NodeMCU."
3. Open a web browser and navigate to `http://192.168.4.1` to access the configuration page.
4. Enter your WiFi SSID, password, IP address, and subnet mask, and click "Save" to configure the ESP8266.
5. The ESP8266 will save the settings to EEPROM, connect to the specified WiFi network, and restart.
6. After the restart, the ESP8266 will have the new WiFi settings.

You can also control GPIO pins by sending HTTP requests to the ESP8266. To do this, make HTTP GET requests to `/control` with the parameters "pin" and "action." The "pin" parameter specifies the GPIO pin (e.g., "D0"), and the "action" parameter specifies whether to turn the pin "on" or "off."

Example:
- To turn on GPIO pin D1: `http://<ESP8266_IP>/control?pin=D1&action=on`
- To turn off GPIO pin D2: `http://<ESP8266_IP>/control?pin=D2&action=off`

## Code Structure

- The code defines the SSID, password, and network settings.
- It creates a web server on port 80.
- The `handleRoot` function serves a web page for configuring WiFi settings.
- The `handleSave` function handles form submissions, saves settings to EEPROM, and restarts the ESP8266.
- WiFi credentials are stored in EEPROM.
- The `handleControl` function manages GPIO pin control through HTTP requests.
- The `getPinValue` function maps pin names (e.g., "D1") to GPIO pin numbers.
- The `setup` function initializes the WiFi and web server, loading saved credentials if available.
- The `loop` function handles incoming HTTP requests.

## Disclaimer

This code provides basic WiFi configuration and control functionality for the ESP8266. It's essential to take security precautions when deploying IoT devices on public networks and to consider security best practices for your specific use case.
