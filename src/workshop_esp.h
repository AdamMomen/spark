#ifndef WORKSHOP_ESP_H
#define WORKSHOP_ESP_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

class WorkshopESP {
private:
  ESP8266WebServer *server;
  Adafruit_SSD1306 *display;

  // LED pins
  int redLEDPin;
  int greenLEDPin;

  // LED states
  bool redLEDState;
  bool greenLEDState;

  // WiFi credentials
  const char *ssid;
  const char *password;

  // OTA password
  const char *otaPassword;

  // Display settings
  static const int SCREEN_WIDTH = 128;
  static const int SCREEN_HEIGHT = 64;
  static const int OLED_RESET = -1;

public:
  WorkshopESP();
  ~WorkshopESP();

  // Setup methods
  void setupWiFi(const char *ssid, const char *password);
  void setupWebServer();
  void setupOTA();
  void setupDisplay();
  void setupLEDs();
  void start();

  // LED control methods
  void toggleLED(int ledNumber);
  void setLED(int ledNumber, bool state);
  bool getLEDState(int ledNumber);

  // Display methods
  void displayWelcome(const char *teamName, const char *member1,
                      const char *member2);
  void displayStatus();
  void displayMessage(const char *message);
  void animateHello(const char *teamName);

  // Web server handlers
  void handleRoot();
  void handleStatus();
  void handleLEDToggle();
  void handleLEDState();
  void handleNotFound();

  // Utility methods
  void printSystemInfo();
  String getSystemStatusJSON();
  void handleClient();

  // Team welcome animation
  void animateTeamWelcome(const char *teamName);
};

#endif
