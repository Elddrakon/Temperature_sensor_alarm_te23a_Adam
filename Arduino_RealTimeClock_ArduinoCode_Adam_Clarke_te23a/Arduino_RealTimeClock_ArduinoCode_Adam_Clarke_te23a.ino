//Name: Real time alarm clock (without sound).
//Aurthor: Adam Clarke te23a.
//Date: 2024-11-12.

#include "U8glib.h" //Inkluderar U8g bibiloteket
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

#include <RTClib.h> // Inkluderar bibilotektet för klockan
#include <Wire.h> //Inkluderar någonting
RTC_DS3231 rtc;
void setup()
{
  u8g.setFont(u8g_font_unifont);
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  //rtc.adjust(DateTime(2019, 1, 21, 5, 0, 0));
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
}
void loop()
{
  DateTime now = rtc.now();
  Serial.println("Time is now: " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second())); //Beskriver vad som ska printas ut i serial monitorn
  draw(String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
  if (now.minute() >= 30) { 
    digitalWrite(4, HIGH);
    if (now.minute() >= 35) {
      digitalWrite(5, HIGH);
      if (now.minute() >= 40) {
        digitalWrite(6, HIGH);
      }
    }
  } 

  delay(1000);
}


void draw(String text) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_osb21);
  u8g.firstPage();  
  do {
    u8g.drawStr( 0, 22, text.c_str());
  } while( u8g.nextPage() );
}





