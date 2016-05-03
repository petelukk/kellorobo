#include <LiquidCrystal.h>



//Kellorobo



uint8_t time_h =0;
uint8_t time_min = 0;
uint8_t alarm_h =0;
uint8_t alarm_min = 0;

const uint8_t piezo_pin1 = 5;
const uint8_t piezo_pin2 = 4;

const uint8_t rot_pin1 = 3;
const uint8_t rot_pin2 = 2;
const uint8_t rot_button = 1;

const uint8_t motor_l_pwm = 5;
const uint8_t motor_r_pwm = 5;
const uint8_t motor_l_en1 = 5;
const uint8_t motor_r_en1 = 5;
const uint8_t motor_l_en2 = 5;
const uint8_t motor_r_en2 = 5;


void main() {
  setup();
    
}




int setTime(){


  
}

void setup(){
    LiquidCrystal lcd(A5, A4, A0, A1, A2, A3); //RS, EN, D4, D5, D6, D7
    lcd.begin(16, 2);                          // init display
}


