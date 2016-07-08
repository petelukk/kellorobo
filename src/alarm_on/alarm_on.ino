#include <LiquidCrystal.h>


// Kellorobo

uint8_t   time_h = 0;
uint8_t   time_min = 0;
uint8_t   alarm_h = 0;
uint8_t   alarm_min = 0;
uint16_t  dist = 0xffff;
unsigned long dur;

const uint8_t ultra_trig  = 13;    // ultrasonic sensor pins
const uint8_t ultra_echo  = 12;

const uint8_t motorA_ena  = 11;    // motor pins
const uint8_t motorB_ena = 10;
const uint8_t motorA_in1  = 9;
const uint8_t motorA_in2  = 8;
const uint8_t motorB_in3  = 7;
const uint8_t motorB_in4  = 6;

const uint8_t piezo_pin1  = 5;    // piezo pins
const uint8_t piezo_pin2  = 4;

const uint8_t rot_pin1    = 2;      // rotary encoder pins
const uint8_t rot_pin2    = 3;
const uint8_t rot_button  = 1;

const uint8_t turn_time   = 700;    // milliseconds, 700 = hattuvakio




bool alarm            = true;
bool turn_right_bool  = false;          // for motors

LiquidCrystal lcd(A5, A4, A0, A1, A2, A3); //RS, EN, D4, D5, D6, D7

void setup()
{
  pinMode(piezo_pin1,   OUTPUT);
  pinMode(piezo_pin2,   OUTPUT);
  pinMode(rot_pin1,     INPUT_PULLUP);
  pinMode(rot_pin2,     INPUT_PULLUP);
  pinMode(rot_button,   INPUT_PULLUP);
  pinMode(motorA_ena,   OUTPUT);
  pinMode(motorB_ena,   OUTPUT);
  pinMode(motorA_in1,   OUTPUT);
  pinMode(motorB_in3,   OUTPUT);
  pinMode(motorA_in2,   OUTPUT);
  pinMode(motorB_in4,   OUTPUT);

  lcd.begin(16, 2);                          // init display

  //attachInterrupt(digitalPinToInterrupt(2), myEncoder, FALLING);  // encoder pin on interrupt 0 - pin 2

  digitalWrite(motorA_ena, LOW);      // Motors are set to free run by default
  digitalWrite(motorB_ena, LOW);
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, LOW);
  digitalWrite(motorB_in3, LOW);
  digitalWrite(motorB_in4, LOW);

  pinMode(ultra_trig,      OUTPUT);   // init ultrasound pins
  pinMode(ultra_echo,      INPUT);

  // Serial.begin(9600);

}

void change_direction()
{
  if (turn_right == false)
  {
    turn_left();
    delay(turn_time);
    turn_right_bool = true;
    drive_fwd();
  }
  else
  {
    turn_right();
    turn_right_bool = false;
    delay(turn_time);
    drive_fwd();
  }
}

void drive_fwd()                              // set robot to drive forwards
{
  digitalWrite(motorA_in1, HIGH);
  digitalWrite(motorA_in2, LOW);
  digitalWrite(motorB_in3, HIGH);
  digitalWrite(motorB_in4, LOW);
}

void turn_right()
{
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, HIGH);
  digitalWrite(motorB_in3, HIGH);
  digitalWrite(motorB_in4, LOW);
}

void turn_left()
{
  digitalWrite(motorA_in1, HIGH);
  digitalWrite(motorA_in2, LOW);
  digitalWrite(motorB_in3, LOW);
  digitalWrite(motorB_in4, HIGH);
}

void stop_motors()
{
  digitalWrite(motorA_ena, LOW);     // set all motor pins low
  digitalWrite(motorB_ena, LOW);
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, LOW);
  digitalWrite(motorB_in3, LOW);
  digitalWrite(motorB_in4, LOW);
}

void calc_distance()
{
  
  while (dist >= 15)        // in cm
  {
    if (digitalRead(rot_button) == LOW)      // pin is low when button is pressed
    {
      alarm = false;
    }
    playsound();

    // Sets the trigPin on HIGH state for 100 microseconds
    digitalWrite(ultra_trig, HIGH);
    delayMicroseconds(100);
    digitalWrite(ultra_trig, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    dur = pulseIn(ultra_echo, HIGH);

    // Calculating the distance in cm
    dist = dur * 0.034 / 2;
  }

}


void loop() // RENAME THIS TO alarm_on() IN FINAL VERSION  // called when alarm rings
{

  digitalWrite(motorA_ena, HIGH);     // set motor power pins high
  digitalWrite(motorB_ena, HIGH);

  drive_fwd();

  digitalWrite(ultra_trig, LOW);      // init
  delay(100);

  while (alarm)
  {
    calc_distance();
    change_direction();
    dist = 0xffff;
  }
    stop_motors();
}

void playsound()
{
  digitalWrite(piezo_pin2, LOW);
  tone(piezo_pin1, 1000, 50);     // 1000 Hz pwm for 50 ms
}



// mitja 8.7.2016
