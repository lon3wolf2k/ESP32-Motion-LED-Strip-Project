# ESP32 Motion-Activated LED Strip (WS2812B + PIR Sensor)
This project uses an ESP32, a PIR motion sensor, and a WS2812B addressable LED strip 
to automatically light up a warm-white fade effect when someone approaches a step 
or warning sign. When no motion is detected, the LEDs fade out smoothly and turn off.

---

## ✨ Features
- Smooth **fade-in** when motion is detected  
- Smooth **fade-out** after timeout  
- Warm-white LED color  
- Fully configurable LED count, fade speed, brightness, and ON duration  
- Uses only **one data pin** (GPIO 5)  
- Works with 5V WS2812B LED strips (30 or 60 LEDs per meter)

---

## 🧰 Hardware Required
- ESP32 Dev Module  
- PIR Sensor (HC-SR501 or similar)  
- WS2812B 5V Addressable LED strip (2–3 meters recommended)  
- 5V USB power supply (2–3A)  
- Jumper wires  
- Optional:  
  - 220–470 Ω resistor for data line  
  - 1000 µF capacitor between 5V and GND for voltage smoothing  

---

## 🪛 Wiring Diagram (Text/ASCII)
