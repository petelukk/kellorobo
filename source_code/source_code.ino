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

const uint8_t motor_l_pwm = 11;
const uint8_t motor_r_pwm = 10;
const uint8_t motor_l_en1 = 9;
const uint8_t motor_l_en2 = 8;
const uint8_t motor_r_en1 = 7;
const uint8_t motor_r_en2 = 6;





void setup()
{
  pinMode(piezo_pin1, OUTPUT);
  pinMode(piezo_pin2, OUTPUT);
  pinMode(rot_pin1, INPUT_PULLUP);
  pinMode(rot_pin2, INPUT_PULLUP);
  pinMode(rot_button, INPUT);
  pinMode(motor_l_pwm, OUTPUT);
  pinMode(motor_r_pwm, OUTPUT);
  pinMode(motor_l_en1, OUTPUT);
  pinMode(motor_r_en1, OUTPUT);
  pinMode(motor_l_en2, OUTPUT);
  pinMode(motor_r_en2, OUTPUT);
  
  LiquidCrystal lcd(A5, A4, A0, A1, A2, A3); //RS, EN, D4, D5, D6, D7
  lcd.begin(16, 2);                          // init display
}

int click() // Function to detect click on rotary encoder
{
  return digitalRead(rot_button);
}

void releaseClick() // Function to wait until user releases button
{
  while(1)
  {
    click == 0;
    delay(25);
    break;
  }
}

void menu()
{
  lcd.print("  fwd <  > rev  ");
  lcd.print(" left <  > right");
  lcd.setCursor(6, 0);

  int location = 0;
  
  while(1)
  {
    if (click() == 1)
    {
      releaseClick();
      return location;
    }
    
  }
}


int setTime()
{

  
}


void main()
{
  setup();

  menu();
}

