
//////////////////////////////////////////////////////////////////////////////
// Arduino button tutorial 4.
//
// Demonstrates:
// - detection of releasing edge
// - distinction of short press and long press
// - OO implementation, supporting arbitrary number of buttons
//
// Push-buttons must be connected as follows:
//              __,__
//   Pin2 ------o   o------ GND
//              __,__
//   Pin3 ------o   o------ GND
//
// (C) 2011 By P. Bauermeister
// This example code is in the public domain.
// TUNED BY TT
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#include <ButtonHandler.h>
  
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#define BUTTON1_PIN               7  // Button 1
#define BUTTON2_PIN               8  // Button 2

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

// Instanciate button objects
CButtonHandler button1(BUTTON1_PIN);
CButtonHandler button2(BUTTON2_PIN, DEFAULT_LONGPRESS_LEN*2);

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

void print_event(const char* button_name, int event)
{
  if (event)
  {
    Serial.print(button_name);
    Serial.print(" -> ");
    Serial.println(event);
  }
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

void setup()
{
   Serial.begin(38400);

  // init buttons pins; I suppose it's best to do here
  button1.init();
  button2.init();
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

void loop()
{
  // handle button
  int event1 = button1.handle();
  int event2 = button2.handle();

  // do other things
  print_event("1", event1);
  print_event("2", event2);

}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
