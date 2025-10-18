#include <Arduino.h>

// LED pin definitions
const int RED_LED_PIN = D2;
const int GREEN_LED_PIN = D3;

// Team information
const char *TEAM_NAME = "Team A";
const char *MEMBER_1 = "Alice";
const char *MEMBER_2 = "Bob";

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);

  // Print welcome message
  Serial.println("\n=== IoT Workshop - Basic LED Test ===");
  Serial.printf("Team: %s\n", TEAM_NAME);
  Serial.printf("Members: %s & %s\n", MEMBER_1, MEMBER_2);
  Serial.println("=====================================");

  // Configure LED pins as outputs
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  // Turn off LEDs initially
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);

  Serial.println("LED pins configured:");
  Serial.printf("  Red LED: D%d (GPIO %d)\n", 2, RED_LED_PIN);
  Serial.printf("  Green LED: D%d (GPIO %d)\n", 3, GREEN_LED_PIN);
  Serial.println("\nStarting LED test sequence...");
}

void loop() {
  // Test Red LED
  Serial.println("Testing Red LED...");
  digitalWrite(RED_LED_PIN, HIGH);
  Serial.println("  Red LED: ON");
  delay(1000);

  digitalWrite(RED_LED_PIN, LOW);
  Serial.println("  Red LED: OFF");
  delay(500);

  // Test Green LED
  Serial.println("Testing Green LED...");
  digitalWrite(GREEN_LED_PIN, HIGH);
  Serial.println("  Green LED: ON");
  delay(1000);

  digitalWrite(GREEN_LED_PIN, LOW);
  Serial.println("  Green LED: OFF");
  delay(500);

  // Test both LEDs together
  Serial.println("Testing both LEDs...");
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, HIGH);
  Serial.println("  Both LEDs: ON");
  delay(1000);

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  Serial.println("  Both LEDs: OFF");
  delay(1000);

  // Print system info
  Serial.printf("Uptime: %lu seconds\n", millis() / 1000);
  Serial.printf("Free Heap: %u bytes\n", ESP.getFreeHeap());
  Serial.println("----------------------------------------");
}
