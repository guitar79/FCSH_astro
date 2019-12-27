#include <EEPROM.h>
 
int value = 0;    //EEPROM value
void setup(){

  delay(1000);
  Serial.begin(115200);
  eepRead();
  Serial.println(value);
  delay(3000);
}

void loop(){
  eepWrite(value);
}

void eepRead()
{
 int NoA = EEPROM.read(0);
 int v = 0;

 v = EEPROM.read(1);
 value = v;
}

void eepWrite(int value_)
{
  int v = value_;
  
  EEPROM.write(1,v);
  
  Serial.print("ROM value : ");
  Serial.println(v);
}

void serialCommand(String commandString)
{
  value = commandString.toInt();
  Serial.println(value);
}

String inputString;
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
