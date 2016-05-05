#include <LiquidCrystal.h>


//Kellorobo

uint8_t time_h =0;
uint8_t time_min = 0;
uint8_t alarm_h =0;
uint8_t alarm_min = 0;

const uint8_t piezo_pin1 = 5;
const uint8_t piezo_pin2 = 4;

const uint8_t rot_pin1 = 2;
const uint8_t rot_pin2 = 3;
const uint8_t rot_button = 1;

const uint8_t motor_l_pwm = 11;
const uint8_t motor_r_pwm = 10;
const uint8_t motor_l_en1 = 9;
const uint8_t motor_l_en2 = 8;
const uint8_t motor_r_en1 = 7;
const uint8_t motor_r_en2 = 6;

int choice = 0;
int location = 0;

LiquidCrystal lcd(A5, A4, A0, A1, A2, A3); //RS, EN, D4, D5, D6, D7

void setup()
{
  pinMode(piezo_pin1, OUTPUT);
  pinMode(piezo_pin2, OUTPUT);
  pinMode(rot_pin1, INPUT_PULLUP);
  pinMode(rot_pin2, INPUT_PULLUP);
  pinMode(rot_button, INPUT_PULLUP);
  pinMode(motor_l_pwm, OUTPUT);
  pinMode(motor_r_pwm, OUTPUT);
  pinMode(motor_l_en1, OUTPUT);
  pinMode(motor_r_en1, OUTPUT);
  pinMode(motor_l_en2, OUTPUT);
  pinMode(motor_r_en2, OUTPUT);
  
  lcd.begin(16, 2);                          // init display

  attachInterrupt(0, doEncoder, FALLING);  // encoder pin on interrupt 0 - pin 2

  digitalWrite(motor_l_pwm, LOW); // Motors are set to free run by default
  digitalWrite(motor_r_pwm, LOW);
  digitalWrite(motor_l_en1, LOW);
  digitalWrite(motor_l_en2, LOW);
  digitalWrite(motor_r_en1, LOW);
  digitalWrite(motor_r_en2, LOW);

  Serial.begin(9600);
  Serial.println("Hello Computer");
}

int click() // Function to detect click on rotary encoder
{
  return digitalRead(rot_button);
}

void releaseClick() // Function to wait until user releases button
{
  while(1)
  {
    click() == 0;
    delay(25);
    Serial.println ("Click!");
    break;
  }
}

void doEncoder()  // Based on http://playground.arduino.cc/Main/RotaryEncoders#Example2
{
  /* If pinA and pinB are both high or both low, it is spinning
   * forward. If they're different, it's going backward.
   *
   * For more information on speeding up this process, see
   * [Reference/PortManipulation], specifically the PIND register.
   */

  noInterrupts();
   
  Serial.println("Interrupt!");
  
  if (digitalRead(rot_pin1) == digitalRead(rot_pin2))
  {
    location++;
  }
  else // originally just location--; but changed to take on negative values
  {
    if (location == 0)
    {
      location == 3;
    }
    else
    {
      location--;
    }
  }
  
  Serial.println (location);
  delay(25);

  interrupts();
}

int menu()
{
  lcd.setCursor(0, 0);
  lcd.print("  fwd <  > rev  ");
  lcd.setCursor(0, 1);
  lcd.print(" left <  > right");
  lcd.setCursor(6, 0);

  location = 0;
  lcd.blink();
  
  while(1)
  {
    if (click() == 1)
    {
      releaseClick();
      lcd.noBlink();
      return location;
    }
    else
    {
      if (location == 0)
      {
        lcd.setCursor(6, 0);
      }
      
      else if (location == 1)
      {
        lcd.setCursor(9, 0);
      }
      
      else if (location == 2)
      {
        lcd.setCursor(6, 1);
      }
      
      else if (location == 3)
      {
        lcd.setCursor(9, 1);
      }
    }
  }
}

void drive(int direction)
{
  digitalWrite(motor_l_pwm, HIGH);
  digitalWrite(motor_r_pwm, HIGH);
  
  if (direction == 0) // Forward
  {
    digitalWrite(motor_l_en1, HIGH);
    digitalWrite(motor_l_en2, LOW);
    digitalWrite(motor_r_en1, HIGH);
    digitalWrite(motor_r_en2, LOW);
    
    delay(2000);
  }
  
  else if (direction == 1) // Backward
  {
    digitalWrite(motor_l_en1, LOW);
    digitalWrite(motor_l_en2, HIGH);
    digitalWrite(motor_r_en1, LOW);
    digitalWrite(motor_r_en2, HIGH);
    
    delay(2000);
  }
  
  else if (direction == 2) // Left
  {
    digitalWrite(motor_l_pwm, LOW);
    
    digitalWrite(motor_r_en1, HIGH);
    digitalWrite(motor_r_en2, LOW);
    
    delay(700);
  }
  
  else if (direction == 3) // Right
  {
    digitalWrite(motor_l_en1, HIGH);
    digitalWrite(motor_l_en2, LOW);
    
    digitalWrite(motor_r_pwm, LOW);
    
    delay(700);
  }

  digitalWrite(motor_l_pwm, LOW);
  digitalWrite(motor_r_pwm, LOW);
  digitalWrite(motor_l_en1, LOW);
  digitalWrite(motor_l_en2, LOW);
  digitalWrite(motor_r_en1, LOW);
  digitalWrite(motor_r_en2, LOW);
}


void loop()
{
  // setup(); This is Arduino, this should be done anyway by default

  choice = menu();

  drive(choice);
}

