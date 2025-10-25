#include "workshop_esp.h"

WorkshopESP::WorkshopESP() {
  Serial.println("CONSTRUCTOR CALLED!");
  Serial.println("=== NEW CODE VERSION ===");
  Serial.println("Constructor: RFID pins being set...");

  server = new ESP8266WebServer(80);

  // Temporarily disable OLED to avoid I2C/SPI pin conflicts
  // Wire.begin(OLED_SDA, OLED_SCL);
  // display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire,
  // OLED_RESET);
  display = nullptr; // Disable display for now

  // Initialize RFID pins - Try GPIO4 for SS
  rfidSSPin = 2;  // GPIO2 (SS/CS) - D4 pin
  rfidRSTPin = 0; // GPIO0 (RST) - D3 pin

  Serial.printf("Constructor: Set rfidSSPin = D%d, rfidRSTPin = D%d\n",
                rfidSSPin, rfidRSTPin);
  Serial.printf("Constructor: D4 = %d, D8 = %d\n", D4, D8);

  // Initialize RFID with GPIO2 for SS
  rfid = new MFRC522(rfidSSPin, rfidRSTPin);

  Serial.printf("After RFID init: rfidSSPin = GPIO%d, rfidRSTPin = GPIO%d\n",
                rfidSSPin, rfidRSTPin);

  // Initialize member variables - DISABLE LEDs to save power
  redLEDPin = -1;   // Disabled to save power
  greenLEDPin = -1; // Disabled to save power
  redLEDState = false;
  greenLEDState = false;
  otaPassword = "workshop123";
}

WorkshopESP::~WorkshopESP() {
  delete server;
  if (display != nullptr)
    delete display;
  delete rfid;
}

void WorkshopESP::setupWiFi(const char *ssid, const char *password) {
  this->ssid = ssid;
  this->password = password;

  Serial.println("Starting WiFi setup...");

  // Display WiFi connection start
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println("Connecting to WiFi");
  display->setCursor(0, 15);
  display->printf("SSID: %s", ssid);
  display->setCursor(0, 30);
  display->println("Initializing...");
  display->display();

  // Set WiFi mode and disconnect any existing connection
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("WiFi mode set to STA");

  // Begin WiFi connection with timeout
  WiFi.begin(ssid, password);
  Serial.println("WiFi.begin() called");

  int attempts = 0;
  int maxAttempts = 10; // Reduced attempts to prevent hanging

  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
    delay(1000); // Increased delay for stability
    Serial.printf("WiFi status: %d, attempt %d/%d\n", WiFi.status(),
                  attempts + 1, maxAttempts);

    // Update display with connection progress
    display->setCursor(0, 45);
    display->printf("Attempt %d/%d", attempts + 1, maxAttempts);
    display->setCursor(0, 55);
    display->print("Connecting");
    for (int i = 0; i < (attempts % 3); i++) {
      display->print(".");
    }
    display->display();

    attempts++;

    // Check for watchdog reset
    if (attempts % 3 == 0) {
      Serial.println("Checking system stability...");
      yield(); // Allow other tasks to run
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected Successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

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
    Serial.printf("Final WiFi status: %d\n", WiFi.status());

    // Display failure message
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->println("WiFi Failed!");
    display->setCursor(0, 15);
    display->printf("Status: %d", WiFi.status());
    display->setCursor(0, 30);
    display->println("Continuing offline");
    display->setCursor(0, 45);
    display->println("Check network");
    display->display();
    delay(2000);
  }

  Serial.println("WiFi setup complete");
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

void WorkshopESP::setupRFID() {
  Serial.println("Setting up RFID with custom SPI pins...");

  // Initialize SPI with custom pins to avoid OLED conflict
  // Default ESP8266 SPI: MOSI=13, MISO=12, SCK=14
  // OLED uses: SDA=14, SCL=12 (conflict!)
  // Solution: Use different pins for RFID SPI

  // Simple SPI initialization - let MFRC522 handle the pins
  SPI.begin();
  rfid->PCD_Init();

  Serial.println("RFID RC522 initialized");
  Serial.printf("RFID SS Pin: GPIO%d (correct!)\n", rfidSSPin);
  Serial.printf("RFID RST Pin: GPIO%d (correct!)\n", rfidRSTPin);
  Serial.println("Note: Using GPIO numbers directly, not D pin names");
  Serial.println("OLED I2C: SDA=D5(GPIO14), SCL=D6(GPIO12)");
  Serial.println(
      "SPI Default: MOSI=D7(GPIO13), MISO=D6(GPIO12), SCK=D5(GPIO14)");

  // Test RFID reader with multiple attempts
  Serial.println("Testing RFID communication...");
  byte version = 0xFF;

  for (int i = 0; i < 5; i++) {
    version = rfid->PCD_ReadRegister(rfid->VersionReg);
    Serial.printf("Attempt %d: MFRC522 Version: 0x%02X\n", i + 1, version);
    if (version != 0xFF && version != 0x00) {
      Serial.println("SUCCESS: Valid version detected!");
      break;
    }
    delay(100);
  }

  // Additional debugging
  Serial.printf("SPI Settings: MOSI=GPIO13, MISO=GPIO12, SCK=GPIO14\n");
  Serial.printf("OLED I2C: SDA=GPIO14, SCL=GPIO12 (CONFLICT!)\n");
  Serial.printf("RFID SS=GPIO%d, RST=GPIO%d\n", rfidSSPin, rfidRSTPin);

  if (version == 0x00 || version == 0xFF) {
    Serial.println("WARNING: RFID reader may not be connected properly!");
    Serial.println("Possible causes:");
    Serial.println("1. Pin conflicts with OLED I2C");
    Serial.println("2. Wrong wiring");
    Serial.println("3. Power issues");
    Serial.println("4. SPI communication failure");
  } else {
    Serial.println("RFID reader is working correctly");
  }

  delay(1000);
}

void WorkshopESP::start() {
  setupWiFi(ssid, password);
  setupWebServer();
  setupOTA();
  setupDisplay();
  setupLEDs();
  setupRFID();

  Serial.println("WorkshopESP initialized successfully!");
  displayMessage("Ready for Workshop!", false);
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
  display->println("Welcome");

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

void WorkshopESP::displayMessage(const char *message, bool header) {
  if (display == nullptr) {
    Serial.println("Display disabled - message: " + String(message));
    return;
  }

  display->clearDisplay();
  display->setTextSize(header ? 2 : 1); // If header, use bigger text size
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
                                        const char *member2,
                                        const char *member3) {
  Serial.println("Starting animation sequence...");

  // Phase 1: Boot sequence with loading bars
  display->clearDisplay();
  display->setTextSize(2); // Bigger text
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println("IoT Workshop");
  display->setCursor(0, 20);
  display->println("Initializing...");

  // Animated loading bar
  for (int i = 0; i <= 100; i += 5) {
    display->setTextSize(1); // Smaller for loading text
    display->setCursor(0, 40);
    display->printf("Loading: %d%%", i);
    display->setCursor(0, 50);
    display->print("[");
    for (int j = 0; j < 20; j++) {
      if (j < (i / 5)) {
        display->print("=");
      } else {
        display->print(" ");
      }
    }
    display->print("]");
    display->display();
    delay(100);
  }

  delay(1000);

  // Phase 2: Matrix-style text effect
  const char *matrixText[] = {"IoT Workshop"};
  for (int phase = 0; phase < 2; phase++) {
    for (int i = 0; i < 3; i++) {
      display->clearDisplay();
      display->setTextSize(2); // Bigger text
      display->setTextColor(SSD1306_WHITE);

      // Typewriter effect
      String text = matrixText[i];
      for (int j = 0; j <= text.length(); j++) {
        display->setCursor(0, 25);
        display->print(text.substring(0, j));
        display->setCursor(0, 45);
        display->print("_");
        display->display();
        delay(150);
      }
      delay(500);
    }
  }

  // Phase 3: Pulsing welcome
  for (int pulse = 0; pulse < 5; pulse++) {
    display->clearDisplay();
    display->setTextSize(3); // Bigger welcome text
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 25);
    display->println("Welcome!");
    display->display();
    delay(300);

    display->clearDisplay();
    display->display();
    delay(200);
  }

  // Phase 4: Team introduction with style
  display->clearDisplay();
  display->setTextSize(3); // Big team name
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println(teamName);

  display->setTextSize(1); // Small member text to fit screen
  display->setCursor(0, 25);
  display->println("Members:");
  display->setCursor(0, 35);
  display->printf("-- %s", member1);
  display->setCursor(0, 45);
  display->printf("-- %s", member2);
  display->setCursor(0, 55);
  display->printf("-- %s", member3);
  display->display();
  delay(30000); // Show all members for 30 seconds

  // Phase 5: LED light show
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println("LED Light Show");
  display->setCursor(0, 15);
  display->println("Watch the LEDs!");
  display->display();

  // LED animation sequence
  for (int cycle = 0; cycle < 3; cycle++) {
    // Red LED sequence
    setLED(1, true);
    display->setCursor(0, 30);
    display->println("Red LED ON");
    display->display();
    delay(500);

    setLED(1, false);
    setLED(2, true);
    display->setCursor(0, 30);
    display->println("Green LED ON");
    display->display();
    delay(500);

    setLED(2, false);
    setLED(1, true);
    setLED(2, true);
    display->setCursor(0, 30);
    display->println("Both LEDs ON");
    display->display();
    delay(500);

    setLED(1, false);
    setLED(2, false);
    display->setCursor(0, 30);
    display->println("LEDs OFF");
    display->display();
    delay(300);
  }

  // Phase 6: System status with animations
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println("System Status");
  display->setCursor(0, 15);
  display->printf("Uptime: %lu s", millis() / 1000);
  display->setCursor(0, 25);
  display->printf("Free RAM: %u", ESP.getFreeHeap());
  display->setCursor(0, 35);
  display->println("WiFi: Ready");
  display->setCursor(0, 45);
  display->println("OLED: Working");
  display->setCursor(0, 55);
  display->println("System: GO!");
  display->display();
  delay(2000);

  // Phase 7: Final countdown
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 20);
  display->println("Starting in");
  display->display();
  delay(1000);

  for (int count = 3; count > 0; count--) {
    display->clearDisplay();
    display->setTextSize(3);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(50, 25);
    display->printf("%d", count);
    display->display();
    delay(1000);
  }

  // Final blast
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 20);
  display->println("LET'S GO!");
  display->setCursor(0, 40);
  display->println("IoT Workshop");
  display->display();

  // Flash LEDs for final effect
  for (int flash = 0; flash < 5; flash++) {
    setLED(1, true);
    setLED(2, true);
    delay(100);
    setLED(1, false);
    setLED(2, false);
    delay(100);
  }

  delay(2000);

  Serial.println("WOW animation sequence complete!");
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

bool WorkshopESP::checkRFID() {
  // Look for new cards
  if (!rfid->PICC_IsNewCardPresent()) {
    return false;
  }

  // Select one of the cards
  if (!rfid->PICC_ReadCardSerial()) {
    return false;
  }

  return true;
}

String WorkshopESP::getRFIDUID() {
  String uid = "";
  for (byte i = 0; i < rfid->uid.size; i++) {
    if (rfid->uid.uidByte[i] < 0x10) {
      uid += "0";
    }
    uid += String(rfid->uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

void WorkshopESP::debugPinValues() {
  Serial.println("=== PIN DEBUG ===");
  Serial.printf("rfidSSPin = GPIO%d (was D4=%d)\n", rfidSSPin, D4);
  Serial.printf("rfidRSTPin = GPIO%d (was D8=%d)\n", rfidRSTPin, D8);
  Serial.printf("D4 = %d, D8 = %d\n", D4, D8);
  Serial.println("================");
}

void WorkshopESP::displayRFIDInfo() {
  // Check if a new card is present
  if (!rfid->PICC_IsNewCardPresent()) {
    return; // No new card, exit early
  }

  // Try to read the card
  if (!rfid->PICC_ReadCardSerial()) {
    return; // Failed to read card
  }

  // Card detected successfully!
  String uid = getRFIDUID();
  Serial.println("RFID Card Detected!");
  Serial.println("UID: " + uid);

  // Display on OLED
  String message = "RFID Detected\nUID: " + uid;
  // displayMessage(message.c_str(), false);

  // Halt PICC
  rfid->PICC_HaltA();
  // Stop encryption on PCD
  rfid->PCD_StopCrypto1();
}
