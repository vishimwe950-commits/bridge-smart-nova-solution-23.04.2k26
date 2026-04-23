// Smart Nova Solutions Security Alarm System
// Arduino Mega 2560

// Pin assignments
const int doorSensorPin = 7;     // Door push button
const int resetButtonPin = 8;    // Reset push button
const int redLedPin = 2;         // Alarm LED
const int greenLedPin = 3;       // Status LED
const int buzzerPin = 6;         // Buzzer
const int ldrPin = A0;           // Light sensor

// State variables
bool alarmActive = false;

void setup() {
  pinMode(doorSensorPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);

  // Start in monitoring mode
  digitalWrite(greenLedPin, HIGH);
  Serial.println("MONITORING ACTIVE");
}

void loop() {
  int doorState = digitalRead(doorSensorPin);
  int resetState = digitalRead(resetButtonPin);
  int ldrValue = analogRead(ldrPin);

  // Night mode check
  if (!alarmActive) {
    if (ldrValue > 600) {
      // Dark → Night mode
      Serial.println("NIGHT MODE ON");
      digitalWrite(greenLedPin, HIGH);
      delay(500);
      digitalWrite(greenLedPin, LOW);
      delay(500);
    } else {
      // Normal monitoring
      digitalWrite(greenLedPin, HIGH);
    }
  }

  // Door sensor pressed → intruder detected
  if (doorState == LOW && !alarmActive) {
    alarmActive = true;
    Serial.println("INTRUDER DETECTED");

    // Flash red LED rapidly 10 times
    for (int i = 0; i < 10; i++) {
      digitalWrite(redLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(redLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }

    // Keep alarm ON
    digitalWrite(redLedPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  }

  // Reset button pressed → turn alarm off
  if (resetState == LOW && alarmActive) {
    alarmActive = false;
    Serial.println("SYSTEM RESET");

    // Turn off alarm
    digitalWrite(redLedPin, LOW);
    digitalWrite(buzzerPin, LOW);

    // Return to monitoring
    digitalWrite(greenLedPin, HIGH);
    Serial.println("MONITORING ACTIVE");
  }
}
