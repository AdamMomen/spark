#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

// LED pin definitions
const int RED_LED_PIN = D2;
const int GREEN_LED_PIN = D3;
const int POT_PIN = A0;

// LED states
bool redLEDState = false;
bool greenLEDState = false;

// Access Point settings
const char *AP_SSID = "IoT-Workshop";
const char *AP_PASSWORD = ""; // No password

// Web server and WebSocket
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Team information
const char *TEAM_NAME = "Team A";
const char *MEMBER_1 = "Alice";
const char *MEMBER_2 = "Bob";

void setupWiFiAP() {
  Serial.println("Creating Access Point...");

  // Set WiFi mode to Access Point
  WiFi.mode(WIFI_AP);

  // Create Access Point with custom name (no password)
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.println("Access Point created!");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Connect to: " + String(AP_SSID));
  Serial.println("No password required!");
  Serial.println("Dashboard: http://192.168.4.1");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload,
                    size_t length) {
  switch (type) {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;

  case WStype_CONNECTED:
    Serial.printf("[%u] Connected from %s\n", num, payload);
    break;

  case WStype_TEXT:
    // Handle incoming messages if needed
    Serial.printf("[%u] Received: %s\n", num, payload);
    break;
  }
}

void sendPotData() {
  int potValue = analogRead(POT_PIN);
  float voltage = (potValue / 1023.0) * 3.3;

  String json = "{\"pot\": " + String(potValue) +
                ", \"voltage\": " + String(voltage, 2) + "}";
  webSocket.broadcastTXT(json);

  // Print to serial for debugging
  Serial.printf("Potentiometer: %d (%.2fV)\n", potValue, voltage);
}

void setupWebServer() {
  // Root page - Modern IoT Control Panel
  server.on("/", []() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<title>IoT Control Panel</title>";
    html +=
        "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<link "
            "href='https://fonts.googleapis.com/"
            "css2?family=Inter:wght@300;400;500;600;700&display=swap' "
            "rel='stylesheet'>";
    html += "<style>";
    html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
    html += "body { font-family: 'Inter', -apple-system, BlinkMacSystemFont, "
            "sans-serif; ";
    html += "background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); ";
    html +=
        "min-height: 100vh; padding: 20px; transition: background 0.5s ease; ";
    html += "overflow-x: hidden; }";
    html += ".container { max-width: 1200px; margin: 0 auto; }";
    html += ".header { text-align: center; margin-bottom: 40px; }";
    html += ".header h1 { color: white; font-size: clamp(2rem, 5vw, 3.5rem); ";
    html += "font-weight: 700; margin-bottom: 10px; text-shadow: 0 2px 10px "
            "rgba(0,0,0,0.3); ";
    html += "letter-spacing: -0.02em; }";
    html += ".header p { color: rgba(255,255,255,0.8); font-size: 1.1rem; ";
    html += "font-weight: 300; }";
    html += ".connection-status { display: inline-flex; align-items: center; ";
    html +=
        "gap: 8px; padding: 8px 16px; border-radius: 20px; font-size: 0.9rem; ";
    html +=
        "font-weight: 500; margin-bottom: 30px; backdrop-filter: blur(10px); ";
    html += "border: 1px solid rgba(255,255,255,0.2); }";
    html += ".connected { background: rgba(76, 175, 80, 0.2); color: #4CAF50; ";
    html += "border-color: rgba(76, 175, 80, 0.3); }";
    html +=
        ".disconnected { background: rgba(244, 67, 54, 0.2); color: #f44336; ";
    html += "border-color: rgba(244, 67, 54, 0.3); }";
    html += ".status-dot { width: 8px; height: 8px; border-radius: 50%; ";
    html += "background: currentColor; animation: pulse 2s infinite; }";
    html +=
        "@keyframes pulse { 0%, 100% { opacity: 1; } 50% { opacity: 0.5; } }";
    html += ".main-grid { display: grid; grid-template-columns: "
            "repeat(auto-fit, minmax(300px, 1fr)); ";
    html += "gap: 30px; margin-bottom: 40px; }";
    html += ".card { background: rgba(255, 255, 255, 0.1); backdrop-filter: "
            "blur(20px); ";
    html += "border-radius: 20px; padding: 30px; border: 1px solid rgba(255, "
            "255, 255, 0.2); ";
    html += "box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1); transition: all 0.3s "
            "ease; }";
    html += ".card:hover { transform: translateY(-5px); box-shadow: 0 15px "
            "40px rgba(0, 0, 0, 0.2); }";
    html += ".card h2 { color: white; font-size: 1.5rem; font-weight: 600; ";
    html += "margin-bottom: 20px; text-align: center; }";
    html += ".potentiometer-display { text-align: center; }";
    html += ".pot-value { font-size: 3rem; font-weight: 700; color: white; ";
    html += "margin-bottom: 10px; text-shadow: 0 2px 10px rgba(0,0,0,0.3); ";
    html += "transition: all 0.3s ease; }";
    html += ".pot-label { color: rgba(255,255,255,0.7); font-size: 1rem; ";
    html += "font-weight: 400; margin-bottom: 20px; }";
    html += ".pot-visual { width: 200px; height: 200px; margin: 0 auto 20px; ";
    html += "position: relative; }";
    html += ".pot-circle { width: 100%; height: 100%; border-radius: 50%; ";
    html += "background: conic-gradient(from 0deg, #ff6b6b, #4ecdc4, #45b7d1, "
            "#96ceb4, #feca57, #ff6b6b); ";
    html += "position: relative; }";
    html += ".pot-inner { position: absolute; top: 20px; left: 20px; right: "
            "20px; bottom: 20px; ";
    html += "background: rgba(255,255,255,0.1); border-radius: 50%; ";
    html += "backdrop-filter: blur(10px); display: flex; align-items: center; ";
    html += "justify-content: center; }";
    html += ".pot-needle { width: 2px; height: 80px; background: white; ";
    html += "position: absolute; top: 20px; left: 50%; transform-origin: "
            "bottom center; ";
    html += "border-radius: 2px; box-shadow: 0 0 10px rgba(255,255,255,0.5); ";
    html += "transition: transform 0.3s ease; }";
    html +=
        ".led-controls { display: flex; flex-direction: column; gap: 20px; }";
    html +=
        ".led-button { padding: 15px 30px; border: none; border-radius: 15px; ";
    html += "font-size: 1.1rem; font-weight: 600; cursor: pointer; transition: "
            "all 0.3s ease; ";
    html += "position: relative; overflow: hidden; min-width: 200px; }";
    html += ".led-button::before { content: ''; position: absolute; top: 0; "
            "left: -100%; ";
    html += "width: 100%; height: 100%; background: linear-gradient(90deg, "
            "transparent, rgba(255,255,255,0.2), transparent); ";
    html += "transition: left 0.5s; }";
    html += ".led-button:hover::before { left: 100%; }";
    html += ".led-button:hover { transform: translateY(-2px); box-shadow: 0 "
            "10px 25px rgba(0,0,0,0.2); }";
    html += ".led-button:active { transform: translateY(0); }";
    html +=
        ".red-btn { background: linear-gradient(135deg, #ff6b6b, #ee5a52); ";
    html += "color: white; box-shadow: 0 5px 15px rgba(255, 107, 107, 0.4); }";
    html +=
        ".green-btn { background: linear-gradient(135deg, #51cf66, #40c057); ";
    html += "color: white; box-shadow: 0 5px 15px rgba(81, 207, 102, 0.4); }";
    html += ".led-status { display: flex; justify-content: space-around; "
            "margin-top: 20px; }";
    html += ".led-indicator { text-align: center; padding: 15px; "
            "border-radius: 15px; ";
    html += "backdrop-filter: blur(10px); border: 1px solid "
            "rgba(255,255,255,0.2); ";
    html += "transition: all 0.3s ease; }";
    html += ".led-indicator.on { background: rgba(81, 207, 102, 0.2); ";
    html += "border-color: rgba(81, 207, 102, 0.4); }";
    html += ".led-indicator.off { background: rgba(255, 107, 107, 0.2); ";
    html += "border-color: rgba(255, 107, 107, 0.4); }";
    html += ".led-indicator h4 { color: white; font-size: 1rem; margin-bottom: "
            "5px; }";
    html += ".led-indicator .state { font-size: 0.9rem; font-weight: 500; }";
    html += ".led-indicator.on .state { color: #51cf66; }";
    html += ".led-indicator.off .state { color: #ff6b6b; }";
    html += "@media (max-width: 768px) { ";
    html += "  .main-grid { grid-template-columns: 1fr; gap: 20px; }";
    html += "  .card { padding: 20px; }";
    html += "  .pot-visual { width: 150px; height: 150px; }";
    html += "  .pot-value { font-size: 2.5rem; }";
    html += "  .led-button { min-width: 150px; padding: 12px 25px; }";
    html += "  .led-status { flex-direction: column; gap: 10px; }";
    html += "}";
    html += "</style></head><body>";
    html += "<div class='container'>";
    html += "<div class='header'>";
    html += "<h1>IoT Control Panel</h1>";
    html += "<p>Real-time sensor control and monitoring</p>";
    html += "</div>";
    html += "<div class='connection-status' id='ws-status'>";
    html += "<div class='status-dot'></div>";
    html += "<span>Connecting...</span>";
    html += "</div>";
    html += "<div class='main-grid'>";
    html += "<div class='card'>";
    html += "<h2>Potentiometer</h2>";
    html += "<div class='potentiometer-display'>";
    html += "<div class='pot-visual'>";
    html += "<div class='pot-circle'>";
    html += "<div class='pot-inner'>";
    html += "<div class='pot-needle' id='pot-needle'></div>";
    html += "</div>";
    html += "</div>";
    html += "</div>";
    html += "<div class='pot-label'>Current Value</div>";
    html += "<div class='pot-value' id='potValue'>0</div>";
    html += "</div>";
    html += "</div>";
    html += "<div class='card'>";
    html += "<h2>LED Controls</h2>";
    html += "<div class='led-controls'>";
    html += "<button class='led-button red-btn' onclick='toggleRedLED()'>";
    html += "Toggle Red LED";
    html += "</button>";
    html += "<button class='led-button green-btn' onclick='toggleGreenLED()'>";
    html += "Toggle Green LED";
    html += "</button>";
    html += "<div class='led-status'>";
    html += "<div class='led-indicator off' id='red-led-status'>";
    html += "<h4>Red LED</h4>";
    html += "<div class='state'>OFF</div>";
    html += "</div>";
    html += "<div class='led-indicator off' id='green-led-status'>";
    html += "<h4>Green LED</h4>";
    html += "<div class='state'>OFF</div>";
    html += "</div>";
    html += "</div>";
    html += "</div>";
    html += "</div>";
    html += "</div>";
    html += "</div>";
    html += "<script>";
    html += "let ws;";
    html += "let lastUpdateTime = new Date();";
    html += "function connectWebSocket() {";
    html += "  ws = new WebSocket('ws://' + location.hostname + ':81/');";
    html += "  ws.onopen = function() {";
    html += "    console.log('WebSocket connected');";
    html += "    const statusEl = document.getElementById('ws-status');";
    html += "    statusEl.innerHTML = '<div "
            "class=\"status-dot\"></div><span>Connected</span>';";
    html += "    statusEl.className = 'connection-status connected';";
    html += "  };";
    html += "  ws.onmessage = function(event) {";
    html += "    const data = JSON.parse(event.data);";
    html += "    updatePotDisplay(data.pot, data.voltage);";
    html += "  };";
    html += "  ws.onclose = function() {";
    html += "    console.log('WebSocket disconnected');";
    html += "    const statusEl = document.getElementById('ws-status');";
    html += "    statusEl.innerHTML = '<div "
            "class=\"status-dot\"></div><span>Disconnected</span>';";
    html += "    statusEl.className = 'connection-status disconnected';";
    html += "    setTimeout(connectWebSocket, 2000);";
    html += "  };";
    html += "  ws.onerror = function(error) {";
    html += "    console.error('WebSocket error:', error);";
    html += "  };";
    html += "}";
    html += "function updatePotDisplay(potValue, voltage) {";
    html += "  document.getElementById('potValue').textContent = potValue;";
    html += "  const angle = (potValue / 1023.0) * 270 - 135;";
    html += "  document.getElementById('pot-needle').style.transform = "
            "'translateX(-50%) rotate(' + angle + 'deg)';";
    html += "  const hue = (potValue / 1023.0) * 240;";
    html +=
        "  document.body.style.background = 'linear-gradient(135deg, hsl(' + "
        "hue + ', 70%, 50%) 0%, hsl(' + (hue + 60) + ', 70%, 40%) 100%)';";
    html += "}";
    html += "function toggleRedLED() {";
    html += "  fetch('/api/led/red/toggle', {method: 'POST'})";
    html += "    .then(response => response.json())";
    html += "    .then(data => updateLEDStatus());";
    html += "}";
    html += "function toggleGreenLED() {";
    html += "  fetch('/api/led/green/toggle', {method: 'POST'})";
    html += "    .then(response => response.json())";
    html += "    .then(data => updateLEDStatus());";
    html += "}";
    html += "function updateLEDStatus() {";
    html += "  fetch('/api/status')";
    html += "    .then(response => response.json())";
    html += "    .then(data => {";
    html += "      const redLed = document.getElementById('red-led-status');";
    html +=
        "      const greenLed = document.getElementById('green-led-status');";
    html += "      if (data.leds['1']) {";
    html += "        redLed.className = 'led-indicator on';";
    html += "        redLed.querySelector('.state').textContent = 'ON';";
    html += "      } else {";
    html += "        redLed.className = 'led-indicator off';";
    html += "        redLed.querySelector('.state').textContent = 'OFF';";
    html += "      }";
    html += "      if (data.leds['2']) {";
    html += "        greenLed.className = 'led-indicator on';";
    html += "        greenLed.querySelector('.state').textContent = 'ON';";
    html += "      } else {";
    html += "        greenLed.className = 'led-indicator off';";
    html += "        greenLed.querySelector('.state').textContent = 'OFF';";
    html += "      }";
    html += "    });";
    html += "}";
    html += "connectWebSocket();";
    html += "setInterval(updateLEDStatus, 2000);";
    html += "updateLEDStatus();";
    html += "</script></body></html>";

    server.send(200, "text/html", html);
  });

  // API endpoints
  server.on("/api/status", []() {
    String json = "{";
    json += "\"wifi_connected\":" +
            String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
    json += "\"uptime\":" + String(millis() / 1000) + ",";
    json += "\"free_heap\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"leds\":{";
    json += "\"1\":" + String(redLEDState ? "true" : "false") + ",";
    json += "\"2\":" + String(greenLEDState ? "true" : "false");
    json += "},";
    json += "\"timestamp\":" + String(millis());
    json += "}";

    server.send(200, "application/json", json);
  });

  server.on("/api/led/red/toggle", HTTP_POST, []() {
    redLEDState = !redLEDState;
    digitalWrite(RED_LED_PIN, redLEDState);
    Serial.printf("Red LED toggled to: %s\n", redLEDState ? "ON" : "OFF");

    String json =
        "{\"led\":\"red\",\"state\":" + String(redLEDState ? "true" : "false") +
        ",\"message\":\"LED toggled successfully\"}";
    server.send(200, "application/json", json);
  });

  server.on("/api/led/green/toggle", HTTP_POST, []() {
    greenLEDState = !greenLEDState;
    digitalWrite(GREEN_LED_PIN, greenLEDState);
    Serial.printf("Green LED toggled to: %s\n", greenLEDState ? "ON" : "OFF");

    String json = "{\"led\":\"green\",\"state\":" +
                  String(greenLEDState ? "true" : "false") +
                  ",\"message\":\"LED toggled successfully\"}";
    server.send(200, "application/json", json);
  });

  server.on("/api/pot/read", []() {
    int potValue = analogRead(POT_PIN);
    float voltage = (potValue / 1023.0) * 3.3;

    String json = "{\"pot\": " + String(potValue) +
                  ", \"voltage\": " + String(voltage, 2) + "}";
    server.send(200, "application/json", json);
  });

  server.begin();
  Serial.println("Web server started");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== IoT Workshop - Potentiometer Control ===");
  Serial.printf("Team: %s\n", TEAM_NAME);
  Serial.printf("Members: %s & %s\n", MEMBER_1, MEMBER_2);
  Serial.println("=============================================");

  // Configure pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);

  // Setup WiFi Access Point, web server, and WebSocket
  setupWiFiAP();
  setupWebServer();

  // Setup WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("Potentiometer Control initialized successfully!");
  Serial.println("Dashboard available at: http://" + WiFi.localIP().toString());
  Serial.println("WebSocket server running on port 81");
  Serial.println("Turn the potentiometer to see real-time updates!");
}

void loop() {
  server.handleClient();
  webSocket.loop();

  // Send potentiometer data every 100ms
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 100) {
    sendPotData();
    lastUpdate = millis();
  }

  delay(10);
}
