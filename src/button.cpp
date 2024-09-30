#include "Arduino.h"
#include "button.h"

Button::Button(int p) {
  flag = 0;  
  state = S0;
  pin = p;
  S2_start_time = millis();
  button_change_time = millis();
  debounce_duration = 10;
  long_press_duration = 1000;
  button_pressed = 0;
}

void Button::read() {
  uint8_t button_val = digitalRead(pin);  
  button_pressed = !button_val; //invert button
}

int Button::update() {
  read();
  flag = 0;
  if (state==0) {
    if (button_pressed) {
      state = S1;
      button_change_time = millis();
    }
  } else if (state==1) {
  	if (button_pressed && millis()-button_change_time >= debounce_duration) {
  	  state = S2;
  	  S2_start_time = millis();
  	} else if (!button_pressed) {
  	  state = S0;
  	  button_change_time = millis();
  	}
  } else if (state==2) {
    if (!button_pressed) {
  	  state = S4;
  	  button_change_time = millis();
  	} else if (millis()-S2_start_time >= long_press_duration) {
  	  state = S3;
  	}
  } else if (state==3) {
  	if (!button_pressed) {
  	  state = S4;
  	  button_change_time = millis();
      flag = 0;
  	} else {flag = 3;}
  } else if (state==4) {      	
  	if (!button_pressed && millis()-button_change_time >= debounce_duration) {
  	  state = S0;
  	  flag = (millis()-S2_start_time >= long_press_duration) ? 2 : 1;
  	} else if (button_pressed && millis()-S2_start_time >= long_press_duration) {
  	  state = S3;
  	  button_change_time = millis();
  	} else if (button_pressed) {
  	  state = S2;
  	  button_change_time = millis();
  	}
  }
  return flag;
}