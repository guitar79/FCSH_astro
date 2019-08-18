#ifdef S_OLED
  #include "U8glib.h"
  U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);

  void U8G_startFMenu()
  {
    u8g.setFont(u8g_font_5x8);
    u8g.setColorIndex(1);
    
    //U8G_DRAW;
    u8g.firstPage();   
      do {      
        u8g.drawStr(40,20,"Position"); u8g.drawStr(64,20,": "+stepper.currentPosition());
        u8g.drawStr(40,40,"MicroStepping Mode"); u8g.drawStr(64,40,": "+stepmode);
        u8g.drawStr(40,80,"Temperature"); //u8g.drawStr(Temperature);
        u8g.drawStr(40,100,"Humidity"); //u8g.drawStr(Humidity);
      } while( u8g.nextPage() ); 
  }


#endif
