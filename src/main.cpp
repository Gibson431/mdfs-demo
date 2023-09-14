#include <Arduino.h>
#include <Servo.h>
#include <Stepper.h>

#define SERVO_PWM 9 // AKA X end stop

#define EN_PIN 8
#define STEP_XPIN 2 // X.STEP
#define DIR_XPIN 5  // X.DIR
#define STEP_YPIN 3 // Y.STEP
#define DIR_YPIN 6  // Y.DIR
#define STEP_ZPIN 4 // Z.STEP
#define DIR_ZPIN 7  // Z.DIR

#define DC_FORWARD 10  // Y+
#define DC_BACKWARD 11 // Z+

#define stepsPerRev 200
#define pulseWidthMicros 100
#define millisBtwnSteps 1000

byte state = 3;

int stepPin = STEP_XPIN;
int dirPin = DIR_XPIN;

Servo servo;
Stepper stepper(stepsPerRev, DIR_YPIN, STEP_YPIN, DIR_ZPIN, STEP_ZPIN);

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);

  pinMode(EN_PIN, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(STEP_XPIN, OUTPUT);
  pinMode(DIR_XPIN, OUTPUT);
  pinMode(STEP_YPIN, OUTPUT);
  pinMode(DIR_YPIN, OUTPUT);
  pinMode(STEP_ZPIN, OUTPUT);
  pinMode(DIR_ZPIN, OUTPUT);

  pinMode(DC_FORWARD, OUTPUT);
  pinMode(DC_BACKWARD, OUTPUT);

  servo.attach(SERVO_PWM);
  stepper.setSpeed(30);

  digitalWrite(EN_PIN, LOW);
}

void loop() {
  switch (state++) {
  case 0: {
    for (size_t i = 0; i < 3; i++) {
      servo.write(0);
      delay(2000);
      servo.write(180);
      delay(2000);
    }
  } break;

  case 1: {
    Serial.println(F("Running clockwise"));
    digitalWrite(dirPin, HIGH);

    for (size_t j = 0; j < 3; j++) {
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
    }
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

  case 3: {
    for (int i = 0; i < 3; i++) {
      stepper.step(stepsPerRev);
      delay(500);
      stepper.step(-stepsPerRev);
      delay(500);
    }
  } break;
  default:

    break;
  }
  state %= 4;
}
