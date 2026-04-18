#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "IITM-DIY-Mecanum-2D";
const char* password = "12345678";

ESP8266WebServer server(80);

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

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>IITM DIY School Connect - Fully Functional Mecanum</title>
<style>
:root {
  --ink:#172233;
  --muted:#5f6f86;
  --panel:#ffffff;
  --edge:#d9e6f5;
  --brand:#0b7285;
  --brand-2:#f97316;
}
* { box-sizing: border-box; }
body {
  margin: 0;
  min-height: 100vh;
  display: grid;
  place-items: center;
  padding: 14px;
  color: var(--ink);
  background:
    radial-gradient(circle at 0% 0%, #ffffff 0%, #edf4fc 45%, #e4edf8 100%);
  font-family: "Trebuchet MS", "Segoe UI", Verdana, sans-serif;
}
.card {
  width: min(520px, 100%);
  background: var(--panel);
  border: 1px solid var(--edge);
  border-radius: 22px;
  padding: 16px;
  box-shadow: 0 18px 40px rgba(23, 34, 52, 0.16);
}
h1 {
  margin: 0;
  font-size: 1.2rem;
  text-align: center;
}
.sub {
  margin: 6px 0 14px;
  text-align: center;
  color: var(--muted);
  font-size: 0.92rem;
}
.grid3 {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 10px;
  margin-bottom: 10px;
}
.grid2 {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 10px;
  margin-bottom: 10px;
}
.btn {
  border: none;
  border-radius: 13px;
  min-height: 52px;
  padding: 8px;
  font-size: 0.9rem;
  font-weight: 700;
  color: #fff;
  background: linear-gradient(140deg, var(--brand) 0%, #0f8d98 100%);
  box-shadow: 0 7px 16px rgba(11, 114, 133, 0.32);
  touch-action: none;
}
.btn:active {
  transform: translateY(1px) scale(0.98);
}
.stop {
  width: 100%;
  margin-top: 6px;
  background: linear-gradient(140deg, #c2410c 0%, var(--brand-2) 100%);
  box-shadow: 0 7px 16px rgba(249, 115, 22, 0.34);
}
.status {
  margin-top: 12px;
  border-radius: 10px;
  padding: 10px;
  background: #eef3fa;
  color: var(--muted);
  text-align: center;
  font-size: 0.9rem;
}
.footer {
  margin-top: 10px;
  text-align: center;
  font-size: 0.8rem;
  color: var(--muted);
}
.footer a {
  color: #0b5fd7;
  text-decoration: none;
  font-weight: 700;
}
</style>
</head>
<body>
<main class="card">
  <h1>IITM DIY School Connect</h1>
  <p class="sub">ESP8266 Bot - Fully Functional Dual Driver Mecanum</p>

  <section class="grid3">
    <button class="btn" data-cmd="FL">F-L</button>
    <button class="btn" data-cmd="F">Forward</button>
    <button class="btn" data-cmd="FR">F-R</button>
  </section>
  <section class="grid2">
    <button class="btn" data-cmd="SL">Strafe Left</button>
    <button class="btn" data-cmd="SR">Strafe Right</button>
  </section>
  <section class="grid3">
    <button class="btn" data-cmd="BL">B-L</button>
    <button class="btn" data-cmd="B">Backward</button>
    <button class="btn" data-cmd="BR">B-R</button>
  </section>
  <section class="grid2">
    <button class="btn" data-cmd="RL">Rotate Left</button>
    <button class="btn" data-cmd="RR">Rotate Right</button>
  </section>

  <button class="btn stop" id="stopBtn">STOP</button>
  <div class="status" id="status">Ready</div>
  <div class="footer">Made by <a href="https://www.youtube.com/@-MRFUN" target="_blank">Namai</a></div>
</main>

<script>
const statusEl = document.getElementById('status');

const labelMap = {
  F: 'Forward',
  B: 'Backward',
  SL: 'Strafing Left',
  SR: 'Strafing Right',
  RL: 'Rotating Left',
  RR: 'Rotating Right',
  FL: 'Forward Left',
  FR: 'Forward Right',
  BL: 'Backward Left',
  BR: 'Backward Right',
  S: 'Stopped'
};

function sendCmd(cmd) {
  fetch('/cmd?move=' + encodeURIComponent(cmd), { cache: 'no-store' }).catch(() => {
    statusEl.textContent = 'Connection issue. Reconnect WiFi and retry.';
  });
}

function bindHold(btn) {
  const cmd = btn.dataset.cmd;
  const down = (ev) => {
    ev.preventDefault();
    sendCmd(cmd);
    statusEl.textContent = labelMap[cmd] || 'Running';
  };
  const up = (ev) => {
    ev.preventDefault();
    sendCmd('S');
    statusEl.textContent = labelMap.S;
  };

  btn.addEventListener('pointerdown', down);
  btn.addEventListener('pointerup', up);
  btn.addEventListener('pointerleave', up);
  btn.addEventListener('pointercancel', up);
}

document.querySelectorAll('[data-cmd]').forEach(bindHold);
document.getElementById('stopBtn').addEventListener('click', () => {
  sendCmd('S');
  statusEl.textContent = labelMap.S;
});
</script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
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
  else stopCar();

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
}

void stopCar() {
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
