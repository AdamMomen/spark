#include <Arduino.h>
#include <ArduinoOTA.h>
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

// OTA password
const char *OTA_PASSWORD = "workshop123";

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

void setupOTA() {
  ArduinoOTA.setHostname("workshop-esp");
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([]() { Serial.println("OTA Update Started"); });

  ArduinoOTA.onEnd([]() { Serial.println("OTA Update Finished"); });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\n", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });

  ArduinoOTA.begin();
  Serial.println("OTA Ready");
}

void setupWebServer() {
  // Root page
  server.on("/", []() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<title>IoT Workshop Dashboard</title>";
    html +=
        "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; margin: 20px; background: "
            "#f0f0f0; }";
    html += ".container { max-width: 600px; margin: 0 auto; background: white; "
            "padding: 20px; border-radius: 10px; }";
    html += "h1 { color: #333; text-align: center; }";
    html += ".led-control { margin: 20px 0; padding: 15px; border: 1px solid "
            "#ddd; border-radius: 5px; }";
    html +=
        "button { background: #007bff; color: white; border: none; padding: "
        "10px 20px; border-radius: 5px; cursor: pointer; margin: 5px; }";
    html += "button:hover { background: #0056b3; }";
    html += ".status { background: #e9ecef; padding: 10px; border-radius: 5px; "
            "margin: 10px 0; }";
    html +=
        ".ota-info { background: #d1ecf1; padding: 10px; border-radius: 5px; "
        "margin: 10px 0; border-left: 4px solid #17a2b8; }";
    html += "</style></head><body>";
    html += "<div class='container'>";
    html += "<h1>IoT Workshop Dashboard</h1>";
    html += "<div class='ota-info'>";
    html += "<h3>OTA Update Available</h3>";
    html += "<p>This device supports Over-The-Air updates!</p>";
    html += "<p>Hostname: workshop-esp</p>";
    html += "<p>Password: " + String(OTA_PASSWORD) + "</p>";
    html += "</div>";
    html += "<div class='status' id='status'>Loading...</div>";
    html += "<div class='led-control'>";
    html += "<h3>LED Control</h3>";
    html += "<button onclick='toggleLED(1)'>Toggle Red LED</button>";
    html += "<button onclick='toggleLED(2)'>Toggle Green LED</button>";
    html += "</div>";
    html += "</div>";
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
    html += "        'WiFi: ' + (data.wifi_connected ? 'Connected' : "
            "'Disconnected') + '<br>' +";
    html += "        'Uptime: ' + data.uptime + 's<br>' +";
    html += "        'Free Heap: ' + data.free_heap + ' bytes<br>' +";
    html += "        'Red LED: ' + (data.leds['1'] ? 'ON' : 'OFF') + '<br>' +";
    html += "        'Green LED: ' + (data.leds['2'] ? 'ON' : 'OFF');";
    html += "    });";
    html += "}";
    html += "setInterval(updateStatus, 2000);";
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

  Serial.println("\n=== IoT Workshop - OTA Update Demo ===");
  Serial.printf("Team: %s\n", TEAM_NAME);
  Serial.printf("Members: %s & %s\n", MEMBER_1, MEMBER_2);
  Serial.println("=====================================");

  // Configure LED pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);

  // Setup WiFi, OTA, and web server
  setupWiFi();
  setupOTA();
  setupWebServer();

  Serial.println("OTA Update Demo initialized successfully!");
  Serial.println("Dashboard available at: http://" + WiFi.localIP().toString());
  Serial.println("OTA Hostname: workshop-esp");
  Serial.println("OTA Password: " + String(OTA_PASSWORD));
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  delay(10);
}
