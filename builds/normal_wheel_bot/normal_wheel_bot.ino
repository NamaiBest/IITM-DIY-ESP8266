#define ENA 5    // GPIO5 (D1)
#define ENB 12   // GPIO12 (D6)
#define IN_1 4   // GPIO4 (D2)
#define IN_2 0   // GPIO0 (D3)
#define IN_3 2   // GPIO2 (D4)
#define IN_4 14  // GPIO14 (D5)

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "IITM-DIY-Normal";
const char* password = "12345678";

ESP8266WebServer server(80);

int speedLevel = 5;
int speedCar = 750;

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>IITM DIY School Connect - Normal DIY</title>
<style>
:root {
  --ink:#162033;
  --muted:#5f6f85;
  --panel:#ffffff;
  --edge:#d8e4f3;
  --brand:#0f766e;
  --brand-2:#f97316;
}
* { box-sizing: border-box; }
body {
  margin: 0;
  min-height: 100vh;
  display: grid;
  place-items: center;
  padding: 16px;
  color: var(--ink);
  background:
    radial-gradient(circle at 10% 5%, #ffffff 0%, #eef4fb 42%, #e5edf8 100%);
  font-family: "Trebuchet MS", "Segoe UI", Verdana, sans-serif;
}
.card {
  width: min(460px, 100%);
  background: var(--panel);
  border: 1px solid var(--edge);
  border-radius: 22px;
  padding: 18px;
  box-shadow: 0 18px 40px rgba(20, 32, 51, 0.16);
}
h1 {
  margin: 0;
  font-size: 1.25rem;
  text-align: center;
}
.sub {
  margin: 6px 0 18px;
  text-align: center;
  color: var(--muted);
  font-size: 0.95rem;
}
.grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 10px;
}
.btn {
  border: none;
  border-radius: 14px;
  padding: 14px 8px;
  font-size: 0.95rem;
  font-weight: 700;
  color: #fff;
  background: linear-gradient(135deg, var(--brand) 0%, #0a9487 100%);
  box-shadow: 0 7px 16px rgba(15, 118, 110, 0.3);
  touch-action: none;
}
.btn:active {
  transform: translateY(1px) scale(0.98);
}
.btn.stop {
  background: linear-gradient(135deg, #d9480f 0%, var(--brand-2) 100%);
  box-shadow: 0 7px 16px rgba(249, 115, 22, 0.3);
}
.fwd { grid-column: 2; }
.left { grid-column: 1; }
.stop { grid-column: 2; }
.right { grid-column: 3; }
.back { grid-column: 2; }
.panel {
  margin-top: 14px;
  border: 1px solid var(--edge);
  border-radius: 14px;
  padding: 12px;
  background: #f9fbfe;
}
.line {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
  margin-bottom: 10px;
}
.badge {
  min-width: 44px;
  text-align: center;
  padding: 4px 8px;
  border-radius: 999px;
  color: #fff;
  font-weight: 700;
  background: linear-gradient(135deg, #1d4ed8 0%, #0369a1 100%);
}
input[type="range"] {
  width: 100%;
}
.status {
  margin-top: 12px;
  border-radius: 10px;
  padding: 10px;
  text-align: center;
  color: var(--muted);
  background: #eef3fa;
  font-size: 0.92rem;
}
.footer {
  margin-top: 12px;
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
  <p class="sub">ESP8266 Bot - Normal DIY Web Control</p>

  <section class="grid">
    <button class="btn fwd" data-cmd="F">Forward</button>
    <button class="btn left" data-cmd="L">Left</button>
    <button class="btn stop" id="stopBtn">Stop</button>
    <button class="btn right" data-cmd="R">Right</button>
    <button class="btn back" data-cmd="B">Backward</button>
  </section>

  <section class="panel">
    <div class="line">
      <strong>Speed</strong>
      <span class="badge" id="speedBadge">5</span>
    </div>
    <input type="range" id="speedSlider" min="0" max="9" value="5">
    <div class="status" id="status">Ready</div>
  </section>

  <div class="footer">Made by <a href="https://www.youtube.com/@-MRFUN" target="_blank">Namai</a></div>
</main>

<script>
const statusEl = document.getElementById('status');
const speedEl = document.getElementById('speedBadge');

function send(path) {
  fetch(path, { cache: 'no-store' }).catch(() => {
    statusEl.textContent = 'Connection issue. Reconnect WiFi and retry.';
  });
}

function moveText(cmd) {
  if (cmd === 'F') return 'Moving forward';
  if (cmd === 'B') return 'Moving backward';
  if (cmd === 'L') return 'Turning left';
  if (cmd === 'R') return 'Turning right';
  return 'Stopped';
}

function bindHold(button) {
  const cmd = button.dataset.cmd;
  const start = (ev) => {
    ev.preventDefault();
    send('/cmd?move=' + cmd);
    statusEl.textContent = moveText(cmd);
  };
  const end = (ev) => {
    ev.preventDefault();
    send('/cmd?move=S');
    statusEl.textContent = 'Stopped';
  };

  button.addEventListener('pointerdown', start);
  button.addEventListener('pointerup', end);
  button.addEventListener('pointerleave', end);
  button.addEventListener('pointercancel', end);
}

document.querySelectorAll('[data-cmd]').forEach(bindHold);
document.getElementById('stopBtn').addEventListener('click', () => {
  send('/cmd?move=S');
  statusEl.textContent = 'Stopped';
});

document.getElementById('speedSlider').addEventListener('input', (ev) => {
  const value = ev.target.value;
  speedEl.textContent = value;
  send('/cmd?speed=' + value);
  statusEl.textContent = 'Speed level set to ' + value;
});
</script>
</body>
</html>
)rawliteral";

int speedFromLevel(int level) {
  switch (level) {
    case 0: return 400;
    case 1: return 470;
    case 2: return 540;
    case 3: return 610;
    case 4: return 680;
    case 5: return 750;
    case 6: return 820;
    case 7: return 890;
    case 8: return 960;
    case 9: return 1023;
    default: return 750;
  }
}

void applySpeed() {
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
}

void stopRobot() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  applySpeed();
}

void goAhead() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  applySpeed();
}

void goBack() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  applySpeed();
}

void goRight() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  applySpeed();
}

void goLeft() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  applySpeed();
}

void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleCommand() {
  if (server.hasArg("speed")) {
    speedLevel = constrain(server.arg("speed").toInt(), 0, 9);
    speedCar = speedFromLevel(speedLevel);
  }

  if (server.hasArg("move")) {
    String move = server.arg("move");
    if (move == "F") goAhead();
    else if (move == "B") goBack();
    else if (move == "L") goLeft();
    else if (move == "R") goRight();
    else stopRobot();
  }

  server.send(200, "text/plain", "OK");
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  analogWriteRange(1023);
  speedCar = speedFromLevel(speedLevel);
  stopRobot();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/cmd", HTTP_GET, handleCommand);
  server.begin();
}

void loop() {
  server.handleClient();
}
