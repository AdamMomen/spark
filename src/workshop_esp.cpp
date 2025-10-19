#include "workshop_esp.h"

WorkshopESP::WorkshopESP() {
  server = new ESP8266WebServer(80);

  // Initialize I2C with custom pins
  Wire.begin(OLED_SDA, OLED_SCL);

  display =
      new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  // Initialize member variables
  redLEDPin = D2;
  greenLEDPin = D3;
  redLEDState = false;
  greenLEDState = false;
  otaPassword = "workshop123";
}

WorkshopESP::~WorkshopESP() {
  delete server;
  delete display;
}

void WorkshopESP::setupWiFi(const char *ssid, const char *password) {
  this->ssid = ssid;
  this->password = password;

  Serial.println("Connecting to WiFi...");

  // Display WiFi connection start
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println("Connecting to WiFi");
  display->setCursor(0, 15);
  display->printf("SSID: %s", ssid);
  display->setCursor(0, 30);
  display->println("Please wait...");
  display->display();

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");

    // Update display with connection progress
    display->setCursor(0, 45);
    display->printf("Attempt %d/20", attempts + 1);
    display->setCursor(0, 55);
    display->print("Connecting");
    for (int i = 0; i < (attempts % 4); i++) {
      display->print(".");
    }
    display->display();

    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());

    // Display success message
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->println("WiFi Connected!");
    display->setCursor(0, 15);
    display->printf("IP: %s", WiFi.localIP().toString().c_str());
    display->setCursor(0, 30);
    display->printf("Signal: %d dBm", WiFi.RSSI());
    display->setCursor(0, 45);
    display->println("Ready for workshop!");
    display->display();
    delay(2000);

  } else {
    Serial.println("\nWiFi Connection Failed!");

    // Display failure message
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->println("WiFi Failed!");
    display->setCursor(0, 15);
    display->println("Check credentials");
    display->setCursor(0, 30);
    display->println("or network");
    display->setCursor(0, 45);
    display->println("Continuing offline...");
    display->display();
    delay(2000);
  }
}

void WorkshopESP::setupWebServer() {
  // Root page
  server->on("/", [this]() { handleRoot(); });

  // API endpoints
  server->on("/api/status", [this]() { handleStatus(); });
  server->on("/api/led/1/toggle", HTTP_POST, [this]() {
    toggleLED(1);
    server->send(200, "application/json", getSystemStatusJSON());
  });
  server->on("/api/led/2/toggle", HTTP_POST, [this]() {
    toggleLED(2);
    server->send(200, "application/json", getSystemStatusJSON());
  });
  server->on("/api/led/1/state", HTTP_POST, [this]() { handleLEDState(); });
  server->on("/api/led/2/state", HTTP_POST, [this]() { handleLEDState(); });

  // 404 handler
  server->onNotFound([this]() { handleNotFound(); });

  server->begin();
  Serial.println("Web server started");
}

void WorkshopESP::setupOTA() {
  ArduinoOTA.setHostname("workshop-esp");
  ArduinoOTA.setPassword(otaPassword);

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

void WorkshopESP::setupDisplay() {
  Serial.println("Initializing OLED display...");
  Serial.printf("Using SDA: D%d (GPIO%d), SCL: D%d (GPIO%d)\n", OLED_SDA,
                OLED_SDA, OLED_SCL, OLED_SCL);

  if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed - Check wiring!");
    Serial.println("Make sure OLED is connected to:");
    Serial.printf("  SDA -> D%d (GPIO%d)\n", OLED_SDA, OLED_SDA);
    Serial.printf("  SCL -> D%d (GPIO%d)\n", OLED_SCL, OLED_SCL);
    Serial.println("  VCC -> 3.3V");
    Serial.println("  GND -> GND");
    return;
  }

  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println("IoT Workshop");
  display->println("Initializing...");
  display->display();

  Serial.println("OLED Display initialized successfully!");
}

void WorkshopESP::setupLEDs() {
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  digitalWrite(redLEDPin, LOW);
  digitalWrite(greenLEDPin, LOW);

  Serial.println("LEDs initialized");
}

void WorkshopESP::start() {
  setupWiFi(ssid, password);
  setupWebServer();
  setupOTA();
  setupDisplay();
  setupLEDs();

  Serial.println("WorkshopESP initialized successfully!");
  displayMessage("Ready for Workshop!");
}

void WorkshopESP::toggleLED(int ledNumber) {
  if (ledNumber == 1) {
    redLEDState = !redLEDState;
    digitalWrite(redLEDPin, redLEDState);
    Serial.printf("Red LED toggled to: %s\n", redLEDState ? "ON" : "OFF");
  } else if (ledNumber == 2) {
    greenLEDState = !greenLEDState;
    digitalWrite(greenLEDPin, greenLEDState);
    Serial.printf("Green LED toggled to: %s\n", greenLEDState ? "ON" : "OFF");
  }
}

void WorkshopESP::setLED(int ledNumber, bool state) {
  if (ledNumber == 1) {
    redLEDState = state;
    digitalWrite(redLEDPin, state);
    Serial.printf("Red LED set to: %s\n", state ? "ON" : "OFF");
  } else if (ledNumber == 2) {
    greenLEDState = state;
    digitalWrite(greenLEDPin, state);
    Serial.printf("Green LED set to: %s\n", state ? "ON" : "OFF");
  }
}

bool WorkshopESP::getLEDState(int ledNumber) {
  if (ledNumber == 1)
    return redLEDState;
  if (ledNumber == 2)
    return greenLEDState;
  return false;
}

// void WorkshopESP::clearDisplay() {
//   display->clearDisplay();
//   display->display();
// }

void WorkshopESP::displayWelcome(const char *teamName, const char *member1,
                                 const char *member2) {
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println(teamName);

  display->setTextSize(1);
  display->setCursor(0, 20);
  display->println("Members:");
  display->setCursor(0, 30);
  display->println(member1);
  display->setCursor(0, 40);
  display->println(member2);

  display->setCursor(0, 55);
  display->println("Welcome!");

  display->display();
}

void WorkshopESP::displayStatus() {
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);

  display->setCursor(0, 0);
  if (WiFi.status() == WL_CONNECTED) {
    display->printf("WiFi: Connected\n");
    display->printf("IP: %s\n", WiFi.localIP().toString().c_str());
    display->printf("Signal: %d dBm\n", WiFi.RSSI());
  } else {
    display->printf("WiFi: Disconnected\n");
    display->printf("SSID: %s\n", ssid);
  }
  display->printf("Uptime: %lu s\n", millis() / 1000);
  display->printf("Free Heap: %u\n", ESP.getFreeHeap());
  display->printf("Red LED: %s\n", redLEDState ? "ON" : "OFF");
  display->printf("Green LED: %s\n", greenLEDState ? "ON" : "OFF");

  display->display();
}

void WorkshopESP::displayMessage(const char *message) {
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println(message);
  display->display();
}

void WorkshopESP::animateHello(const char *teamName) {
  for (int i = 0; i < 3; i++) {
    display->clearDisplay();
    display->setTextSize(2);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 20);
    display->println("Hello");
    display->setCursor(0, 40);
    display->println(teamName);
    display->display();
    delay(500);

    display->clearDisplay();
    display->display();
    delay(200);
  }
}

void WorkshopESP::animateTeamWelcome(const char *teamName) {
  // Animate "Hello" message
  animateHello(teamName);

  // Display welcome message
  displayWelcome(teamName, "Member 1", "Member 2");
  delay(3000);

  // Show status
  displayStatus();
}

void WorkshopESP::playCompleteAnimation(const char *teamName,
                                        const char *member1,
                                        const char *member2) {
  Serial.println("Starting complete animation sequence...");

  // Cool Animation for 0.96" OLED Screen
  const char *animationFrames[] = {"IoT Workshop",
                                   "October 2025",
                                   "Network School",
                                   "",
                                   "Ready to",
                                   "Build IoT!",
                                   "",
                                   "Let's Start!",
                                   "🚀"};

  // Display animation frames
  for (int frame = 0; frame < 3; frame++) {
    for (int i = 0; i < 9; i++) {
      displayMessage(animationFrames[i]);
      delay(600);
    }
    delay(1000);
  }

  // Display team welcome
  displayWelcome(teamName, member1, member2);
  delay(3000);

  // Display cool patterns
  const char *patterns[] = {"Pattern 1", "Pattern 2", "Pattern 3"};

  for (int i = 0; i < 3; i++) {
    displayMessage(patterns[i]);
    delay(1000);
  }

  // Display system info
  displayStatus();
  delay(2000);

  // Final animated welcome
  animateTeamWelcome(teamName);

  Serial.println("Complete animation sequence finished!");
}

void WorkshopESP::handleRoot() {
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
  html += "button { background: #007bff; color: white; border: none; padding: "
          "10px 20px; border-radius: 5px; cursor: pointer; margin: 5px; }";
  html += "button:hover { background: #0056b3; }";
  html += ".status { background: #e9ecef; padding: 10px; border-radius: 5px; "
          "margin: 10px 0; }";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>IoT Workshop Dashboard</h1>";
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

  server->send(200, "text/html", html);
}

void WorkshopESP::handleStatus() {
  server->send(200, "application/json", getSystemStatusJSON());
}

void WorkshopESP::handleLEDState() {
  if (server->hasArg("plain")) {
    String body = server->arg("plain");
    // Parse JSON body for state
    bool state = body.indexOf("\"state\":true") > -1;
    String uri = server->uri();

    if (uri.indexOf("/1/state") > -1) {
      setLED(1, state);
    } else if (uri.indexOf("/2/state") > -1) {
      setLED(2, state);
    }

    server->send(200, "application/json", getSystemStatusJSON());
  } else {
    server->send(400, "application/json",
                 "{\"error\":\"Invalid request body\"}");
  }
}

void WorkshopESP::handleNotFound() {
  server->send(404, "application/json", "{\"error\":\"Not found\"}");
}

void WorkshopESP::printSystemInfo() {
  Serial.println("=== System Information ===");
  Serial.printf("WiFi Status: %s\n",
                WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
  Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("MAC Address: %s\n", WiFi.macAddress().c_str());
  Serial.printf("Signal Strength: %d dBm\n", WiFi.RSSI());
  Serial.printf("Uptime: %lu seconds\n", millis() / 1000);
  Serial.printf("Free Heap: %u bytes\n", ESP.getFreeHeap());
  Serial.printf("Red LED: %s\n", redLEDState ? "ON" : "OFF");
  Serial.printf("Green LED: %s\n", greenLEDState ? "ON" : "OFF");
  Serial.println("==========================");
}

String WorkshopESP::getSystemStatusJSON() {
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
  return json;
}

void WorkshopESP::handleClient() {
  server->handleClient();
  ArduinoOTA.handle();
}
