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

// WiFi credentials
const char *WIFI_SSID = "1810@celcomdigifibre";
const char *WIFI_PASSWORD = "88888888";

// Web server and WebSocket
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

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
  // Root page - Combined LED + Potentiometer Control
  server.on("/", []() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<title>LED + Potentiometer Control</title>";
    html +=
        "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; text-align: center; "
            "padding: 20px; transition: background-color 0.3s ease; }";
    html += ".controls { margin: 20px; }";
    html += "button { padding: 10px 20px; margin: 10px; font-size: 16px; "
            "border: none; border-radius: 5px; cursor: pointer; }";
    html += ".red-btn { background-color: #ff4444; color: white; }";
    html += ".green-btn { background-color: #44ff44; color: black; }";
    html += ".status { margin: 20px; padding: 10px; background-color: #f0f0f0; "
            "border-radius: 5px; }";
    html += ".connection-status { margin: 10px; padding: 5px; border-radius: "
            "3px; background-color: #f0f0f0; }";
    html += ".connected { background-color: #d4edda; color: #155724; }";
    html += ".disconnected { background-color: #f8d7da; color: #721c24; }";
    html += "</style></head><body>";
    html += "<h1>IoT Control Panel</h1>";
    html += "<div class='connection-status' id='ws-status'>WebSocket: "
            "Connecting...</div>";
    html += "<div class='controls'>";
    html += "<button class='red-btn' onclick='toggleRedLED()'>Toggle Red "
            "LED</button>";
    html += "<button class='green-btn' onclick='toggleGreenLED()'>Toggle Green "
            "LED</button>";
    html += "</div>";
    html += "<script>";
    html += "let ws;";
    html += "let lastUpdateTime = new Date();";
    html += "function connectWebSocket() {";
    html += "  ws = new WebSocket('ws://' + location.hostname + ':81/');";
    html += "  ws.onopen = function() {";
    html += "    console.log('WebSocket connected');";
    html += "    document.getElementById('ws-status').textContent = "
            "'WebSocket: Connected';";
    html += "    document.getElementById('ws-status').className = "
            "'connection-status connected';";
    html += "  };";
    html += "  ws.onmessage = function(event) {";
    html += "    const data = JSON.parse(event.data);";
    html += "    updatePotDisplay(data.pot, data.voltage);";
    html += "  };";
    html += "  ws.onclose = function() {";
    html += "    console.log('WebSocket disconnected');";
    html += "    document.getElementById('ws-status').textContent = "
            "'WebSocket: Disconnected';";
    html += "    document.getElementById('ws-status').className = "
            "'connection-status disconnected';";
    html += "    setTimeout(connectWebSocket, 2000);";
    html += "  };";
    html += "  ws.onerror = function(error) {";
    html += "    console.error('WebSocket error:', error);";
    html += "  };";
    html += "}";
    html += "function updatePotDisplay(potValue, voltage) {";
    // html += "  document.getElementById('potValue').textContent = potValue;";
    // html += "  document.getElementById('voltage').textContent = 'Voltage: ' +
    // "
    //         "voltage + ' V';";
    html += "  const red = Math.floor((potValue / 1023.0) * 255);";
    html += "  const blue = Math.floor(255 - (potValue / 1023.0) * 255);";
    html += "  document.body.style.backgroundColor = 'rgb(' + red + ', 0, ' + "
            "blue + ')';";
    html += "}";
    html += "function toggleRedLED() {";
    html += "  fetch('/api/led/red/toggle', {method: 'POST'})";
    html += "    .then(response => response.text())";
    html += "    .then(data => console.log(data));";
    html += "}";
    html += "function toggleGreenLED() {";
    html += "  fetch('/api/led/green/toggle', {method: 'POST'})";
    html += "    .then(response => response.text())";
    html += "    .then(data => console.log(data));";
    html += "}";
    html += "connectWebSocket();";
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

  // Setup WiFi, web server, and WebSocket
  setupWiFi();
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
