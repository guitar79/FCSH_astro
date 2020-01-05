//https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a608b2395b64ac15451d16d0371fe13ce

#include "Board.h"

// for menu control
  short subm = 0;
  short menu = 1;
  short power = 1;
  short rm = 0;

// microstepping
  short stepmode = 1;

// controling PWM (0~100)
  int PWM_value = 0;
  bool currentstate = false;
  unsigned long currentMillis = 0;
  unsigned long previousMillis = 0;
  unsigned long delayMillis = 0;

  bool PCMODE = false;
  int EEPcurrentPosition = 0;
  
#include <AccelStepper.h>
#include <DHT.h>
#include <Servo.h>

// for Stepper motor control
  AccelStepper stepper(motorInterfaceType, STEP, DIR);
  
// for the temperature and hubmidity sensor
#define DHTTYPE DHT22
   DHT dht(DHT22_PIN,DHTTYPE);
   int chkSensor;
   String Temperature;
   String Humidity;

// for Servor motor control
  Servo servo;
  bool BMask = false;
  
  String inputString = "";

void setup() {
  
  Serial.begin(115200);
  Serial.println("GStouch#");
  U8G_start();
  Servo_start();
  
  eepRead();
  stepper.setCurrentPosition(EEPcurrentPosition);
  
  stepper.setMaxSpeed(2000.0);
  stepper.setAcceleration(300.0);
  stepper.setSpeed(100);

  inputString.reserve(200);

  pinset();
  digitalWrite(PWM,LOW);
}


void loop() {
/*
  if(stepper.distanceToGo() == 0 || subm!=2)
  {
    Temperature = String(dht.readTemperature(),1);
    Humidity = String(dht.readHumidity(),1);
    humidityTemperatureReport();
    buttonRead();
    draw();
  }
*/
  if(delayMillis >0 && delayMillis <500)
  {
    Serial.println(delayMillis);
    currentMillis = millis();
    if(currentMillis > previousMillis + delayMillis)
    {
      previousMillis = currentMillis;
      currentstate = !currentstate;
      delayMillis = 500-delayMillis;
    }
    if(currentstate == true) digitalWrite(PWM,HIGH);
    else digitalWrite(PWM,LOW);
  }//test OK
  
}

void reportPosition() {
  Serial.print("POSITION:");
  Serial.print(stepper.currentPosition());
  Serial.println("#");
}


/**
* process the command we recieved from the client
* command format is <Letter><Space><Integer>
* i.e. A 500 ---- Fast Rewind with 500 steps
*/
void serialCommand(String commandString) {
  char _command = commandString.charAt(0);
  int _value = commandString.substring(2).toInt();
  String _answer = "";
  int _currentPosition = stepper.currentPosition();
  int _newPosition = _currentPosition;


  
  switch (_command) {

  case 'A':  // SET PWM
  case 'a': _newPosition = _currentPosition; // non move command
    //PWM test
    delayMillis = 5 * _value;
    previousMillis = millis();
    currentMillis = millis();
    currentstate = true;
    if(delayMillis == 500) digitalWrite(PWM,HIGH);
    break;
    
  case 'B':  // REVERSE "<"
  case 'b': _newPosition = _currentPosition - _value;
    break;
    
  case 'C':  // FORWARD ">"
  case 'c': _newPosition = _currentPosition + _value;
    break;
    
  case 'E':  // MOVE TO POSITION
  case 'e': _newPosition = _value;
    break;
    
  case 'F':  // GET CURRENT POSITION
  case 'f': _answer += _currentPosition;
    break;
    
  case 'G':  // SET CURRENT POSITION
  case 'g': _newPosition = _value;
    _currentPosition = _value;
    stepper.setCurrentPosition(_value);
    break;
    
  case 'H':  // SET ACCELERATION
  case 'h': _newPosition = _currentPosition; // non move command
    stepper.setAcceleration(_value);
    _answer += "SET-ACCELERATION:";
    _answer += _value;
    break;
    
  case 'I':  // SET SPEED
    _newPosition = _currentPosition; // non move command
    stepper.setSpeed(_value);
    _answer += "SET-SPEED:";
    _answer += _value;
    break;
    
  case 'i':  // GET SPEED
    _newPosition = _currentPosition; // non move command
    _answer += "GET-SPEED:";
    _answer += stepper.speed();
    break;
    
  case 'k': // GET TEMPERATURE & HUMIDITY
    _newPosition = _currentPosition; // non move command
    humidityTemperatureReport();
    break;

  case 'N':  // Mask Set
  case 'n': _newPosition = _currentPosition; // non move command
  BMask = !BMask;
  
    servo.attach(ServoPin); // 5도 동시제어
    if(BMask) servo.write(_value);
      else if(!BMask) servo.write(0);
    delay(500);
    servo.detach();
    delay(500);

  case 'X':  // GET STATUS - may not be needed
  case 'x':
    stepper.stop();
    break;
  case 'Z':  // IDENTIFY
  case 'z':  _answer += "GStouch";
  PCMODE = true;
    break;
    
  default:
    _answer += "GStouch";
    break;
  }

  if (_newPosition != _currentPosition) {
        // a move command was issued
    Serial.print("MOVING:");
    Serial.print(_newPosition);
    Serial.println("#");
    //
    stepper.moveTo(_newPosition);
    stepper.runToPosition();
    _answer += "POSITION:";
    _answer += stepper.currentPosition();
  }
  
  eepWrite(_newPosition);
  eepRead();
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

void Servo_start()
{
  servo.attach(ServoPin); // 5도 동시제어
  servo.write(0);
  delay(500);
  servo.detach();
}

