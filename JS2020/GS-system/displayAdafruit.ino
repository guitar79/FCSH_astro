
//https://github.com/olikraus/u8glib/wiki/fontsize
//https://github.com/olikraus/u8glib/wiki/userreference#setprintpos

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold6pt7b.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH 16
#define XPOS 0
#define YPOS 1
#define DELTAY 2


//#include<U8Glib.h>
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 


  void dis_start()
  {
    display.begin(SSD1306_SWITCHCAPVCC);
    display.display();
    delay(1000);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
  }

  void draw()
  {
    
    display.clearDisplay();
    
    Serial.print(subm);
    Serial.print(menu);
    Serial.println(rm);

      display.setCursor(4,8);
      display.print("TEMP:"); 
      display.print(Temperature);
      display.print("C");

      display.setCursor(68,8);
      display.print("HUM:"); 
      display.print(Humidity);
      display.print("%");

      display.setCursor(8,22);
      display.print("POSITION :    "); 
      display.print(stepper.currentPosition());
        
        switch(subm)
        {
          case 0:
          display.setCursor(4,0);
            display.print("STATUS"); 
            display.setCursor(8,36);
            display.print("MSMODE :      ");
            display.print(stepmode);
              display.setCursor(8,50);
              if(PCMODE)display.print("PCMODE :      ON");
              else display.print("PCMODE :      OFF"); 
          display.setCursor(108,0);
            display.print("(>)"); 
          break;

          case 1:
          //u8g.setFont(u8g_font_unifont); u8g.setFontPosTop();
          display.setCursor(4,0);
            display.print("MENU");
          display.setCursor(8,36);   
            display.print("MOTOR CONTROL");
          display.setCursor(8,46);
            display.print("MOTOR SETTING");
          display.setCursor(8,56);
            display.print("SERVO & RELAY");
            
          display.setCursor(102,0);
            display.print("(<>)");
          display.setCursor(0,26+10*menu);
           display.print(">");
          break;
              
          default:
              switch(menu)
              { 
                case 1:
                //u8g.setFont(u8g_font_unifont); u8g.setFontPosTop();
                  display.setCursor(4,0);
                    display.print("POSITION CONTROL");
                  display.setCursor(8,36);
                    display.print("POWER :");
                  display.setCursor(92,36);
                    display.print(power);
                  display.setCursor(8,50);
                    display.print("MICROSTEPPING:");
                  display.setCursor(92,50);
                    display.print(stepmode);
                  display.setCursor(108,0);
                    display.print("(<)");
                  display.setCursor(0,22+rm*14);
                    if(subm==2)display.print(">");
                    else display.write(15);
                break;

                case 2:
                //u8g.setFont(u8g_font_unifont); u8g.setFontPosTop();
                  display.setCursor(4,0);
                    display.print("STEPMODE CHANGE");
                //u8g.setFont(u8g_font_5x8);
                  display.setCursor(8,36);
                    display.print("STEPMODE :    ");
                    display.print(stepmode);
                  display.setCursor(8,46);
                    display.print("POSITION TO : ");;
                    display.print(_resetPosition);
                  display.setCursor(8,56);
                    display.print("APPLY SETTINGS");
                  display.setCursor(108,0);
                    display.print("(<)");
                  display.setCursor(0,36+rm*10);
                    if(subm==2)display.print(">");
                    else display.write(15);
                break;

                case 3:
                //u8g.setFont(u8g_font_unifont); u8g.setFontPosTop();
                  display.setCursor(4,0);
                    display.print("SERVO & RELAY");
                //u8g.setFont(u8g_font_5x8);
                  display.setCursor(8,36);
                    display.print("COVER :       ");
                    if(BMask1) display.print("ON");
                      else display.print("OFF");
                  display.setCursor(8,50);
                  
                    display.print("RELAY :       ");
                    if(!rm) display.print("1");
                    else display.print(rm);
                    display.print("/");
                      switch(rm)
                      {
                        case 0:
                          if(Relay1) display.print("ON");
                          else display.print("OFF"); 
                        break;
                        case 1:
                          if(Relay1) display.print("ON");
                          else display.print("OFF"); 
                        break;
                        
                        case 2:
                          if(Relay2) display.print("ON");
                          else display.print("OFF"); 
                        break;
                        case 3:
                          if(Relay3) display.print("ON");
                          else display.print("OFF"); 
                        break;
                        case 4:
                          if(Relay4) display.print("ON");
                          else display.print("OFF"); 
                        break;
  
                      }
                    
                  display.setCursor(8,64);
                    display.print("APPLY SETTINGS(");
                    display.write(15);
                    display.print(")");
                  display.setCursor(108,0);
                    display.print("(<)");
                    if(rm) display.setCursor(0,50);
                      else display.setCursor(0,36);
                    if(subm==2)display.print(">");
                    else display.write(15);
                break;

              
            }

          break;
        }
   display.display();
  }

