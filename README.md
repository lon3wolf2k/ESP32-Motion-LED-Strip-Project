# ESP32 PIR-Activated Fading Knight Rider LED Strip

This project uses an **ESP32 DevKit v4**, a **WS2812/Neopixel LED strip**, and an **HC-SR501 PIR sensor**  
to create a **color-cycling Knight Rider (Larson Scanner) effect with smooth fading tails**.

## 🔥 Features
- Double-direction Knight Rider animation  
- Smooth fading LED tails  
- Automatic color cycling (Orange → White → Red → Blue → repeat)  
- PIR sensor activates animation  
- LED strip turns off 10 seconds after no motion  
- Fully compatible with ESP32 DevKit v4

## 🪛 Hardware Connections

### PIR Sensor (HC-SR501)
- VCC → **5V main power supply**
- GND → **5V PSU GND**
- OUT → **ESP32 GPIO 27**

### WS2812 LED Strip
- +5V → **5V main power supply**
- GND → **5V PSU GND**
- DIN → **ESP32 GPIO 5**

### Grounding (IMPORTANT)
All grounds must be tied together:
- 5V PSU GND  
- ESP32 GND  
- PIR GND  
- LED Strip GND  

This ensures proper data signaling to WS2812 LEDs.

## 📁 Files Included
- `main.ino` — Full Arduino sketch  
- `README.md` — Setup and wiring instructions

## 🚀 Uploading
Use **Arduino IDE** or **PlatformIO** with:
- Board: `ESP32 Dev Module`
- Library: `Adafruit NeoPixel`

Enjoy your fully animated sensor-reactive LED strip!
