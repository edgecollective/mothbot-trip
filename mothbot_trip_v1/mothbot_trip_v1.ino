
#include <SoftwareSerial.h>

#include <TinyGPS.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

TinyGPS gps;
SoftwareSerial ss(4, 3);

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}


void u8g2_box_title(uint8_t a) {
  //u8g2.drawStr( 10+a*2, 5, "U8g2");
  u8g2.drawStr( 3, 54, "FN42IJ48");
  
  //u8g2.drawFrame(0,0,96,64);
  u8g2.drawFrame(0,0,75,50);
}


void u8g2_box_frame(uint8_t a) {
  u8g2.drawStr( 0, 0, "drawBox");
  u8g2.drawBox(5,10,20,10);
  u8g2.drawBox(10+a,15,30,7);
  u8g2.drawStr( 0, 30, "drawFrame");
  u8g2.drawFrame(5,10+30,20,10);
  u8g2.drawFrame(10+a,15+30,30,7);
}

void u8g2_disc_circle(uint8_t a) {
  u8g2.drawStr( 0, 0, "drawDisc");
  u8g2.drawDisc(10,18,9);
  u8g2.drawDisc(24+a,16,7);
  u8g2.drawStr( 0, 30, "drawCircle");
  u8g2.drawCircle(10,18+30,9);
  u8g2.drawCircle(24+a,16+30,7);
}

void u8g2_r_frame(uint8_t a) {
  u8g2.drawStr( 0, 0, "drawRFrame/Box");
  u8g2.drawRFrame(5, 10,40,30, a+1);
  u8g2.drawRBox(50, 10,25,40, a+1);
}


void u8g2_line(uint8_t a) {
  u8g2.drawStr( 0, 0, "drawLine");
  u8g2.drawLine(7+a, 10, 40, 55);
  u8g2.drawLine(7+a*2, 10, 60, 55);
  u8g2.drawLine(7+a*3, 10, 80, 55);
  u8g2.drawLine(7+a*4, 10, 100, 55);
}

void u8g2_extra_page(uint8_t a)
{
  u8g2.drawStr( 0, 0, "Unicode");
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.setFontPosTop();
  u8g2.drawUTF8(0, 24, "☀ ☁");
  switch(a) {
    case 0:
    case 1:
    case 2:
    case 3:
      u8g2.drawUTF8(a*3, 36, "☂");
      break;
    case 4:
    case 5:
    case 6:
    case 7:
      u8g2.drawUTF8(a*3, 36, "☔");
      break;
  }
}



void setup(void) {

  /* U8g2 Project: SSD1306 Test Board */
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(10, 0);
  digitalWrite(9, 0);		

  /* U8g2 Project: T6963 Test Board */
  //pinMode(18, OUTPUT);
  //digitalWrite(18, 1);	

  /* U8g2 Project: KS0108 Test Board */
  //pinMode(16, OUTPUT);
  //digitalWrite(16, 0);	

  u8g2.begin();
  //u8g2.setFlipMode(0);
  Serial.begin(9600);

  ss.begin(9600);
  
}

void u8g2_map(uint8_t max_x, uint8_t max_y, uint8_t x, uint8_t y) {
  //u8g2.drawStr( 10+a*2, 5, "U8g2");
  
  u8g2_prepare();
  u8g2.drawFrame(0,0,max_x,max_y);
  //u8g2.drawFrame(0,0,75,50);

  u8g2.setFontDirection(1);
  u8g2.drawStr(105, 8, "FN42IJ48");
  u8g2.drawFrame(max_x-1,0,13,max_y);
  
  u8g2.drawDisc(x,y,2);

  
  u8g2.setFontDirection(0);
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(113, 1, "N");

  
  //u8g2.drawStr(111, 1, "OK");
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.setFontPosTop();
  u8g2.drawUTF8(111, 12, "↑");
}

int x = 10;
int y = 10;

int max_x = 96;
int max_y = 64;

/*
float lon_left = -71.30833333333334;
float lon_right = -71.275;
float lat_top = 42.416666666666664;
float lat_bottom = 42.40416666666667;
*/

float lon_left = -71.30353331565858;
float lon_right = -71.28207564353944;
float lat_top = 42.41479165382034;
float lat_bottom = 42.40687025526763;

float xfrac=0;
float yfrac=0;

void loop(void) {

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);

    float xfrac = (flon-lon_left)/(lon_right-lon_left);
    Serial.print("xfrac=");
    Serial.println(xfrac);

    float yfrac = 1-(flat-lat_bottom)/(lat_top-lat_bottom);
    Serial.print("yfrac=");
    Serial.println(yfrac);
    
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(",");
    Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);

    x=round(yfrac*max_x);
    y=round(xfrac*max_y);
    
    Serial.print("x,y: ");
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);

    u8g2.firstPage();  
    do {
        
        u8g2_map(max_x,max_y,x,y);
    } while( u8g2.nextPage() );
    
  
  }

  gps.stats(&chars, &sentences, &failed);
  
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
    
  
  // delay between each page
  delay(150);

}
