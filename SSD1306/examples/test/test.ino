
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#include <I2Cdev.h>
#include <Wire.h>
#include <SPI.h>

#include "SSD1306.h"
  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#define LED_RX          17        

#define LED_RX_PINMODE            pinMode (LED_RX, OUTPUT);
#define LED_RX_TOGGLE             digitalWrite(LED_RX,!digitalRead(LED_RX));
#define LED_RX_OFF                digitalWrite(LED_RX,LOW);
#define LED_RX_ON                 digitalWrite(LED_RX,HIGH);

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

SSD1306 oSSD1306;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

void setup()
{
  LED_RX_PINMODE
  
  Wire.begin();
  delay(500);

  oSSD1306.init(true);
  //oSSD1306.inverDisplay();
  oSSD1306.brightness(255);
  oSSD1306.clearDisplay();
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

uint8_t Brightness = 0;
void loop()
{
  delay(50);
  LED_RX_TOGGLE
  
  //oSSD1306.clearVideoBuffer();

  oSSD1306.drawFillSquare(20,30,10);
  oSSD1306.drawSquare(20,50,9,10);

  oSSD1306.drawHLine(60,3,5);
  oSSD1306.drawVLine(60,3,2);
  
  oSSD1306.drawSquare(20,30,10);

  oSSD1306.send_string("Viva la musica",3,1);
  oSSD1306.send_float((float)(Brightness)/10.0f,9,1);
  oSSD1306.send_long((long)(Brightness++),50,1);

  oSSD1306.displayVideoBuffer();
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
