// EQ Simple Focuser
// Arduino code in controlling an absolute focuser
// using a stepper motor
//https://blog.naver.com/chandong83/220875868466
//https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a9d917f014317fb9d3b5dc14e66f6c689
#include <AccelStepper.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <DRV8825.h>
#include <BasicStepperDriver.h>

//BaudRate
#define baudrate 115200

// motor pins
#define DIR 4
#define STEP 3
#define MS0 15
#define MS1 12
#define MS2 13
#define MOTOR_STEPS 200

// for the temperature and hubmidity sensor
#define DHT22_PIN 2
// Declaration needed for the DRV8825 Library
DRV8825 stepper(MOTOR_STEPS, DIR,STEP,MS0,MS1,MS2);
//AccelStepper stepper1(AccelStepper::FULL4WIRE, motorPin5, motorPin7, motorPin6, motorPin8);

// for command purposes
String inputString = "";
int step = 0;
int backlashStep = 0;
String lastDirection = "NONE"; //"OUTWARD"
String currentDirection = "NONE";

// for pin values : cw(clockwise but 시계반대방향), ccw(counterclockwise but 시계방향) : 정방향, 역방향
//https://m.blog.naver.com/PostView.nhn?blogId=glooory&logNo=220926317204&proxyReferer=https%3A%2F%2Fwww.google.com%2F


// for the variable resistor : 가변저항
#define potpin 
#ifdef potpin
int variableResistorValue = 10;
int oldVariableResistorValue = 10;
#define ccwPin 7
#define cwPin 6
int ccwVal = 0;
int cwVal = 0;
#endif

// for manual control
long toPosition;
bool positionReported = false;
float Accel;
float Speed;
float nowRPM;
unsigned long currentPosition = 25000;
double distance = 0;


// temperature and humidity sensor
DHT_Unified dht(DHT22_PIN,DHT22);
int delayMS;

void setup() 
    {
        Serial.begin(baudrate);
        Serial.println("EQFOCUSER_STEPPER#");
      
        Accel = 100.0;
        Speed = 100; //step per sec
        stepper.setRPM(60*Speed/MOTOR_STEPS);
        nowRPM = 60*Speed/MOTOR_STEPS;
        
        inputString.reserve(200);
      
        pinMode(ccwPin, INPUT_PULLUP);
        pinMode(cwPin, INPUT_PULLUP);

        dht.begin();
        sensor_t sensor;
        dht.humidity().getSensor(&sensor);
        
        delayMS = sensor.min_delay/1000;
    }


void loop() 
    {
//      variableResistorValue = analogRead(potpin);
//
//      
//        cwVal = digitalRead(cwPin);
//        ccwVal = digitalRead(ccwPin);
//
//        if (ccwVal == LOW || cwVal == LOW) 
//            {
//                    if (oldVariableResistorValue < variableResistorValue - 5 || oldVariableResistorValue > variableResistorValue + 5 )
//                    {
//                      // set the max acceleration
//                      Accel = variableResistorValue / 102.4;
//                      oldVariableResistorValue = variableResistorValue;
//                      Serial.print("Changing Value of acceleration");
//                      Serial.println(oldVariableResistorValue);
//                    }
//              
//                // the PULLUP Pins are pressed
//                Serial.print("MOVING:");
//                
//                if (ccwVal == LOW) 
//                    {
//                        toPosition = currentPosition - variableResistorValue / 10;
//                        Serial.print(toPosition);
//                        applyBacklashStep(toPosition, lastDirection, "INWARD");
//                        //      stepper1.moveTo(toPosition);
//                        lastDirection = "INWARD";
//
//                    }
//                if (cwVal == LOW) 
//                    {
//                        toPosition = currentPosition + variableResistorValue / 10;
//                        Serial.print(toPosition);
//                        applyBacklashStep(toPosition, lastDirection, "OUTWARD");
//                        //      stepper1.moveTo(toPosition);
//                        lastDirection = "OUTWARD";
//                    }
//                    
//                        Serial.println("#");
//      
//                       // stepper1.run();
//                    
//                        if (toPosition == currentPosition) 
//                            {
//                                // the stepper is not really moving here so just report the posiiton
//                                reportPosition();
//                            }
//                }
//            else 
//                {
                    if (distance != 0) 
                        {
                          // let the stepper finish the movement
                          positionReported = false;
                        }
                    if (distance == 0 && !positionReported) 
                        {
                          reportPosition();
                          delay(500);
                          positionReported = true;
                        }
//                }
    }

void moveTo(long toPosition)
{
  distance = toPosition - currentPosition;

  stepper.rotate(distance*1.8);
  stepper.setRPM(nowRPM);
  
  distance = 0;
}

// test if direction is the same, otherwise apply backlash step
// this method is only applicable for manual focusing changes
void applyBacklashStep(int toPosition, String lastDirection, String currentDirection) 
    {
        if (lastDirection == currentDirection) 
            {
                // no backlash
                moveTo(toPosition);
            }
        else 
            {
                // apply backlash
                moveTo(toPosition + backlashStep);
                currentPosition = toPosition - backlashStep;
            }
    }
