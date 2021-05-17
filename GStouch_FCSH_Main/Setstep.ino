void setstep()
  {
    switch(stepmode)
    {
      case 1:
      digitalWrite(MS0,LOW);
      digitalWrite(MS1,LOW);
      digitalWrite(MS2,LOW);
        break;  
        
      case 2:
      digitalWrite(MS0,HIGH);
      digitalWrite(MS1,LOW);
      digitalWrite(MS2,LOW);
        break;
        
      case 3:
      digitalWrite(MS0,LOW);
      digitalWrite(MS1,HIGH);
      digitalWrite(MS2,LOW);
        break;
        
      case 4:
      digitalWrite(MS0,HIGH);
      digitalWrite(MS1,HIGH);
      digitalWrite(MS2,LOW);
        break;

      default:
      Serial.println("Microstepping mode should be from 1 to 4");
    }
        Serial.print("Microstepping mode:"); Serial.print(stepmode);
  }
