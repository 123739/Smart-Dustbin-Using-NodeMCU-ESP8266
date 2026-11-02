
/*
 * Smart Dustbin - NodeMCU ESP8266 + Blynk IoT
 * Hardware: 2x HC-SR04, SG90 Servo
 *
 * Trash sensor:  TRIG D1, ECHO D2
 * Person sensor: TRIG D5, ECHO D6
 * Servo signal:  D7
 *
 * Blynk Virtual Pins:
 * V0 = Fill percentage
 * V1 = Trash distance (cm)
 * V2 = Servicing alert (0/1)
 * V3 = Person distance (cm)
 */

#define BLYNK_TEMPLATE_ID   "TMPL34IqNhoPG"
#define BLYNK_TEMPLATE_NAME "periodcare"
#define BLYNK_AUTH_TOKEN    "70X1oqbbBOcXSs4V5LxRaOx3L17EbrOy"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

// Enter your Wi-Fi details
char ssid[] = "RadheShyam";
char pass[] = "RadheShyam";

// Pin definitions
#define TRIG_PIN D1
#define ECHO_PIN D2

#define PERSON_TRIG D5
#define PERSON_ECHO D6

#define SERVO_PIN D7

Servo servo;
BlynkTimer timer;

// Trash level settings
const float EMPTY_DISTANCE_CM = 25.0;
const float FULL_DISTANCE_CM = 5.0;
const float DETECTION_DISTANCE = 20.0;

// Adjust these angles for your lid mechanism
const int OPEN_ANGLE = 0;
const int CLOSE_ANGLE = 180;

const unsigned long OPEN_TIME = 3500;

bool lidOpen = false;
bool personPreviouslyDetected = false;
unsigned long lidOpenedAt = 0;

// Measure ultrasonic distance
float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration =
      pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1.0;
  }

  return duration * 0.0343 / 2.0;
}

// Read trash level and update Blynk
void updateTrashLevel() {
  float distance = measureDistance(TRIG_PIN, ECHO_PIN);

  if (distance < 0) {
    Serial.println("Trash sensor: No valid reading");
    return;
  }

  float fillPercent =
      (EMPTY_DISTANCE_CM - distance) /
      (EMPTY_DISTANCE_CM - FULL_DISTANCE_CM) * 100.0;

  fillPercent = constrain(fillPercent, 0.0, 100.0);

  int alert = (fillPercent >= 80.0) ? 1 : 0;

  Serial.print("Trash distance: ");
  Serial.print(distance, 1);
  Serial.print(" cm | Fill: ");
  Serial.print(fillPercent, 0);
  Serial.println("%");

  if (alert) {
    Serial.println("ALERT: BIN NEEDS SERVICING!");
  }

  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, (int)fillPercent);
    Blynk.virtualWrite(V1, distance);
    Blynk.virtualWrite(V2, alert);
  }
}

// Read person sensor and control lid
void updatePersonDetection() {
  float distance =
      measureDistance(PERSON_TRIG, PERSON_ECHO);

  if (distance < 0) {
    Serial.println("Person sensor: No valid reading");
    return;
  }

  Serial.print("Person distance: ");
  Serial.print(distance, 1);
  Serial.println(" cm");

  if (Blynk.connected()) {
    Blynk.virtualWrite(V3, distance);
  }

  bool personDetected = (distance <= DETECTION_DISTANCE);

  // Open once when a person enters the detection zone
  if (personDetected && !personPreviouslyDetected && !lidOpen) {
    servo.write(OPEN_ANGLE);
    lidOpen = true;
    lidOpenedAt = millis();

    Serial.println("PERSON DETECTED - LID OPENED");
  }

  // Rearm only after the person leaves the detection zone
  if (!personDetected) {
    personPreviouslyDetected = false;
  } else {
    personPreviouslyDetected = true;
  }
}

// Close lid after 3.5 seconds without blocking Blynk
void controlLid() {
  if (lidOpen && millis() - lidOpenedAt >= OPEN_TIME) {
    servo.write(CLOSE_ANGLE);
    lidOpen = false;

    Serial.println("LID CLOSED");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(PERSON_TRIG, OUTPUT);
  pinMode(PERSON_ECHO, INPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(PERSON_TRIG, LOW);

  servo.attach(SERVO_PIN);
  servo.write(CLOSE_ANGLE);

  Serial.println();
  Serial.println("SMART DUSTBIN STARTING");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Alternate sensor measurements to reduce interference
  timer.setInterval(1000L, updateTrashLevel);
  timer.setInterval(1200L, updatePersonDetection);
  timer.setInterval(100L, controlLid);

  Serial.println("SMART DUSTBIN READY");
}

void loop() {
  Blynk.run();
  timer.run();
}
