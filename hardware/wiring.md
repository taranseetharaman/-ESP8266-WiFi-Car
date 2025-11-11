# Wiring (ESP8266 + L298N)

- **Power** L298N from appropriate battery (e.g., 7.4–12V per your motors/module).
- **ESP8266 VIN/5V** typically from a regulated 5V source (ensure common GND).
- **Motor A** → OUT1/OUT2, **Motor B** → OUT3/OUT4.
- **NodeMCU pins:**
  - D1=enA, D2=in1, D3=in2, D4=in3, D5=in4, D6=enB
  - D7=buzzer, D8=headlight LED (with resistor), D0=Wi-Fi LED
- If your L298N has ENA/ENB jumpers:
  - Leave jumpers in place for **full speed** (we drive EN pins HIGH).
  - Remove and use PWM on D1/D6 for speed control if needed.
