#include "workshop_esp.h"

// WorkshopESP instance
WorkshopESP workshop;

// Team configuration - Update these for each team
const char *TEAM_NAME = "Team A";
const char *MEMBER_1 = "Alice";
const char *MEMBER_2 = "Bob";

// WiFi credentials - Update these for your network
const char *WIFI_SSID = "WorkshopWiFi";
const char *WIFI_PASSWORD = "workshop2025";

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== IoT Workshop Starting ===");
  Serial.printf("Team: %s\n", TEAM_NAME);
  Serial.printf("Members: %s & %s\n", MEMBER_1, MEMBER_2);
  Serial.println("=============================");

  // Initialize WorkshopESP
  workshop.setupWiFi(WIFI_SSID, WIFI_PASSWORD);
  workshop.setupWebServer();
  workshop.setupOTA();
  workshop.setupDisplay();
  workshop.setupLEDs();

  // Display team welcome animation
  workshop.animateTeamWelcome(TEAM_NAME);

  // Print system information
  workshop.printSystemInfo();

  Serial.println("WorkshopESP initialized successfully!");
  Serial.println("Dashboard available at: http://" + WiFi.localIP().toString());
  Serial.println("API endpoints:");
  Serial.println("  GET  /api/status");
  Serial.println("  POST /api/led/1/toggle");
  Serial.println("  POST /api/led/2/toggle");
  Serial.println("  POST /api/led/1/state");
  Serial.println("  POST /api/led/2/state");
}

void loop() {
  // Handle web server requests and OTA updates
  workshop.handleClient();

  // Update display every 5 seconds
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate > 5000) {
    workshop.displayStatus();
    lastDisplayUpdate = millis();
  }

  // Print system info every 30 seconds
  static unsigned long lastInfoPrint = 0;
  if (millis() - lastInfoPrint > 30000) {
    workshop.printSystemInfo();
    lastInfoPrint = millis();
  }

  // Small delay to prevent watchdog reset
  delay(10);
}
