#define ENA   5          // Enable/speed motors Right       GPIO5(D1)
#define ENB   12          // Enable/speed motors Left       GPIO12(D6)
#define IN_1  4         // L298N in1 motors Rightx          GPIO4(D2)
#define IN_2  0          // L298N in2 motors Right          GPIO0(D3)
#define IN_3  2           // L298N in3 motors Left          GPIO2(D4)
#define IN_4  14           // L298N in4 motors Left         GPIO14(D5)

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;             //String to store app command state.
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;

const char* ssid = "Namai's Robo"; // CUSTOMIZE YOUR BOT NAME HERE  
ESP8266WebServer server(80);

void setup() {
 
 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
  
  Serial.begin(115200);
  
// Connecting WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}

void goAhead(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goBack(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goRight(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goLeft(){

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void stopRobot(){  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
 }

void loop() {
    server.handleClient();
    
      command = server.arg("State");
      if (command == "F") goAhead();
      else if (command == "B") goBack();
      else if (command == "L") goLeft();
      else if (command == "R") goRight();
      else if (command == "0") speedCar = 400;
      else if (command == "1") speedCar = 470;
      else if (command == "2") speedCar = 540;
      else if (command == "3") speedCar = 610;
      else if (command == "4") speedCar = 680;
      else if (command == "5") speedCar = 750;
      else if (command == "6") speedCar = 820;
      else if (command == "7") speedCar = 890;
      else if (command == "8") speedCar = 960;
      else if (command == "9") speedCar = 1023;
      else if (command == "S") stopRobot();
}

void HTTP_handleRoot(void) {
  if( server.hasArg("State") ){
    Serial.println(server.arg("State"));
    server.send ( 200, "text/html", "" );
  } else {
    server.send ( 200, "text/html", getHTML() );
  }
  delay(1);
}

String getHTML() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='en'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>";
  html += "<title>IITM DIY Bot Control</title>";
  html += "<style>";
  html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
  html += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; display: flex; justify-content: center; align-items: center; padding: 20px; }";
  html += ".container { background: white; border-radius: 20px; padding: 30px; max-width: 400px; width: 100%; box-shadow: 0 20px 60px rgba(0,0,0,0.3); }";
  html += "h1 { text-align: center; color: #333; margin-bottom: 10px; font-size: 24px; }";
  html += ".subtitle { text-align: center; color: #666; font-size: 14px; margin-bottom: 30px; }";
  html += ".controls { display: grid; grid-template-columns: repeat(3, 1fr); gap: 15px; margin-bottom: 30px; }";
  html += ".btn { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; border: none; border-radius: 15px; padding: 20px; font-size: 18px; font-weight: bold; cursor: pointer; transition: all 0.3s; box-shadow: 0 4px 15px rgba(102, 126, 234, 0.4); touch-action: manipulation; }";
  html += ".btn:active { transform: scale(0.95); box-shadow: 0 2px 8px rgba(102, 126, 234, 0.4); }";
  html += ".btn:disabled { opacity: 0.3; cursor: not-allowed; }";
  html += ".btn-forward { grid-column: 2; background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); box-shadow: 0 4px 15px rgba(56, 239, 125, 0.4); }";
  html += ".btn-left { grid-column: 1; grid-row: 2; }";
  html += ".btn-stop { grid-column: 2; grid-row: 2; background: linear-gradient(135deg, #ee0979 0%, #ff6a00 100%); box-shadow: 0 4px 15px rgba(238, 9, 121, 0.4); }";
  html += ".btn-right { grid-column: 3; grid-row: 2; }";
  html += ".btn-backward { grid-column: 2; grid-row: 3; background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%); box-shadow: 0 4px 15px rgba(245, 87, 108, 0.4); }";
  html += ".speed-control { margin-top: 20px; }";
  html += ".speed-label { display: flex; justify-content: space-between; align-items: center; margin-bottom: 10px; color: #333; font-weight: 600; }";
  html += ".speed-value { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 5px 15px; border-radius: 20px; font-size: 14px; }";
  html += ".slider { width: 100%; height: 8px; border-radius: 5px; background: #ddd; outline: none; -webkit-appearance: none; }";
  html += ".slider::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 24px; height: 24px; border-radius: 50%; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); cursor: pointer; box-shadow: 0 2px 8px rgba(102, 126, 234, 0.5); }";
  html += ".slider::-moz-range-thumb { width: 24px; height: 24px; border-radius: 50%; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); cursor: pointer; border: none; box-shadow: 0 2px 8px rgba(102, 126, 234, 0.5); }";
  html += ".status { text-align: center; margin-top: 20px; padding: 10px; background: #f5f5f5; border-radius: 10px; color: #666; font-size: 14px; }";
  html += ".footer { text-align: center; margin-top: 20px; padding: 15px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); border-radius: 10px; color: white; font-size: 12px; }";
  html += ".footer a { color: white; text-decoration: none; font-weight: bold; border-bottom: 1px solid white; }";
  html += ".footer a:hover { opacity: 0.8; }";
  html += "@media (max-width: 480px) { .container { padding: 20px; } h1 { font-size: 20px; } .btn { padding: 18px; font-size: 16px; } }";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<h1>🤖 IITM DIY School Connect</h1>";
  html += "<div class='subtitle'>ESP8266 Bot - Web Control Panel</div>";
  html += "<div class='controls'>";
  html += "<button class='btn btn-forward' ontouchstart='sendCommand(\"F\")' ontouchend='sendCommand(\"S\")' onmousedown='sendCommand(\"F\")' onmouseup='sendCommand(\"S\")'>▲<br>Forward</button>";
  html += "<button class='btn btn-left' ontouchstart='sendCommand(\"L\")' ontouchend='sendCommand(\"S\")' onmousedown='sendCommand(\"L\")' onmouseup='sendCommand(\"S\")'>◄<br>Left</button>";
  html += "<button class='btn btn-stop' ontouchstart='sendCommand(\"S\")' onmousedown='sendCommand(\"S\")'>■<br>Stop</button>";
  html += "<button class='btn btn-right' ontouchstart='sendCommand(\"R\")' ontouchend='sendCommand(\"S\")' onmousedown='sendCommand(\"R\")' onmouseup='sendCommand(\"S\")'>►<br>Right</button>";
  html += "<button class='btn btn-backward' ontouchstart='sendCommand(\"B\")' ontouchend='sendCommand(\"S\")' onmousedown='sendCommand(\"B\")' onmouseup='sendCommand(\"S\")'>▼<br>Backward</button>";
  html += "</div>";
  html += "<div class='speed-control'>";
  html += "<div class='speed-label'><span>Speed Control</span><span class='speed-value' id='speedValue'>5</span></div>";
  html += "<div class='footer'>Made by <a href='https://www.youtube.com/@-MRFUN' target='_blank'>Namai</a> 🚀<br>Visit for more projects & inspiration!</div>";
  html += "<input type='range' min='0' max='9' value='5' class='slider' id='speedSlider' oninput='updateSpeed(this.value)'>";
  html += "</div>";
  html += "<div class='status' id='status'>Ready to control</div>";
  html += "</div>";
  html += "<script>";
  html += "function sendCommand(cmd) {";
  html += "  fetch('/?State=' + cmd).then(() => {";
  html += "    let status = document.getElementById('status');";
  html += "    if(cmd === 'F') status.textContent = '⬆️ Moving Forward';";
  html += "    else if(cmd === 'B') status.textContent = '⬇️ Moving Backward';";
  html += "    else if(cmd === 'L') status.textContent = '⬅️ Turning Left';";
  html += "    else if(cmd === 'R') status.textContent = '➡️ Turning Right';";
  html += "    else if(cmd === 'S') status.textContent = '⏹️ Stopped';";
  html += "  });";
  html += "}";
  html += "function updateSpeed(val) {";
  html += "  document.getElementById('speedValue').textContent = val;";
  html += "  fetch('/?State=' + val).then(() => {";
  html += "    document.getElementById('status').textContent = '⚡ Speed updated to ' + val;";
  html += "  });";
  html += "}";
  html += "</script>";
  html += "</body>";
  html += "</html>";
  return html;
}
