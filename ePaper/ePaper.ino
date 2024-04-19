#define LILYGO_MINI_EPAPER_ESP32S3
#include <GxEPD.h>
#include <boards.h>
#include <GxGDGDEW0102T4/GxGDGDEW0102T4.h> //1.02" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include "myFont.h"

#include "RTClib.h"
RTC_DS3231 rtc;
int sda=7; int scl=6;

GxIO_Class io(SPI,  EPD_CS, EPD_DC,  EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);

#define white GxEPD_WHITE
#define black GxEPD_BLACK

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  59 

String Wdays[7]={"SU", "MO", "TU", "WE", "TH", "FR", "SA"};
String hour,minute,day,month;

void setup(void)
{  
     pinMode(9,OUTPUT);
     digitalWrite(9,0);
     pinMode(8,OUTPUT);
     digitalWrite(8,1);
     delay(50);
     Wire.begin(sda,scl);

    rtc.begin(); 

    pinMode(EPD_POWER_ENABLE, OUTPUT);
    digitalWrite(EPD_POWER_ENABLE, HIGH);
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init(); // enable diagnostic output on Serial     
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

        DateTime now = rtc.now();

    if(now.hour()<10) hour="0"+String(now.hour());else hour=String(now.hour());
    if(now.minute()<10) minute="0"+String(now.minute()); else minute=String(now.minute());
    if(now.month()<10) month="0"+String(now.month()); else month=String(now.month());
    if(now.day()<10) day="0"+String(now.day()); else day=String(now.day());
  

    display.setRotation(1);
    display.fillScreen(white);
    display.setTextColor(black);
    //main fram
    display.drawRoundRect(4, 4, 120, 72, 5, black);
    
      //date frame
    display.drawRoundRect(60, 8, 58, 28, 4, black);

    //day frame
    display.fillRoundRect(8, 44, 19, 15, 2, black);
   
    //batery
    display.drawRect(8, 10, 33, 14, black);
    display.fillRect(41, 14, 3, 6, black);
    for(int i=0;i<6;i++)
    display.fillRect(10+(i*5), 12, 4, 10, black);
    // bat voltage
    display.setFont(&Dialog_bold_12);
    display.setCursor(8, 38);
    display.println("TIME");

    //print time
    display.setFont(&DSEG14_Classic_Bold_24);
    display.setCursor(30, 70);
    display.println(hour+":"+minute);

    //print date
    display.setFont(&DSEG14_Modern_Bold_11);
    display.setCursor(64, 32);
    display.println(month+"/"+day);
   
    display.setFont(&Dialog_bold_9);
    display.setCursor(86, 18);
    display.println("DATE");
    display.setTextColor(white);
    display.setCursor(9, 55);
    display.println(Wdays[now.dayOfTheWeek()]);

    display.update();
    esp_deep_sleep_start();
}


void loop()
{

}




