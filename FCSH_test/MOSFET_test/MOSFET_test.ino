
int Input = 5; //Switch
int Output = 6; //Read
bool Switch = false;

void setup() {
  pinMode(Input, OUTPUT);
  pinMode(Output, INPUT);
  Serial.begin(115200);
  digitalWrite(Input,LOW);
}

// the loop function runs over and over again forever
void loop() {
  if(digitalRead(Output)) Serial.println("1");
  else Serial.println("0");
}

void serialCommand(String commandString)
{
  Switch = !Switch;
  if(Switch) digitalWrite(Input,HIGH);
  else digitalWrite(Input,LOW);
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
