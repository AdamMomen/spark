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
  // Root page
  server.on("/", []() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<title>LED Control</title>";
    html +=
        "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; margin: 20px; }";
    html += "button { background: #007bff; color: white; border: none; "
            "padding: 10px 20px; margin: 5px; border-radius: 5px; }";
    html += "button:hover { background: #0056b3; }";
    html += ".status { background: #f8f9fa; padding: 10px; margin: 10px 0; "
            "border-radius: 5px; }";
    html += "</style></head><body>";
    html += "<h1>LED Control Dashboard</h1>";
    html += "<div class='status'>";
    html += "<h3>System Status</h3>";
    html += "<p>WiFi: Connected</p>";
    html += "<p>IP: " + WiFi.localIP().toString() + "</p>";
    html += "<p>Team: " + String(TEAM_NAME) + "</p>";
    html += "</div>";
    html += "<h3>LED Control</h3>";
    html += "<button onclick='toggleLED(1)'>Toggle Red LED</button>";
    html += "<button onclick='toggleLED(2)'>Toggle Green LED</button>";
    html += "<div id='status'></div>";
    html += "<script>";
    html += "function toggleLED(led) {";
    html += "  fetch('/api/led/' + led + '/toggle', {method: 'POST'})";
    html += "    .then(response => response.json())";
    html += "    .then(data => updateStatus());";
    html += "}";
    html += "function updateStatus() {";
    html += "  fetch('/api/status')";
    html += "    .then(response => response.json())";
    html += "    .then(data => {";
    html += "      document.getElementById('status').innerHTML = ";
    html += "        'Red LED: ' + (data.leds['1'] ? 'ON' : 'OFF') + '<br>' +";
    html += "        'Green LED: ' + (data.leds['2'] ? 'ON' : 'OFF');";
    html += "    });";
    html += "}";
    html += "setInterval(updateStatus, 1000);";
    html += "updateStatus();";
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

  server.begin();
  Serial.println("Web server started");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== IoT Workshop - Web Control ===");
  Serial.printf("Team: %s\n", TEAM_NAME);
  Serial.printf("Members: %s & %s\n", MEMBER_1, MEMBER_2);
  Serial.println("==================================");

  // Configure LED pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);

  // Setup WiFi and web server
  setupWiFi();
  setupWebServer();

  Serial.println("Web Control initialized successfully!");
  Serial.println("Dashboard available at: http://" + WiFi.localIP().toString());
}

void loop() {
  server.handleClient();
  delay(10);
}
