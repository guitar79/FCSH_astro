// basic test program for DRV8825
// rotates motor one full rotation then the reverse direction with 2s delay between moves

#include <Arduino.h>
#include <myAFMotor.h>                // needed for stepper motor and L293D shield

// Stepper Motor stuff - YOU NEED TO USE THE CORRECT ONES FOR YOUR STEPPER MOTOR
// Motor port on the L293D shield to use
#define Motor_Port 2      // use M3 and M4 as its easier to connect
// you need to change the above line to reflect which port you are using on the L293D shield
// it is either 1 (M2/M1) or 2 (M3/M4)

// stepper motor steps per full revolution using full steps
const int stepsPerRevolution = 1028;       // NEMA17-PG5 motor
// you need to change the above line to reflect your stepper motor, examples below
// const int stepsPerRevolution = 2048;    // 24BBYJ-48 motor
// const int stepsPerRevolution = 1028;    // NEMA17-PG5 motor
// const int stepsPerRevolution = 200;     // NEMA17 motor

// motor speeds in RPM
int motorSpeedSlow = 1;
int motorSpeedMed = 10;
int motorSpeedFast = 200;
int motorSpeed = motorSpeedMed;
int mscmd = 0;

AF_Stepper mystepper(stepsPerRevolution, Motor_Port);

// disable the stepper motor outputs - coil power off
void clearOutput() {
    mystepper.release();
}

// Move stepper anticlockwise
void anticlockwise( int stepmode) {
   // (!ReverseDirection)? analogWrite( gledOUT, 255) : analogWrite( bledIN, 255);
   // mystepper.step(1, (!ReverseDirection)? BACKWARD : FORWARD, (halfstep) ? INTERLEAVE : DOUBLE );
   // (!ReverseDirection)? analogWrite( gledOUT, 0 ): analogWrite( bledIN, 0);
    if ( stepmode == 2 )
      mystepper.step(1, BACKWARD, INTERLEAVE); // one half step anticlockwise
    else
      mystepper.step(1, BACKWARD, DOUBLE);     // one step anticlockwise
}

// Move stepper clockwise
void clockwise( int stepmode) {
  // (!ReverseDirection)? analogWrite( bledIN, 255) : analogWrite( gledOUT, 255);
  // mystepper.step(1, (!ReverseDirection)? BACKWARD : FORWARD, (halfstep) ? INTERLEAVE : DOUBLE );
  // (!ReverseDirection)? analogWrite( bledIN, 0 ) : analogWrite( gledOUT, 0);
    if ( stepmode == 2 )
      mystepper.step(1, FORWARD, INTERLEAVE);   // one half step clockwise
    else
      mystepper.step(1, FORWARD, DOUBLE);       // one step clockwise
}

// set the microstepping mode
void setstepmode() {
  // set within processcmd
  // handled with anticlockwise() and clockwise()
  // ignore
}

// Setup
void setup() {
  // initialize serial for ASCOM
  Serial.begin(9600);

  motorSpeed = motorSpeedMed;
  mystepper.setSpeed(motorSpeed);
}

// Main Loop
void loop() {
  // enable full steps
  Serial.println("Clockwise 200 steps - Full steps");
  for ( int lp = 0; lp < 200; lp++ )\
    clockwise(1);
  Serial.println("Wait 2s");
  delay(2000);
  Serial.println("Anti-Clockwise 200 steps - Full steps");
  for ( int lp = 0; lp < 200; lp++ )\
    anticlockwise(1);
  Serial.println("Wait 2s");
  delay(2000);
  // enable half-steps
  Serial.println("Clockwise 200 steps - Half steps");
  for ( int lp = 0; lp < 200; lp++ )\
    clockwise(2);
  Serial.println("Wait 2s");
  delay(2000);
  Serial.println("Anti-Clockwise 200 steps - Half steps");
  for ( int lp = 0; lp < 200; lp++ )\
    anticlockwise(2);
  Serial.println("Wait 2s");
  delay(2000);
}


