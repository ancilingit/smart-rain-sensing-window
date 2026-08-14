/*
  Smart Rain-Sensing Window System
  NexBuildOn Hack 2026 - Round 2 Prototype

  Reads a rain sensor and controls a servo-driven window.
  - Dry: no action
  - Light rain: asks passenger for permission (Yes/No buttons), times out to auto-close
  - Heavy rain: closes immediately, no permission needed
  - Driver override button takes priority at all times
*/

#include <Servo.h>

// ---- Pin definitions ----
const int RAIN_SENSOR_PIN     = A0;
const int YES_BUTTON_PIN      = 2;
const int NO_BUTTON_PIN       = 3;
const int OVERRIDE_BUTTON_PIN = 4;
const int BUZZER_PIN          = 5;
const int LED_GREEN_PIN       = 6;  // window open indicator
const int LED_RED_PIN         = 7;  // window closed indicator
const int SERVO_PIN           = 9;

// ---- Thresholds (calibrate these using your own sensor's readings) ----
// Lower analog value generally means more water on the sensing plate.
const int LIGHT_RAIN_THRESHOLD = 500;  // below this = at least light rain
const int HEAVY_RAIN_THRESHOLD = 300;  // below this = heavy rain

// ---- Servo positions ----
const int WINDOW_OPEN_POS   = 0;
const int WINDOW_CLOSED_POS = 90;

// ---- Debounce for false positives (e.g. a single splash) ----
const unsigned long SUSTAINED_READING_MS = 3000; // must stay wet for 3 sec to count

Servo windowServo;
bool windowClosed = false;
bool waitingForResponse = false;
unsigned long wetSince = 0;
unsigned long promptStartTime = 0;
const unsigned long RESPONSE_TIMEOUT = 10000; // 10 sec to respond

void setup() {
  Serial.begin(9600);
  pinMode(YES_BUTTON_PIN, INPUT_PULLUP);
  pinMode(NO_BUTTON_PIN, INPUT_PULLUP);
  pinMode(OVERRIDE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  windowServo.attach(SERVO_PIN);
  openWindow();
}

void loop() {
  int rainValue = analogRead(RAIN_SENSOR_PIN);
  Serial.println(rainValue);

  // Driver override always takes priority over everything else
  if (digitalRead(OVERRIDE_BUTTON_PIN) == LOW) {
    toggleWindowManually();
    delay(500); // simple debounce for the button itself
    return;
  }

  if (rainValue < HEAVY_RAIN_THRESHOLD) {
    // Heavy rain -> close immediately, no permission needed
    if (!windowClosed) {
      closeWindow();
      alertHeavyRain();
    }
    waitingForResponse = false;
    wetSince = 0;
  }
  else if (rainValue < LIGHT_RAIN_THRESHOLD) {
    // Possible light rain - require a sustained reading to rule out a one-off splash
    if (wetSince == 0) {
      wetSince = millis();
    }

    bool sustainedEnough = (millis() - wetSince) >= SUSTAINED_READING_MS;

    if (sustainedEnough && !windowClosed && !waitingForResponse) {
      waitingForResponse = true;
      promptStartTime = millis();
      Serial.println("Light rain detected. Close window? (press YES or NO)");
    }

    if (waitingForResponse) {
      if (digitalRead(YES_BUTTON_PIN) == LOW) {
        closeWindow();
        waitingForResponse = false;
      } else if (digitalRead(NO_BUTTON_PIN) == LOW) {
        waitingForResponse = false;
        Serial.println("Passenger chose to keep the window open.");
      } else if (millis() - promptStartTime > RESPONSE_TIMEOUT) {
        // No response within the timeout -> default to the safer action
        closeWindow();
        waitingForResponse = false;
        Serial.println("No response - auto-closing for safety.");
      }
    }
  }
  else {
    // Dry - reset tracking, no action needed
    wetSince = 0;
    waitingForResponse = false;
  }

  delay(200);
}

void closeWindow() {
  windowServo.write(WINDOW_CLOSED_POS);
  windowClosed = true;
  digitalWrite(LED_RED_PIN, HIGH);
  digitalWrite(LED_GREEN_PIN, LOW);
  Serial.println("Window CLOSED.");
}

void openWindow() {
  windowServo.write(WINDOW_OPEN_POS);
  windowClosed = false;
  digitalWrite(LED_GREEN_PIN, HIGH);
  digitalWrite(LED_RED_PIN, LOW);
  Serial.println("Window OPEN.");
}

void toggleWindowManually() {
  if (windowClosed) {
    openWindow();
  } else {
    closeWindow();
  }
}

void alertHeavyRain() {
  tone(BUZZER_PIN, 1000, 500); // short beep to alert passengers nearby
}
