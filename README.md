# IITM DIY School Connect ESP8266 Bot Car

## Introduction
This project provides the code and resources for building and controlling an ESP8266-based bot car as part of the IITM DIY School Connect initiative. The bot can be assembled with either normal wheels or Mecanum wheels, allowing for different movement capabilities. The ESP8266 microcontroller is programmed to create a WiFi hotspot, enabling wireless control of the bot via a dedicated mobile app.

## Features
- Wireless control using ESP8266 WiFi
- Two drive options: Normal wheels and Mecanum wheels
- Compatible WiFi Robot Car app
- Adjustable speed control

## Getting Started

## Connection Diagram
Refer to the following image for wiring and hardware setup:

![Connection Diagram](Connection%20Diagram.jpg)

### 1. Download and Install Arduino IDE
- [Arduino IDE Download](https://www.arduino.cc/en/software)

### 2. Add ESP8266 Board to Arduino IDE
- Open Arduino IDE
- Go to **Preferences**
- In the "Additional Boards Manager URLs" field, paste the following link:
  - `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
- Go to **Tools > Board > Boards Manager**
- Search for **ESP8266** and install it

### 3. Download the Controller App
- [ESP8266 WiFi Robot Car App (Android)](https://play.google.com/store/apps/details?id=com.bluino.esp8266wifirobotcar&pli=1)

### 4. Upload the Code
- Use the provided `.ino` files:
  - `normal_wheel_bot.ino` for bots with normal wheels
  - `mecanum_wheel_bot.ino` for bots with Mecanum wheels
- Open the desired file in Arduino IDE
- Select the correct board (**NodeMCU 1.0 (ESP-12E Module)** or similar)
- Connect your ESP8266 board and upload the code

### 5. Connect and Control
- Power on the bot. It will create a WiFi hotspot (default SSID: `Namai's Robo`)
- Connect your phone to this WiFi network
- Open the Bluino app and control your bot

## Resources
- **Board Link (for Arduino Preferences):**
  - `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
- **App Link (Controller):**
  - [Bluino ESP8266 WiFi Robot Car App](https://play.google.com/store/apps/details?id=com.bluino.esp8266wifirobotcar)
- **Arduino IDE Download:**
  - [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

## Tutorial Videos
- [Normal Wheels Bot Tutorial](https://youtu.be/QIqrNbqF-RU?si=QlamUCG8KTXTk9-0)
- [Mecanum Wheels Bot Tutorial](https://youtu.be/-OaFbP7Gd48)

## File Overview
- `normal_wheel_bot.ino` — Code for bot with normal wheels
- `mecanum_wheel_bot.ino` — Code for bot with Mecanum wheels

---

**Developed by Namai: IITM DIY Core Team Lead for IITM DIY School Connect**
