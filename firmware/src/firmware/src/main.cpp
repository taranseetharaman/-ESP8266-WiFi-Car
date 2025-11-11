/******************* ESP8266 WiFi Car - AP Mode (L298N + OTA) *******************/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include "config.h"

// ===== Pin Map (NodeMCU D-pins) =====
int enA = D1;  // Motor A enable (set HIGH = full speed; use PWM if you want speed control)
int in1 = D2;  // Motor A IN1
int in2 = D3;  // Motor A IN2
int in3 = D4;  // Motor B IN1
int in4 = D5;  // Motor B IN2
int enB = D6;  // Motor B enable

const int buzPin    = D7;  // Buzzer
const int headLed   = D8;  // Headlight LED
const int wifiLed   = D0;  // WiFi status LED (LOW = ON on many boards)

// ===== Globals =====
ESP8266WebServer server(80);
String command;

void stopAll() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
}

void forward()  { digitalWrite(in1,HIGH); digitalWrite(in2,LOW);  digitalWrite(in3,HIGH); digitalWrite(in4,LOW);  }
void backward() { digitalWrite(in1,LOW);  digitalWrite(in2,HIGH); digitalWrite(in3,LOW);  digitalWrite(in4,HIGH); }
void rightTurn(){ digitalWrite(in1,LOW);  digitalWrite(in2,HIGH); digitalWrite(in3,HIGH); digitalWrite(in4,LOW);  }
void leftTurn() { digitalWrite(in1,HIGH); digitalWrite(in2,LOW);  digitalWrite(in3,LOW);  digitalWrite(in4,HIGH); }

void beepHorn(){ digitalWrite(buzPin,HIGH); delay(150); digitalWrite(buzPin,LOW); }
void lightOn(){ digitalWrite(headLed, HIGH); }
void lightOff(){ digitalWrite(headLed, LOW); }

// ===== HTTP =====
const char* UI_HTML =
  "<!doctype html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'/>"
  "<style>body{font-family:sans-serif;margin:18px}button{padding:14px 18px;margin:6px;font-size:18px}</style>"
  "</head><body><h2>ESP8266 Car</h2>"
  "<div><button onclick=\"fetch('/?State=F')\">Forward</button>"
  "<button onclick=\"fetch('/?State=B')\">Backward</button></div>"
  "<div><button onclick=\"fetch('/?State=L')\">Left</button>"
  "<button onclick=\"fetch('/?State=R')\">Right</button>"
  "<button onclick=\"fetch('/?State=S')\">Stop</button></div>"
  "<div><button onclick=\"fetch('/?State=V')\">Horn</button>"
  "<button onclick=\"fetch('/?State=W')\">Light ON</button>"
  "<button onclick=\"fetch('/?State=w')\">Light OFF</button></div>"
  "<p>You can also call <code>/?State=F|B|L|R|S|V|W|w</code></p>"
  "</body></html>";

void handleRoot() {
  // Parse command if present
  if (server.hasArg("State")) {
    command = server.arg("State");
    if      (command == "F") forward();
    else if (command == "B") backward();
    else if (command == "R") rightTurn();
    else if (command == "L") leftTurn();
    else if (command == "S") stopAll();
    else if (command == "V") beepHorn();
    else if (command == "W") lightOn();
    else if (command == "w") lightOff();
  }
  server.send(200, "text/html", UI_HTML);
}

void setupPins() {
  pinMode(buzPin, OUTPUT);  digitalWrite(buzPin, LOW);
  pinMode(headLed, OUTPUT); digitalWrite(headLed, LOW);
  pinMode(wifiLed, OUTPUT); digitalWrite(wifiLed, HIGH); // turned ON after AP starts (active LOW on many boards)

  pinMode(enA, OUTPUT); digitalWrite(enA, HIGH); // full speed (set PWM if needed)
  pinMode(enB, OUTPUT); digitalWrite(enB, HIGH);
  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  stopAll();
}

void setupAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP SSID: "); Serial.println(AP_SSID);
  Serial.print("AP PASS: "); Serial.println("(hidden)");
  Serial.print("AP IP: ");   Serial.println(ip.toString());
  digitalWrite(wifiLed, LOW); // WiFi LED ON
}

void setupServer() {
  server.on("/", handleRoot);
  server.onNotFound(handleRoot);
  server.begin();
}

void setupOTA() {
#ifdef OTA_ENABLED
  ArduinoOTA.setHostname(AP_HOSTNAME);
  ArduinoOTA.begin();
#endif
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n* ESP8266 WiFi Car - AP Mode *");
  setupPins();
  setupAP();
  setupServer();
  setupOTA();
}

void loop() {
#ifdef OTA_ENABLED
  ArduinoOTA.handle();
#endif
  server.handleClient();
}
