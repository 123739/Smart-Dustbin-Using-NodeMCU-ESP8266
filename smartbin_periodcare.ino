
/*
 * Project Name: Smart Dustbin using NodeMCU ESP8266
 * Components:
 * 1. NodeMCU ESP-12E
 * 2. Two HC-SR04 Ultrasonic Sensors
 * 3. Servo Motor (SG90)
 *
 * Sensor 1: Trash Level Detection
 * Sensor 2: Person Detection
 *
 * Trash Sensor TRIG: D1
 * Trash Sensor ECHO: D2
 *
 * Person Sensor TRIG: D5
 * Person Sensor ECHO: D6
 *
 * Servo Signal: D7
 */

// Include Servo library
#include <Servo.h>

// ---------------- PIN DEFINITIONS ----------------

// Ultrasonic sensor 1 (Trash level)
#define TRIG_PIN D1
#define ECHO_PIN D2

// Ultrasonic sensor 2 (Person detection)
#define PERSON_TRIG D5
#define PERSON_ECHO D6

// Servo motor
#define SERVO_PIN D7

Servo servo;

// ---------------- TRASH LEVEL SETTINGS ----------------

const float EMPTY_DISTANCE_CM = 25.0;
const float FULL_DISTANCE_CM = 5.0;

// ---------------- PERSON DETECTION SETTINGS ----------------

const float DETECTION_DISTANCE = 20.0;

// Servo positions
const int OPEN_ANGLE = 0;
const int CLOSE_ANGLE = 180;

// Lid timing
const unsigned long OPEN_TIME = 3500;

// ---------------- FUNCTION: MEASURE DISTANCE ----------------

float measureDistance(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  unsigned long duration =
      pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  float distance =
      duration * 0.0343 / 2.0;

  return distance;
}

// ---------------- SETUP ----------------

void setup() {

  Serial.begin(115200);

  // Configure trash sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Configure person detection sensor
  pinMode(PERSON_TRIG, OUTPUT);
  pinMode(PERSON_ECHO, INPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(PERSON_TRIG, LOW);

  // Initialize servo
  servo.attach(SERVO_PIN);
  servo.write(CLOSE_ANGLE);

  delay(500);

  Serial.println();
  Serial.println("SMART DUSTBIN STARTED");
  Serial.println("----------------------");
}

// ---------------- MAIN LOOP ----------------

void loop() {

  // ==========================================
  // PART 1: TRASH LEVEL DETECTION
  // ==========================================

  float trashDistance =
      measureDistance(TRIG_PIN, ECHO_PIN);

  if (trashDistance >= 0) {

    // Calculate bin fill percentage
    float fillPercent =
        (EMPTY_DISTANCE_CM - trashDistance) /
        (EMPTY_DISTANCE_CM - FULL_DISTANCE_CM) * 100.0;

    fillPercent =
        constrain(fillPercent, 0.0, 100.0);

    Serial.print("Trash Distance: ");
    Serial.print(trashDistance, 1);
    Serial.print(" cm | Bin Fill: ");

    Serial.print(fillPercent, 0);
    Serial.println("%");

    // Servicing alert
    if (fillPercent >= 80.0) {

      Serial.println("ALERT: BIN NEEDS SERVICING!");

    }

  } else {

    Serial.println("Trash sensor: No valid reading");

  }

  delay(60);

  // ==========================================
  // PART 2: PERSON DETECTION
  // ==========================================

  float personDistance =
      measureDistance(PERSON_TRIG, PERSON_ECHO);

  if (personDistance >= 0) {

    Serial.print("Person Distance: ");
    Serial.print(personDistance, 1);
    Serial.println(" cm");

    // Check if person is within 20 cm
    if (personDistance <= DETECTION_DISTANCE) {

      Serial.println("PERSON DETECTED!");
      Serial.println("OPENING DUSTBIN LID");

      // Open lid
      servo.write(OPEN_ANGLE);

      // Keep lid open for 3.5 seconds
      delay(OPEN_TIME);

      // Close lid
      servo.write(CLOSE_ANGLE);

      Serial.println("DUSTBIN LID CLOSED");

      delay(500);

    }

  } else {

    Serial.println("Person sensor: No valid reading");

  }

  Serial.println("----------------------");

  delay(300);
}
