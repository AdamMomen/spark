#include "workshop_esp.h"
#include <Arduino.h>

// WorkshopESP instance
WorkshopESP workshop;

// Team configuration - Update these for each team
const char *TEAM_NAME = "Team A";
const char *MEMBER_1 = "Alice";
const char *MEMBER_2 = "Bob";

// WiFi credentials - Update these for your network
const char *WIFI_SSID = "WorkshopWiFi";
const char *WIFI_PASSWORD = "workshop2025";

// Cool Animation for 0.96" OLED Screen
void displayCoolAnimation() {
  // Simple, screen-friendly animations
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
    for (int i = 0; i < 10; i++) {
      workshop.displayMessage(animationFrames[i]);
      delay(600);
    }
    delay(1000);
  }
}

void displayTeamWelcome() {
  // Cool team welcome sequence
  const char *welcomeMessages[] = {"Welcome to",
                                   "IoT Workshop!",
                                   "",
                                   TEAM_NAME,
                                   "",
                                   "Members:",
                                   MEMBER_1,
                                   "&",
                                   MEMBER_2,
                                   "",
                                   "Let's build",
                                   "something",
                                   "amazing!",
                                   "",
                                   "Ready to",
                                   "start?",
                                   "",
                                   "Power ON!",
                                   "🚀"};

  // Animate welcome messages
  for (int i = 0; i < 19; i++) {
    workshop.displayMessage(welcomeMessages[i]);
    delay(600);
  }
}

void displayCoolPatterns() {
  // Display cool LED patterns
  Serial.println("Displaying cool LED patterns...");

  // Pattern 1: Alternating blink
  for (int i = 0; i < 5; i++) {
    workshop.setLED(1, true);  // Red ON
    workshop.setLED(2, false); // Green OFF
    delay(200);
    workshop.setLED(1, false); // Red OFF
    workshop.setLED(2, true);  // Green ON
    delay(200);
  }

  // Pattern 2: Both blink together
  for (int i = 0; i < 3; i++) {
    workshop.setLED(1, true);
    workshop.setLED(2, true);
    delay(300);
    workshop.setLED(1, false);
    workshop.setLED(2, false);
    delay(300);
  }

  // Pattern 3: Chase effect
  for (int i = 0; i < 4; i++) {
    workshop.setLED(1, true);
    delay(100);
    workshop.setLED(1, false);
    workshop.setLED(2, true);
    delay(100);
    workshop.setLED(2, false);
  }

  // Turn off all LEDs
  workshop.setLED(1, false);
  workshop.setLED(2, false);
}

void displaySystemInfo() {
  // Display system information
  workshop.displayMessage("System Ready!");
  delay(1000);

  workshop.displayMessage("WiFi: Connected");
  delay(800);

  workshop.displayMessage("IP: Ready");
  delay(800);

  workshop.displayMessage("API: Active");
  delay(800);

  workshop.displayMessage("Dashboard: Live");
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  delay(2000);

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

  Serial.println("Starting cool animation sequence...");

  // Run the cool animation sequence
  displayCoolAnimation();
  displayTeamWelcome();
  displayCoolPatterns();
  displaySystemInfo();

  // Final welcome message
  workshop.animateTeamWelcome(TEAM_NAME);

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
