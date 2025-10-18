#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

// LED pin definitions
const int RED_LED_PIN = D2;
const int GREEN_LED_PIN = D3;

// LED states
bool redLEDState = false;
bool greenLEDState = false;

// WiFi credentials
const char *WIFI_SSID = "WorkshopWiFi";
const char *WIFI_PASSWORD = "workshop2025";

// Web server
ESP8266WebServer server(80);

// Team information
const char *TEAM_NAME = "Team A";
const char *MEMBER_1 = "Alice";
const char *MEMBER_2 = "Bob";

void setupWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setupWebServer() {
  // Root page - Dashboard template
  server.on("/", []() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<title>Creative IoT Dashboard</title>";
    html +=
        "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>";
    html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
    html +=
        "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; ";
    html += "background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); ";
    html += "min-height: 100vh; padding: 20px; }";
    html += ".container { max-width: 1200px; margin: 0 auto; background: "
            "rgba(255, 255, 255, 0.95); ";
    html += "border-radius: 20px; padding: 30px; box-shadow: 0 20px 40px "
            "rgba(0, 0, 0, 0.1); }";
    html += ".header { text-align: center; margin-bottom: 30px; }";
    html +=
        ".header h1 { color: #333; font-size: 2.5em; margin-bottom: 10px; }";
    html += ".header p { color: #666; font-size: 1.2em; }";
    html += ".status-grid { display: grid; grid-template-columns: "
            "repeat(auto-fit, minmax(250px, 1fr)); ";
    html += "gap: 20px; margin-bottom: 30px; }";
    html += ".status-card { background: white; padding: 20px; border-radius: "
            "15px; ";
    html += "box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1); border-left: 5px solid "
            "#007bff; }";
    html += ".status-card h3 { color: #333; margin-bottom: 10px; font-size: "
            "1.1em; }";
    html += ".status-card .value { font-size: 1.5em; font-weight: bold; color: "
            "#007bff; }";
    html += ".led-control { background: white; padding: 30px; border-radius: "
            "15px; ";
    html += "box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1); margin-bottom: 30px; }";
    html += ".led-control h2 { color: #333; margin-bottom: 20px; text-align: "
            "center; }";
    html += ".led-buttons { display: flex; justify-content: center; gap: 20px; "
            "flex-wrap: wrap; }";
    html +=
        ".led-button { padding: 15px 30px; border: none; border-radius: 10px; ";
    html += "font-size: 1.1em; font-weight: bold; cursor: pointer; transition: "
            "all 0.3s ease; min-width: 150px; }";
    html += ".led-button.red { background: #dc3545; color: white; }";
    html += ".led-button.red:hover { background: #c82333; transform: "
            "translateY(-2px); }";
    html += ".led-button.green { background: #28a745; color: white; }";
    html += ".led-button.green:hover { background: #218838; transform: "
            "translateY(-2px); }";
    html += ".led-button.off { background: #6c757d; color: white; }";
    html += ".led-button.off:hover { background: #5a6268; transform: "
            "translateY(-2px); }";
    html += ".led-status { display: flex; justify-content: center; gap: 40px; "
            "margin-top: 20px; flex-wrap: wrap; }";
    html += ".led-indicator { text-align: center; padding: 15px; "
            "border-radius: 10px; min-width: 120px; }";
    html += ".led-indicator.on { background: #d4edda; color: #155724; border: "
            "2px solid #28a745; }";
    html += ".led-indicator.off { background: #f8d7da; color: #721c24; border: "
            "2px solid #dc3545; }";
    html += ".led-indicator h4 { margin-bottom: 5px; }";
    html += ".footer { text-align: center; color: #666; margin-top: 30px; }";
    html += ".loading { text-align: center; color: #666; font-style: italic; }";
    html += ".error { background: #f8d7da; color: #721c24; padding: 15px; "
            "border-radius: 10px; margin: 20px 0; text-align: center; }";
    html += "@media (max-width: 768px) { .container { padding: 20px; } .header "
            "h1 { font-size: 2em; } ";
    html += ".led-buttons { flex-direction: column; align-items: center; } "
            ".led-status { flex-direction: column; align-items: center; } }";
    html += "</style></head><body>";
    html += "<div class='container'>";
    html += "<div class='header'>";
    html += "<h1>🚀 IoT Workshop Dashboard</h1>";
    html += "<p>Creative LED Control Interface</p>";
    html += "</div>";
    html += "<div class='status-grid'>";
    html += "<div class='status-card'><h3>WiFi Status</h3><div class='value' "
            "id='wifi-status'>Checking...</div></div>";
    html += "<div class='status-card'><h3>Uptime</h3><div class='value' "
            "id='uptime'>0s</div></div>";
    html += "<div class='status-card'><h3>Free Memory</h3><div class='value' "
            "id='memory'>0 bytes</div></div>";
    html += "<div class='status-card'><h3>Last Update</h3><div class='value' "
            "id='last-update'>Never</div></div>";
    html += "</div>";
    html += "<div class='led-control'>";
    html += "<h2>🎛️ LED Control Panel</h2>";
    html += "<div class='led-buttons'>";
    html += "<button class='led-button red' onclick='toggleLED(1)'>Toggle Red "
            "LED</button>";
    html += "<button class='led-button green' onclick='toggleLED(2)'>Toggle "
            "Green LED</button>";
    html += "<button class='led-button off' onclick='turnOffAll()'>Turn All "
            "Off</button>";
    html += "</div>";
    html += "<div class='led-status'>";
    html += "<div class='led-indicator off' id='red-led-status'><h4>🔴 Red "
            "LED</h4><div id='red-led-state'>OFF</div></div>";
    html += "<div class='led-indicator off' id='green-led-status'><h4>🟢 Green "
            "LED</h4><div id='green-led-state'>OFF</div></div>";
    html += "</div>";
    html += "</div>";
    html += "<div class='footer'>";
    html += "<p>Built for IoT Workshop - October 19, 2025</p>";
    html += "<p>Team: " + String(TEAM_NAME) + " - " + String(MEMBER_1) + " & " +
            String(MEMBER_2) + "</p>";
    html += "</div>";
    html += "</div>";
    html += "<script>";
    html += "let lastUpdateTime = new Date();";
    html += "function updateStatus() {";
    html += "  fetch('/api/status')";
    html += "    .then(response => response.json())";
    html += "    .then(data => {";
    html += "      document.getElementById('wifi-status').textContent = "
            "data.wifi_connected ? 'Connected' : 'Disconnected';";
    html += "      document.getElementById('uptime').textContent = data.uptime "
            "+ 's';";
    html += "      document.getElementById('memory').textContent = "
            "data.free_heap + ' bytes';";
    html += "      document.getElementById('last-update').textContent = new "
            "Date().toLocaleTimeString();";
    html += "      updateLEDStatus(1, data.leds['1']);";
    html += "      updateLEDStatus(2, data.leds['2']);";
    html += "    })";
    html += "    .catch(error => {";
    html += "      console.error('Error fetching status:', error);";
    html +=
        "      document.getElementById('wifi-status').textContent = 'Error';";
    html += "    });";
    html += "}";
    html += "function updateLEDStatus(ledNumber, isOn) {";
    html += "  const ledElement = document.getElementById(ledNumber === 1 ? "
            "'red-led-status' : 'green-led-status');";
    html += "  const stateElement = document.getElementById(ledNumber === 1 ? "
            "'red-led-state' : 'green-led-state');";
    html += "  if (isOn) {";
    html += "    ledElement.className = 'led-indicator on';";
    html += "    stateElement.textContent = 'ON';";
    html += "  } else {";
    html += "    ledElement.className = 'led-indicator off';";
    html += "    stateElement.textContent = 'OFF';";
    html += "  }";
    html += "}";
    html += "function toggleLED(ledNumber) {";
    html += "  fetch(`/api/led/${ledNumber}/toggle`, {method: 'POST'})";
    html += "    .then(response => response.json())";
    html += "    .then(data => {";
    html += "      console.log(`LED ${ledNumber} toggled:`, data);";
    html += "      updateStatus();";
    html += "    })";
    html += "    .catch(error => {";
    html += "      console.error('Error toggling LED:', error);";
    html += "    });";
    html += "}";
    html += "function turnOffAll() {";
    html += "  fetch('/api/led/1/state', { method: 'POST', headers: "
            "{'Content-Type': 'application/json'}, body: "
            "JSON.stringify({state: false}) });";
    html += "  fetch('/api/led/2/state', { method: 'POST', headers: "
            "{'Content-Type': 'application/json'}, body: "
            "JSON.stringify({state: false}) });";
    html += "  setTimeout(updateStatus, 100);";
    html += "}";
    html += "function initDashboard() {";
    html += "  updateStatus();";
    html += "  setInterval(updateStatus, 2000);";
    html += "}";
    html += "document.addEventListener('DOMContentLoaded', initDashboard);";
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

  server.on("/api/led/1/toggle", HTTP_POST, []() {
    redLEDState = !redLEDState;
    digitalWrite(RED_LED_PIN, redLEDState);
    Serial.printf("Red LED toggled to: %s\n", redLEDState ? "ON" : "OFF");

    String json =
        "{\"led\":1,\"state\":" + String(redLEDState ? "true" : "false") +
        ",\"message\":\"LED toggled successfully\"}";
    server.send(200, "application/json", json);
  });

  server.on("/api/led/2/toggle", HTTP_POST, []() {
    greenLEDState = !greenLEDState;
    digitalWrite(GREEN_LED_PIN, greenLEDState);
    Serial.printf("Green LED toggled to: %s\n", greenLEDState ? "ON" : "OFF");

    String json =
        "{\"led\":2,\"state\":" + String(greenLEDState ? "true" : "false") +
        ",\"message\":\"LED toggled successfully\"}";
    server.send(200, "application/json", json);
  });

  server.on("/api/led/1/state", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      String body = server.arg("plain");
      bool state = body.indexOf("\"state\":true") > -1;
      redLEDState = state;
      digitalWrite(RED_LED_PIN, state);
      Serial.printf("Red LED set to: %s\n", state ? "ON" : "OFF");
    }
    server.send(200, "application/json", "{\"message\":\"LED state updated\"}");
  });

  server.on("/api/led/2/state", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      String body = server.arg("plain");
      bool state = body.indexOf("\"state\":true") > -1;
      greenLEDState = state;
      digitalWrite(GREEN_LED_PIN, state);
      Serial.printf("Green LED set to: %s\n", state ? "ON" : "OFF");
    }
    server.send(200, "application/json", "{\"message\":\"LED state updated\"}");
  });

  server.begin();
  Serial.println("Web server started");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== IoT Workshop - Dashboard Template ===");
  Serial.printf("Team: %s\n", TEAM_NAME);
  Serial.printf("Members: %s & %s\n", MEMBER_1, MEMBER_2);
  Serial.println("=========================================");

  // Configure LED pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);

  // Setup WiFi and web server
  setupWiFi();
  setupWebServer();

  Serial.println("Dashboard Template initialized successfully!");
  Serial.println("Dashboard available at: http://" + WiFi.localIP().toString());
  Serial.println("This is your starting point for the creative challenge!");
}

void loop() {
  server.handleClient();
  delay(10);
}
