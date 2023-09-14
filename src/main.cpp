#include <Arduino.h>
#include <Servo.h>

#define SERVO_PWM 9 // AKA X end stop

#define EN_PIN 8
#define STEP_XPIN 2 // X.STEP
#define DIR_XPIN 5  // X.DIR
// #define STEP_YPIN 3 // Y.STEP
// #define DIR_YPIN 6  // Y.DIR
// #define STEP_ZPIN 4 // Z.STEP
// #define DIR_ZPIN 7  // Z.DIR

#define DC_FORWARD 10
#define DC_BACKWARD 11

#define stepsPerRev 200
#define pulseWidthMicros 100
#define millisBtwnSteps 1000

byte state = 2;

int stepPin = STEP_XPIN;
int dirPin = DIR_XPIN;

Servo servo;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  servo.attach(SERVO_PWM);
  pinMode(EN_PIN, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(DC_FORWARD, OUTPUT);
  pinMode(DC_BACKWARD, OUTPUT);

  digitalWrite(EN_PIN, LOW);
  for (size_t i = 0; i < 10; i++) {
    Serial.println("asdf");
  }
}

void loop() {
  switch (state++) {
  case 0: {
    servo.write(0);
    delay(2000);
    servo.write(180);
    delay(2000);
  } break;

  case 1: {
    Serial.println(F("Running clockwise"));
    digitalWrite(dirPin, HIGH);

    // Makes 200 pulses for making one full cycle rotation
    for (int i = 0; i < 4 * stepsPerRev; ++i) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(pulseWidthMicros);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(millisBtwnSteps);
    };
    delay(1000); // One second delay

    Serial.println(F("Running counter-clockwise"));
    digitalWrite(dirPin, LOW); // Changes the rotations direction
    // Makes 400 pulses for making two full cycle rotation
    for (size_t i = 0; i < 4 * stepsPerRev; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(pulseWidthMicros);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(millisBtwnSteps);
    }
    delay(1000);
  } break;

  case 2: {
    for (int i = 0; i < 3; i++) {
      // DO NOT INCREASE, the 6v motor is powered by a 12v batt
      analogWrite(DC_FORWARD, 50);
      analogWrite(DC_BACKWARD, 0);
      delay(1000);

      // DO NOT INCREASE, the 6v motor is powered by a 12v batt
      analogWrite(DC_FORWARD, 0);
      analogWrite(DC_BACKWARD, 50);
      delay(3000);
    }
    analogWrite(DC_FORWARD, 0);
    analogWrite(DC_BACKWARD, 0);
  } break;
  default:

    break;
  }
  state %= 3;
}

void stepperDemo() {}
