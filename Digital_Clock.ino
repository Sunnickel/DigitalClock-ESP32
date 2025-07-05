#include <WiFi.h>
#include "time.h"

// Thats a Hotspot btw so yeah
char ssid[] = "dings";
char pass[] = "8765432!";

const char* ntpServer = "pool.ntp.org";
const char* timezone = "CET-1CEST,M3.5.0/2,M10.5.0/3";
struct tm timeinfo;

int segTop = 3;
int segTopLeft = 46;
int segTopRight = 13;
int segMiddle = 10;
int segBottomLeft = 11;
int segBottomRight = 12;
int segBottom = 9;

//                     A,           B,            C,           D,            E,         F,        G
int segments[] = { segTopLeft, segTopRight, segBottomRight, segMiddle, segBottomLeft, segTop, segBottom };

int digit1Common = 17;  // First digit (leftmost)
int digit2Common = 37;  // Second digit
int digit3Common = 16;  // Third digit
int digit4Common = 38;  // Fourth digit
int digit5Common = 15;  // Fifth digit
int digit6Common = 39;  // Sixth digit (rightmost)

int commonDigits[] = { digit1Common, digit2Common, digit3Common, digit4Common, digit5Common, digit6Common };

int digits[10][7] = {
  //A, B, C, D, E, F, G
  { 1, 1, 1, 1, 1, 1, 0 },  // 0
  { 0, 1, 1, 0, 0, 0, 0 },  // 1
  { 1, 1, 0, 1, 1, 0, 1 },  // 2
  { 1, 1, 1, 1, 0, 0, 1 },  // 3
  { 0, 1, 1, 0, 0, 1, 1 },  // 4
  { 1, 0, 1, 1, 0, 1, 1 },  // 5
  { 1, 0, 1, 1, 1, 1, 1 },  // 6
  { 1, 1, 1, 0, 0, 0, 0 },  // 7
  { 1, 1, 1, 1, 1, 1, 1 },  // 8
  { 1, 1, 1, 1, 0, 1, 1 }   // 9
};

unsigned long lastUpdate = 0;
int currentDigit = 0;

void setup() {
  Serial.begin(115200);

  // Segment pins
  pinMode(segTop, OUTPUT);
  pinMode(segTopLeft, OUTPUT);
  pinMode(segTopRight, OUTPUT);
  pinMode(segMiddle, OUTPUT);
  pinMode(segBottom, OUTPUT);
  pinMode(segBottomLeft, OUTPUT);
  pinMode(segBottomRight, OUTPUT);

  // Common Pins
  pinMode(digit1Common, OUTPUT);
  pinMode(digit2Common, OUTPUT);
  pinMode(digit3Common, OUTPUT);
  pinMode(digit4Common, OUTPUT);
  pinMode(digit5Common, OUTPUT);
  pinMode(digit6Common, OUTPUT);

  // activate all
  for (int i = 0; i < 6; i++) {
    digitalWrite(commonDigits[i], HIGH);
  }

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);

  WiFi.useStaticBuffers(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to WiFi");

  configTzTime(timezone, ntpServer);

  displayDigit(0);
}

void loop() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  int displayDigits[] = {
    timeinfo.tm_hour / 10,
    timeinfo.tm_hour % 10,
    timeinfo.tm_min / 10,
    timeinfo.tm_min % 10,
    timeinfo.tm_sec / 10,
    timeinfo.tm_sec % 10
  };

  for (int i = 0; i < 6; i++) {
    showDigit(i, displayDigits[i]);
    delay(1);
  }
}

void showDigit(int digitIndex, int number) {
  for (int i = 0; i < 6; i++) {
    digitalWrite(commonDigits[i], HIGH);
  }

  displayDigit(number);

  digitalWrite(commonDigits[digitIndex], LOW);
}

void displayDigit(int digit) {
  if (digit < 0 || digit > 9) {
    return;
  }
  for (int i = 0; i < 7; i++) {
    digitalWrite(segments[i], digits[digit][i] ? HIGH : LOW);
  }
}