#ifndef Button_h
#define Button_h
#include "Arduino.h"

enum button_state {S0, S1, S2, S3, S4};
const int SHORT_PRESS = 1;
const int LONG_PRESS = 2;
const int HELD = 3;

class Button{
  public:
  uint32_t S2_start_time;
  uint32_t button_change_time;    
  uint32_t debounce_duration;
  uint32_t long_press_duration;
  uint8_t pin;
  uint8_t flag;
  uint8_t button_pressed;
  button_state state;
  Button(int p);
  void read();
  int update();
};
#endif