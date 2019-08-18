// EQ Simple Focuser : copy
// Arduino code in controlling an absolute focuser
// using a stepper motor
//https://github.com/adafruit/AccelStepper

#define DHT22_ 1
#define MSmode 1
#define D_OLED 1

#include <AccelStepper.h>
  #ifdef DHT22_
   //#include <dht.h>
   #include <DHT.h>
  #endif

// motor pins
#define motorInterfaceType 1
#define DIR 4
#define STEP 3
  #ifdef MSmode
    #define MS0 15
    #define MS1 12
    #define MS2 13
    short stepmode = 1;
  #endif

// for the temperature and hubmidity sensor
  #ifdef DHT22_
   #define DHT22_PIN 2
   #define DHTTYPE DHT22
   DHT dht(DHT22_PIN,DHTTYPE);
  #endif

// Declaration needed for the AccelStepper Library
//AccelStepper stepper1(AccelStepper::FULL4WIRE, motorPin5, motorPin7, motorPin6, motorPin8);
  AccelStepper stepper(motorInterfaceType, STEP, DIR);

// for command purposes
String inputString = "";
int step = 0;
int backlashStep = 0;
String lastDirection = "NONE"; //"OUTWARD"
String currentDirection = "NONE";

// for pin values
int ccwPin = 7;
int cwPin = 6;
int ccwVal = 0;
int cwVal = 0;

// for the variable resistor
int potpin = 0;
int variableResistorValue = 10;
int oldVariableResistorValue = 10;

// for manual control
long toPosition;
bool positionReported = false;

// temperature and humidity sensor
  #ifdef DHT22
   //dht DHT;
   int chkSensor;
  #endif

void setup() {
  Serial.begin(115200);
  Serial.println("EQFOCUSER_STEPPER#");
  
  stepper.setMaxSpeed(100.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(100);

  inputString.reserve(200);

  pinMode(ccwPin, INPUT_PULLUP);
  pinMode(cwPin, INPUT_PULLUP);
}


void loop() {
  variableResistorValue = analogRead(potpin);

  cwVal = digitalRead(cwPin);
  ccwVal = digitalRead(ccwPin);

  if (ccwVal == LOW || cwVal == LOW) {
    //    if (oldVariableResistorValue < variableResistorValue - 5 || oldVariableResistorValue > variableResistorValue + 5 ){
    //      // set the max acceleration
    //      stepper.setAcceleration(variableResistorValue / 102.4);
    //      oldVariableResistorValue = variableResistorValue;
    //      Serial.print("Changing Value of acceleration");
    //      Serial.println(oldVariableResistorValue);
    //    }
    // the PULLUP Pins are pressed
    Serial.print("MOVING:");
    if (ccwVal == LOW) {
      toPosition = stepper.currentPosition() - variableResistorValue / 10;
      Serial.print(toPosition);
      applyBacklashStep(toPosition, lastDirection, "INWARD");
//      stepper.moveTo(toPosition);
      lastDirection = "INWARD";
      
    }
    if (cwVal == LOW) {
      toPosition = stepper.currentPosition() + variableResistorValue / 10;
      Serial.print(toPosition);
      applyBacklashStep(toPosition, lastDirection, "OUTWARD");
//      stepper.moveTo(toPosition);
      lastDirection = "OUTWARD";
    }
    Serial.println("#");
    stepper.run();

    if (toPosition == stepper.currentPosition()) {
      // the stepper is not really moving here so just report the posiiton
      reportPosition();
    }
  }
  else {
    if (stepper.distanceToGo() != 0) {
      // let the stepper finish the movement
      stepper.run();
      positionReported = false;
    }
    if (stepper.distanceToGo() == 0 && !positionReported) {
      reportPosition();
      delay(500);
      positionReported = true;
    }
  }
}

void reportPosition() {
  Serial.print("POSITION:");
  Serial.print(stepper.currentPosition());
  Serial.println("#");
}

// test if direction is the same, otherwise apply backlash step
// this method is only applicable for manual focusing changes
void applyBacklashStep(int toPosition, String lastDirection, String currentDirection){
  if (lastDirection == currentDirection){
    // no backlash
    stepper.moveTo(toPosition);
  }
  else {
    // apply backlash
    stepper.moveTo(toPosition + backlashStep);
    stepper.setCurrentPosition(toPosition - backlashStep);
  }
}

/**
* process the command we recieved from the client
* command format is <Letter><Space><Integer>
* i.e. A 500 ---- Fast Rewind with 500 steps
*/
void serialCommand(String commandString) {
  char _command = commandString.charAt(0);
  int _step = commandString.substring(2).toInt();
  String _answer = "";
  int _currentPosition = stepper.currentPosition();
  int _newPosition = _currentPosition;
  int _backlashStep;


  
  switch (_command) {
  case 'A':  // FAST REVERSE "<<"
  case 'a': _newPosition = _currentPosition - ( _step * 2 );
            currentDirection = "INWARD";
    break;
  case 'B':  // REVERSE "<"
  case 'b': _newPosition = _currentPosition - _step;
            currentDirection = "INWARD";
    break;
  case 'C':  // FORWARD ">"
  case 'c': _newPosition = _currentPosition + _step;
            currentDirection = "OUTWARD";
    break;
  case 'D':  // FAST FORWARD ">>"
  case 'd': _newPosition = _currentPosition + ( _step * 2 );
            currentDirection = "OUTWARD";
    break;
  case 'E':  // MOVE TO POSITION
  case 'e': _newPosition = _step;
    break;
  case 'F':  // GET CURRENT POSITION
  case 'f': _answer += _currentPosition;
    break;
  case 'G':  // SET POSITION TO 0
  case 'g': _newPosition = 0;
    _currentPosition = 0;
    stepper.setCurrentPosition(0);
    break;
  case 'H':  // SET ACCELERATION
  case 'h': _newPosition = _currentPosition; // non move command
    stepper.setAcceleration(_step);
    _answer += "SET-ACCELERATION:";
    _answer += _step;
    break;
  case 'I':  // SET SPEED
    _newPosition = _currentPosition; // non move command
    stepper.setSpeed(_step);
    _answer += "SET-SPEED:";
    _answer += _step;
    break;
  case 'i':  // GET SPEED
    _newPosition = _currentPosition; // non move command
    _answer += "GET-SPEED:";
    _answer += stepper.speed();
    break;
  case 'J':  // SET MAX SPEED
  case 'j':  _newPosition = _currentPosition; // non move command
    stepper.setMaxSpeed(_step);
    _answer += "SET-MAXSPEED:";
    _answer += _step;
    break;
      #ifdef DHT22
        case 'k': // GET TEMPERATURE / HUMIDITY
          _newPosition = _currentPosition; // non move command
          humidityTemperatureReport();
          break;
      #endif
  case 'L' :
  case 'l' :
    backlashStep = _step;
    _answer += "SET-BACKLASHSTEP:";
    _answer += _step;
    break;
      #ifdef MSmode
        case 'M' : // SET MICROSTEPPING
        case 'm' : 
          if(1<=_step && _step<=4) stepmode = _step;
          else Serial.println("Microstepping mode should be from 1 to 4");
          setstep();
      #endif
  case 'X':  // GET STATUS - may not be needed
  case 'x':
    stepper.stop();
    break;
  case 'Z':  // IDENTIFY
  case 'z':  _answer += "EQFOCUSER_STEPPER";
    break;
  default:
    _answer += "EQFOCUSER_STEPPER";
    break;
  }

  if (_newPosition != _currentPosition) {
    if (lastDirection != "NONE"){
      if (stepper.currentPosition() < _newPosition){
        // moving forward
        currentDirection == "OUTWARD";
      }
      if (stepper.currentPosition() > _newPosition){
        // moving backward
        currentDirection == "INWARD";
      }
      Serial.print(lastDirection);Serial.print("===");Serial.println(currentDirection);
      if (lastDirection != currentDirection){
        if (currentDirection == "OUTWARD") _newPosition = _newPosition + backlashStep;
        if (currentDirection == "INWARD") _newPosition = _newPosition - backlashStep;
      }
      else {
        _backlashStep = 0;
      }
    }
  
    // a move command was issued
    Serial.print("MOVING:");
    Serial.print(_newPosition);
    Serial.println("#");
    //    stepper.runToNewPosition(_newPosition);  // this will block the execution
    stepper.moveTo(_newPosition);
    stepper.runSpeedToPosition();
    lastDirection = currentDirection;
    _answer += "POSITION:";
    _answer += stepper.currentPosition();
  }


  Serial.print(_answer);
  Serial.println("#");
}

/**
* handler for the serial communicationes
* calls the SerialCommand whenever a new command is received
*/
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      serialCommand(inputString);
      inputString = "";
    }
  }
}

/**
* for DHT routine
*/
#ifdef DHT22_
  void humidityTemperatureReport() {
    chkSensor = digitalRead(DHT22_PIN);
    switch (chkSensor) {
    case 1:
      Serial.print("TEMPERATURE:");
      Serial.print(String(dht.readTemperature(),1));
      Serial.println("#");
      delay(50);
      Serial.print("HUMIDITY:");
      Serial.print(String(dht.readHumidity(),1));
      Serial.println("#");
      delay(50);
      break;
    case 0:
      Serial.print("TEMPERATURE:");
      Serial.print("CHECKSUMERROR");
      Serial.println("#");
      Serial.print("HUMIDITY:");
      Serial.print("CHECKSUMERROR");
      Serial.println("#");
      break;
    default:
      Serial.print("TEMPERATURE:");
      Serial.print("UNKNOWNERROR");
      Serial.println("#");
      Serial.print("HUMIDITY:");
      Serial.print("UNKNOWNERROR");
      Serial.println("#");
      break;
    }
  }
#endif

