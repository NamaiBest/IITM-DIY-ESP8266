#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "controller_page.h"

const char* ssid = "IITM-DIY-Mecanum-UI";
const char* password = "12345678";

ESP8266WebServer server(80);

// Require periodic movement commands while a direction is held.
constexpr unsigned long MOVE_TIMEOUT_MS = 850;
unsigned long lastMoveAt = 0;
bool moving = false;

// ESP8266 pin map for dual L298N wiring.
#define IN1 5    // GPIO5  (D1) - Front Left  IN1
#define IN2 4    // GPIO4  (D2) - Front Left  IN2
#define IN3 14   // GPIO14 (D5) - Front Right IN1
#define IN4 12   // GPIO12 (D6) - Front Right IN2
#define IN5 13   // GPIO13 (D7) - Back Left   IN1
#define IN6 16   // GPIO16 (D0) - Back Left   IN2
#define IN7 3    // GPIO3  (RX) - Back Right  IN1
#define IN8 1    // GPIO1  (TX) - Back Right  IN2

void handleRoot();
void handleCommand();
void stopCar();
void forward();
void backward();
void strafeRight();
void strafeLeft();
void rotateRight();
void rotateLeft();
void forwardLeft();
void forwardRight();
void backLeft();
void backRight();

void handleRoot() {
  server.sendHeader("Cache-Control", "no-store");
  server.send_P(200, "text/html; charset=utf-8", INDEX_HTML);
}

void handleCommand() {
  String move = server.arg("move");

  if (move == "F") forward();
  else if (move == "B") backward();
  else if (move == "SL") strafeLeft();
  else if (move == "SR") strafeRight();
  else if (move == "RL") rotateLeft();
  else if (move == "RR") rotateRight();
  else if (move == "FL") forwardLeft();
  else if (move == "FR") forwardRight();
  else if (move == "BL") backLeft();
  else if (move == "BR") backRight();
  else {
    stopCar();
    server.send(200, "text/plain", "OK");
    return;
  }

  moving = true;
  lastMoveAt = millis();
  server.send(200, "text/plain", "OK");
}

void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT); pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT); pinMode(IN8, OUTPUT);

  stopCar();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/cmd", HTTP_GET, handleCommand);
  server.begin();
}

void loop() {
  server.handleClient();
  if (moving && millis() - lastMoveAt >= MOVE_TIMEOUT_MS) {
    stopCar();
  }
}

void stopCar() {
  moving = false;
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW); digitalWrite(IN8, LOW);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
}

void strafeRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW);  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
}

void strafeLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);  digitalWrite(IN8, HIGH);
}

void rotateRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);  digitalWrite(IN8, HIGH);
}

void rotateLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
}

void forwardLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);  digitalWrite(IN8, LOW);
}

void forwardRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);  digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
}

void backLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);  digitalWrite(IN8, HIGH);
}

void backRight() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW);  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW);  digitalWrite(IN8, LOW);
}
