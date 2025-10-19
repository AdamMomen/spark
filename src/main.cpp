#include "workshop_esp.h"
#include <Arduino.h>

// WorkshopESP instance
WorkshopESP workshop;

// Team configuration - Update these for each team

void setup() {
  Serial.begin(115200);
  delay(2000);

  workshop.setupDisplay();
}

void loop() {
  // Simple status update every 10 seconds
  workshop.playCompleteAnimation(TEAM_NAME, MEMBER_1, MEMBER_2, MEMBER_3);
  delay(1000);
}