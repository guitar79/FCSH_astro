
int ct=0;
int pt=0;
void motorControl()
{
  int stp = 0;
  switch(power)
  {
   case 1: stp=1; break;
   case 2: stp=10; break;
   case 3: stp=100; break;
   case 4: stp=300; break;
   case 5: stp=1000; break;
  }
  
  switch(subm)
  {
    case 2:
      if (S[0] && rm) rm--; 
      else if(S[1] && !rm) rm++;
      else if(S[2]) subm++;
      else if(S[3]) subm--; 
    break;

    case 3:
      switch(rm)
      {
        case 0: // POSITION
        
          if(!now[0]||!now[1])
          {
            int _nextPosition = stepper.currentPosition();
            if(!now[0]) _nextPosition += stp;
            else _nextPosition -= stp;
              if(_nextPosition != stepper.currentPosition())
              {
                stepper.moveTo(_nextPosition);
                stepper.runToPosition();
              }
          }
          else if(S[3]) subm--;
        break;
        
        case 1: // POWER
          if(S[0] && power<5) power++;
          else if(S[1] && power>1) power--;
          else if(S[3]) 
          {
            subm--;
              switch(power)
              {
                case 1: stp=1; break;
                case 2: stp=10; break;
                case 3: stp=100; break;
                case 4: stp=300; break;
                case 5: stp=1000; break;
              }
          }
          stepper.setSpeed(300*stp);
        break;
      }
    break;
  }
}

void MSmodeControl()
{
  switch(subm)
  {
    case 2:
      if (S[0] && rm) rm--; 
      else if(S[1] && rm != 2) rm++;
      else if(S[2]) subm++;
      else if(S[3]) subm--; 
    break;

    case 3:
      switch(rm)
      {
        case 0:
          if(S[0] && stepmode<4) stepmode++;
          else if(S[1] && stepmode>1) stepmode--;
          else if(S[3]) subm--;
        break;

        case 1:
            if(S[0] && _resetPosition < 500000 && rm==1) _resetPosition = (_resetPosition+100)/100*100;
            else if(S[1] && _resetPosition > 0 && rm==1) _resetPosition = (_resetPosition-100)/100*100;
            else if(S[3]) subm--;
        break;

        case 2:
            if(S[2])
            {
              rm=0; subm=0;
              stepper.setCurrentPosition(_resetPosition); // reset position setting
              _resetPosition = 0;
      
              setstep(); //microstepping setting
            }
            else if (S[3]) subm--;
      }
  }

}

//Main_Control로 이동
void Servo_Relay_Control()
{
  switch(subm)
  {
    case 2:
      if (S[0] && rm) rm--; 
      else if(S[1] && rm!=4) rm++;
      else if(S[2]) subm++;
      else if(S[3]) subm--; 
    break;

    case 3:
      switch(rm)
      {
        case 0:
            if(S[0])
            {
             BMask1 = true;
             servo.attach(ServoPin1); // 5도 동시제어
             servo.write(150);
             delay(500);
             servo.detach();
             delay(500);
            }
            else if(S[1])
            {
             BMask1 = false;
             servo.attach(ServoPin1); // 5도 동시제어
             servo.write(30);
             delay(500);
             servo.detach();
             delay(500);
            }
          else if(S[3]) subm--;
        break;

        case 1:
          if(S[0] && !Relay1) 
            {Relay1 = !Relay1; digitalWrite(RelayPin1,HIGH);}
          else if(S[1] && Relay1) 
            {Relay1 = !Relay1; digitalWrite(RelayPin1,LOW);}
          else if(S[3]) subm--;
        break;

        case 2:
          if(S[0] && !Relay2) 
            {Relay2 = !Relay2; digitalWrite(RelayPin2,HIGH);}
          else if(S[1] && Relay2) 
            {Relay2 = !Relay2; digitalWrite(RelayPin2,LOW);}
          else if(S[3]) subm--;
        break;

        case 3:
          if(S[0] && !Relay3) 
            {Relay3 = !Relay3; digitalWrite(RelayPin3,HIGH);}
          else if(S[1] && Relay3) 
            {Relay3 = !Relay3; digitalWrite(RelayPin3,LOW);}
          else if(S[3]) subm--;
        break;

        case 4:
          if(S[0] && !Relay4) 
            {Relay4 = !Relay4; digitalWrite(RelayPin4,HIGH);}
          else if(S[1] && Relay4) 
            {Relay4 = !Relay4; digitalWrite(RelayPin4,LOW);}
          else if(S[3]) subm--;
        break;
      }
  }


}

