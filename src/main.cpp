#include "workshop_esp.h"
#include <Arduino.h>

// WorkshopESP instance for testing go-to-definition
WorkshopESP workshop;
char teamName[] = "Team 1";

void setup() {
  // Test go-to-definition: try Cmd+Click on setupWiFi
  String wifiSSID = String("IoT-Workshop-") + String(teamName);
  workshop.setupWiFiAP(wifiSSID.c_str(), "");
  workshop.animateHello(teamName);
}

void loop() {
  // Test go-to-definition: try Cmd+Click on handleClient
  workshop.handleClient();
}
