// basic test program for ULN2003
// rotates motor one full rotation then the reverse direction with 2s delay between moves

#include <Arduino.h>
#include <myStepper.h>

const int stepsPerRevolution = 200;  // NEMA17 change this to fit the number of steps per revolution
// initialize the stepper library on pins 5 through 8:
Stepper myStepper(stepsPerRevolution, 5, 6, 7, 8);

// Stepper Motor stuff, control pins for L298N board
#define IN1     5  // input lines for stepping
#define IN2     6
#define IN3     7
#define IN4     8

// stepontime - time in microseconds that coil power is ON for one step, board requires 2us pulse
int stepontime = 50;      // time in microseconds that coil power is ON for one step, board requires 2us pulse
int motorSpeedSlow = 10;   // 10RPM
int motorSpeedMed = 30;    // 30RPM
int motorSpeedFast = 50;   // 50RPM 
const int    motorSpeedDefault = motorSpeedMed;
int          motorSpeed = motorSpeedDefault;

// disable the stepper motor outputs - coil power off
void clearOutput() {
    digitalWrite( IN1, false);
    digitalWrite( IN2, false);
    digitalWrite( IN3, false);
    digitalWrite( IN4, false);
}

// Move stepper anticlockwise
void anticlockwise() {
    myStepper.step(1);
    delayMicroseconds(stepontime);
}

// Move stepper clockwise
void clockwise() {
    myStepper.step(-1);
    delayMicroseconds(stepontime);
}

// Setup
void setup() {
  // initialize serial for ASCOM
  Serial.begin(9600);
  
  // set the speed to default 30rpm:
  myStepper.setSpeed(motorSpeed);

  // setup INx step lines as outputs, set low so motor does not move
  pinMode( IN1, OUTPUT );
  pinMode( IN2, OUTPUT );
  pinMode( IN3, OUTPUT );
  pinMode( IN4, OUTPUT );
  digitalWrite( IN1, false);
  digitalWrite( IN2, false);
  digitalWrite( IN3, false);
  digitalWrite( IN4, false);
}

// Main Loop
void loop() {
  // full steps only
  Serial.println("Clockwise 200 steps - Full steps");
  for ( int lp = 0; lp < 200; lp++ )\
    clockwise();
  Serial.println("Wait 2s");
  delay(2000);
  Serial.println("Anti-Clockwise 200 steps - Full steps");
  for ( int lp = 0; lp < 200; lp++ )\
    anticlockwise();
  Serial.println("Wait 2s");
  delay(2000);
}


