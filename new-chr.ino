#include "pitches.h"
int melody[] = {
  NOTE_C7 ,NOTE_D7,NOTE_C7,NOTE_D7,
  NOTE_C8,NOTE_E7,NOTE_C7,NOTE_E7,
  NOTE_C7,NOTE_D7,NOTE_C8,NOTE_D7,
  NOTE_C7,NOTE_D7,NOTE_C8,NOTE_D7
};//melody
int noteDurations[] = {
  8,8,8,8,
  8,8,8,8,
  8,8,8,8,
  8,8,8,8
};//melody duration
const int buttonPin = 2;           // Button connected to pin 2
const int buzzerPin = 7;           // Buzzer connected to pin 7
const int trigPin = 8;             // Ultrasonic sensor trigger pin
const int echoPin = 9;             // Ultrasonic sensor echo pin
unsigned long buttonPressTime = 0; // Time when button is continuously LOW
const unsigned long thresholdTime = 10000; // 30 seconds in milliseconds
bool alertActive = false;          // Flag to indicate if the buzzer should be active

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with internal pull-up
  pinMode(buzzerPin, OUTPUT);       // Set buzzer pin as output
  pinMode(trigPin, OUTPUT);         // Set ultrasonic trigger pin as output
  pinMode(echoPin, INPUT);          // Set ultrasonic echo pin as input
  Serial.begin(9600);               // Start the serial monitor
}

void loop() {
  int buttonState = digitalRead(buttonPin); // Read button state
  
  // If the button is continuously LOW for 30 seconds
  if (buttonState == LOW) {
    if (buttonPressTime == 0) {
      buttonPressTime = millis(); // Start timing if button is pressed
    } else if (millis() - buttonPressTime >= thresholdTime) {
      alertActive = true; // Set the alert to active
    }
  } else {
    buttonPressTime = 0; // Reset timing if button is not pressed
  }

  // Measure distance with ultrasonic sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Calculate distance in cm

  // If alert is active, check distance to stop buzzer
    if (alertActive) {
    if (distance < 70 and distance > 30) {
      alertActive = false;        // Stop the alert if distance is less than 10 cm
      buttonPressTime = 0;        // Reset button press timer
    } else {
      digitalWrite(buzzerPin, HIGH); // Activate buzzer
      delay(300);
      digitalWrite(buzzerPin, LOW);
      for (int thisNote = 0; thisNote < 16; thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(12, melody[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(8);
      }

 
    }
  } else {
    digitalWrite(buzzerPin, LOW);    // Deactivate buzzer

  }

  // Display pin status and distance in the Serial Monitor
  Serial.print("Button State: ");
  Serial.print(buttonState);
  Serial.print(" | Buzzer State: ");
  Serial.print(alertActive ? "ON" : "OFF");
  Serial.print(" | Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(100); // Small delay to avoid serial flooding
}


