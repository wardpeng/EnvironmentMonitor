
#include <Wire.h>
#include "RTClib.h"

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display 

/*时钟模块*/
RTC_DS1307 rtc;


void setup()
{

  
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
//  lcd.print("Hello, world!");  
  
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));//调整时间
  if (! rtc.isrunning()) {
    lcd.print("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  lcd.print("OK!!!");
 
}


void loop()
{

}

