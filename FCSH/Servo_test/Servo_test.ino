#include <Servo.h>
 
Servo myservo;    // create servo object to control a servo
int pos = 0;    // variable to store the servo position

void setup(){
  myservo.attach(3);    // attaches the servo on pin 6 to the servo object
  myservo.write(90);
  delay(1000);
  Serial.begin(115200);
}

void loop(){

}
void serialCommand(String commandString)
{
  pos = commandString.toInt();
  myservo.write(pos);
  Serial.print("Angle to: ");
  Serial.println(pos);
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
