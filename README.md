# ESP8266 Wi-Fi Car (AP Mode, L298N, OTA)

Self-hosted Wi-Fi car using **ESP8266 AP mode**. No router required: the board creates its own hotspot; your phone connects to it and sends simple HTTP commands to drive the car. Includes **OTA** and a minimal web UI.

> **Security:** SSID/password are stored in `firmware/src/config.h` (local only). The repo only ships `config.h.example` with placeholders.

## Features
- AP mode: phone connects directly to car (no internet needed)
- L298N motor driver (2 DC motors)
- Commands: **F**(Forward), **B**(Backward), **L**(Left), **R**(Right), **S**(Stop)  
  Extras: **V**(Horn beep), **W**(Light ON), **w**(Light OFF)
- Simple web UI + URL API (`/?State=F`)
- OTA updates

## Default Pins (NodeMCU/ESP8266)
| Function            | Pin |
|--------------------:|:---:|
| enA (Motor A EN)    | D1  |
| in1                 | D2  |
| in2                 | D3  |
| in3                 | D4  |
| in4                 | D5  |
| enB (Motor B EN)    | D6  |
| Buzzer              | D7  |
| Headlight LED       | D8  |
| Wi-Fi status LED    | D0  |

> Adjust pins in `main.cpp` if your wiring differs.

## Build

### Arduino IDE
1. Install **ESP8266 board** support + libraries **ESP8266WebServer**, **ArduinoOTA**.
2. Copy `firmware/src/config.h.example` → **`firmware/src/config.h`** and fill your AP name/password.
3. Open `firmware/src/main.cpp` → Board: **NodeMCU 1.0 (ESP-12E)** → Upload.

### PlatformIO
```bash
cd firmware
pio run -t upload
