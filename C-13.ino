#include <ESP8266WiFi.h>

// Define pin connections
const int trigPin1 = D1;        // Trig pin of the front-left ultrasonic sensor
const int echoPin1 = D2;        // Echo pin of the front-left ultrasonic sensor
const int trigPin2 = D3;        // Trig pin of the front-right ultrasonic sensor
const int echoPin2 = D4;        // Echo pin of the front-right ultrasonic sensor
const int buzzerPin = D5;       // Buzzer control pin

// Define threshold distances (in centimeters)
const int distanceLevel1 = 50; // Level 1 threshold (object detected)
const int distanceLevel2 = 30;  // Level 2 threshold (object closer)
const int distanceLevel3 = 10;  // Level 3 threshold (object very close)

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);

  // Set up the ultrasonic sensor pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Set up the buzzer pin
  pinMode(buzzerPin, OUTPUT);

  // Initialize buzzer to OFF
  digitalWrite(buzzerPin, LOW);

  // Print initial status
  Serial.println("Blind Stick Initialized. Sensors ready, Buzzer OFF.");
}

void loop() {
  // Measure distances from both ultrasonic sensors
  int distance1 = getDistance(trigPin1, echoPin1);
  int distance2 = getDistance(trigPin2, echoPin2);

  // Print distances to serial monitor (for debugging)
  Serial.print("Distance Left: ");
  Serial.print(distance1);
  Serial.print(" cm | Distance Right: ");
  Serial.print(distance2);
  Serial.println(" cm");

  // Check distance levels for both sensors
  if (distance1 <= distanceLevel3 || distance2 <= distanceLevel3) {
    // Object is very close (Level 3)
    digitalWrite(buzzerPin, HIGH);  // Continuous beep
    delay(100);                     // Small delay for continuous sound
  } 
  else if (distance1 <= distanceLevel2 || distance2 <= distanceLevel2) {
    // Object is close (Level 2)
    digitalWrite(buzzerPin, HIGH);  // Fast beep-beep
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  } 
  else if (distance1 <= distanceLevel1 || distance2 <= distanceLevel1) {
    // Object is detected (Level 1)
    digitalWrite(buzzerPin, HIGH);  // Slow beep-beep
    delay(300);
    digitalWrite(buzzerPin, LOW);
    delay(300);
  } 
  else {
    // No object detected within threshold
    digitalWrite(buzzerPin, LOW);  // Buzzer OFF
  }

  // Short delay before next loop
  delay(100);
}

// Function to calculate distance from ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  // Send a 10us pulse to trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin and calculate the distance
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Speed of sound is 0.034 cm/us
  return distance;
}
