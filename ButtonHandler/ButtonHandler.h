//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#ifndef CBUTTONHANDLER_H_
#define CBUTTONHANDLER_H_

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////


/*
Original from (C) 2011 By P. Bauermeister
Tuned by TT
*/

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#define DEFAULT_LONGPRESS_LEN    25  // Min nr of loops for a long press

enum eEvent
{
	EV_NONE=0, 
	EV_SHORTPRESS,
	EV_LONGPRESS 
};

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

class CButtonHandler
{
  public:

    CButtonHandler(int pin, int longpress_len=DEFAULT_LONGPRESS_LEN);
    ~CButtonHandler(){}

   // Initialization done after construction, to permit static instances
    void init();

    // Handler, to be called in the loop()
    eEvent handle();

  protected:
  
    boolean 		    _was_pressed;     // previous state
    int             _pressed_counter;     // press running duration
    const uint8_t 	_pin;           // pin to which button is connected
    const int 		  _longpress_len; // longpress duration
    long 			      _lastLoopTime;
};

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

#endif /* CBUTTONHANDLER_H_ */

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

