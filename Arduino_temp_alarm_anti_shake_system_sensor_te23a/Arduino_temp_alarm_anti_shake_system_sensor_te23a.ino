//Name: Temperature alarm sensor and fan.
//Aurthor: Adam Clarke
//Date: 2024-11-12
//Description: This code uses an temperature sensor to inform you if your surounding temperature is above your liking,
//it further uses the information to activate an fan that will help cool you down if the surounding temperature is too high. 
//All the values, including the set threshhold and the current surounding temperature will be given att all time on an u8g led screen,
//so that you will be able to know how close the surounding temp is to your threshhold.
//The Included libraries--
#include "U8glib.h" 
//--
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

//Global variables--
unsigned long projectDuration;
const int potPin = A0;
const int potPin1 = A1;
//--


void setup(void) {
  Serial.begin(9600);
  


  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  pinMode(potPin, INPUT);
  pinMode(3, OUTPUT);
  
  for(int i = 3; i <= 5; i++){
    pinMode(i, OUTPUT);
  }
}

void loop(void) {
  
  int thresh = getThresh(); //Gets the Treshhold variable
  float temp = getTemp(); //Gets the Temperature variable
  Serial.println(temp); //Prints it out in the serialmonitor so that I can test it without the u8g screen.
  Serial.println(thresh); //Same purpose as stated in the comment above.
  if (thresh > temp){
    digitalWrite(3, LOW); //Turns the motor sheild off when the temperature is lower then the treshhold.
    draw("The temperature is ok", "Thresh: " + String(thresh) + " > " + String(temp)); //Calls on the draw function and gives it the elements stated.
  } else {
    //alarm(); //Is written as an comment incase I want to bring it back in the future.
    digitalWrite(3, HIGH); //Turns the motor sheild on when the temperature is above the threshhold.
    draw("Temperature too high!", "Thresh: " + String(thresh) + " < " + String(temp)); //Calls on the draw function and gives it the elements stated.
  }


  // rebuild the picture after some delay
  //delay(50);
}


void draw(String text, String PrintValue) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_6x10r);
  //u8g.setFont(u8g_font_osb21);
  u8g.firstPage();  
  //Prints out all the text and values on the u8g screen.
  do {
    u8g.drawStr( 0, 22, text.c_str());
    u8g.drawStr(0, 55, PrintValue.c_str());
  } while( u8g.nextPage() );
}

//Converts the 0-1023 interval into a 18-35 interval so that better fits the expected temp in a household.
int getThresh() {
  return map(analogRead(potPin), 0, 1023, 18, 35);
}
 // beräknar den riktiga temperaturen runt området.
float getTemp(){
  int Vo;
  float R1 = 10000; // value of R1 on board
  float logR2, R2, T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; //steinhart-hart coeficients for thermistor

  Vo = analogRead(potPin1);
  R2 = R1 * (1023.0 / (float)Vo - 1.0); //calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
  T = T - 273.15; //convert Kelvin to Celcius
  return T;
}

//Was an old alarm system that used 3-4 led lights that would turn off and on when the temperature was too high.
void alarm() {
  for(int i = 3; i <= 5; i++){
    digitalWrite(i, HIGH);
    delay(100);
    digitalWrite(i, LOW);
  }
}



