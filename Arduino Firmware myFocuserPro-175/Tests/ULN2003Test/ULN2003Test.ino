// basic test program for ULN2003
// rotates motor one full rotation then the reverse direction with 2s delay between moves

#include <Arduino.h>
#include <myStepper.h>                  // needed for stepper motor

// initialize the stepper library on pins 4 (IN1), 5 (IN2), 6 (IN3), 7 (IN4)
Stepper mystepper(2048, 4, 6, 7, 5);

// Setup
void setup() {
  // initialize serial for ASCOM
  Serial.begin(9600);

  mystepper.setSpeed(2); // medium speed

}

// Main Loop
void loop() {
    mystepper.step(2048);        // step the motor one step anticlockwise
    delay(2000);
    mystepper.step(-2048);        // step the motor one step anticlockwise
    delay(2000);
    
}
