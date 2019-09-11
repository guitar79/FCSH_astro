
#define DHT22_ 1
#define MSmode 1
#define S_OLED 

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
int ccwPin = 0;
int cwPin = 0;
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
   String Temperature;
   String Humidity;
  #endif
  

void setup() {
  Serial.begin(115200);
  Serial.println("GStouch#");
  
  stepper.setMaxSpeed(100.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(100);

  inputString.reserve(200);

  pinMode(ccwPin, INPUT_PULLUP);
  pinMode(cwPin, INPUT_PULLUP);

}


void loop() {

  #ifdef DHT22_
    Temperature = String(dht.readTemperature(),1);
    Humidity = String(dht.readHumidity(),1);
  #endif
  
  #ifdef S_OLED
    U8G_startFMenu();
  #endif
  
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
    break;
  case 'B':  // REVERSE "<"
  case 'b': _newPosition = _currentPosition - _step;
            currentDirection = "INWARD";
    break;
  case 'C':  // FORWARD ">"
  case 'c': _newPosition = _currentPosition + _step;
            currentDirection = "OUTWARD";
    break;
  case 'E':  // MOVE TO POSITION
  case 'e': _newPosition = _step;
    break;
  case 'F':  // GET CURRENT POSITION
  case 'f': _answer += _currentPosition;
    break;
  case 'G':  // SET POSITION TO _step
  case 'g': _newPosition = _step;
    _currentPosition = _step;
    stepper.setCurrentPosition(_step);
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
          else Serial.println("stepmode should be from 1 to 4");
          setstep();
      #endif
  case 'X':  // GET STATUS - may not be needed
  case 'x':
    stepper.stop();
    break;
  case 'Z':  // IDENTIFY
  case 'z':  _answer += "GStouch";
    break;
  default:
    _answer += "GStouch";
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
    stepper.runSpeed();
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
    Temperature = String(dht.readTemperature(),1);
    Humidity = String(dht.readHumidity(),1);
    switch (chkSensor) {
    case 1:
      Serial.print("TEMPERATURE:");
      Serial.print(Temperature);
      Serial.println("#");
      delay(50);
      Serial.print("HUMIDITY:");
      Serial.print(Humidity);
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

