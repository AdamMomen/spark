#include "workshop_esp.h"
#include <Arduino.h>

// WorkshopESP instance
WorkshopESP workshop;

// Team configuration - Update these for each team
const char *TEAM_NAME = "Team A";
const char *MEMBER_1 = "Alice";
const char *MEMBER_2 = "Bob";

// WiFi credentials - Update these for your network
const char *WIFI_SSID = "NS Lobby";
const char *WIFI_PASSWORD = "darktalent2024!";

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Clear OLED screen at startup
  // workshop.clearDisplay();

  Serial.println("\n");
  Serial.println(
      "╔══════════════════════════════════════════════════════════════╗");
  Serial.println(
      "║                    IoT WORKSHOP STARTUP                   ║");
  Serial.println(
      "║                      October 19, 2025                     ║");
  Serial.println(
      "╚══════════════════════════════════════════════════════════════╝");
  Serial.println();

  // Print workshop info to serial
  Serial.println(
      "╔══════════════════════════════════════════════════════════════╗");
  Serial.println(
      "║                    IoT WORKSHOP STARTUP                   ║");
  Serial.println(
      "║                      October 19, 2025                     ║");
  Serial.println(
      "║                       Network School                      ║");
  Serial.println(
      "╚══════════════════════════════════════════════════════════════╝");
  Serial.println();

  Serial.printf("Team: %s\n", TEAM_NAME);
  Serial.printf("Members: %s & %s\n", MEMBER_1, MEMBER_2);
  Serial.println("================================================");

  // Initialize WorkshopESP
  workshop.setupWiFi(WIFI_SSID, WIFI_PASSWORD);
  workshop.setupWebServer();
  workshop.setupOTA();
  workshop.setupDisplay();
  workshop.setupLEDs();

  // Play the complete animation sequence with team info
  workshop.playCompleteAnimation(TEAM_NAME, MEMBER_1, MEMBER_2);

  Serial.println("Animation complete! System ready for workshop.");
  Serial.println("Dashboard available at: http://" + WiFi.localIP().toString());
  Serial.println("================================================");
}

void loop() {
  // Handle web server requests and OTA updates
  workshop.handleClient();

  // Update display every 10 seconds with status
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate > 10000) {
    workshop.displayStatus();
    lastDisplayUpdate = millis();
  }

  // Print system info every 60 seconds
  static unsigned long lastInfoPrint = 0;
  if (millis() - lastInfoPrint > 60000) {
    workshop.printSystemInfo();
    lastInfoPrint = millis();
  }

  delay(10);
}