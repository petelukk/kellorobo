#include <LiquidCrystal.h>


// Kellorobo

uint8_t time_h =0;
uint8_t time_min = 0;
uint8_t alarm_h =0;
uint8_t alarm_min = 0;


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

int choice = 0;
volatile signed int location = 1;


bool turn_right = false;          // for motors

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
  
  Serial.begin(9600);
  
}

void change_direction()
{
  if(turn_right == false)
    {
      digitalWrite(motorA_in1, HIGH);         // turn
      digitalWrite(motorA_in2, LOW);
      digitalWrite(motorB_in3, LOW);
      digitalWrite(motorB_in4, HIGH);
      turn_right = true;
      delay(turn_time);
      drive_fwd();       
    }
  else
    {
      digitalWrite(motorA_in1, LOW);          // turn
      digitalWrite(motorA_in2, HIGH);
      digitalWrite(motorB_in3, HIGH);
      digitalWrite(motorB_in4, LOW);
      turn_right = false;
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
      
int calc_distance()
  {
    uint16_t dist;
    unsigned long dur;
    
  
      // Sets the trigPin on HIGH state for 10 microseconds
    digitalWrite(ultra_trig, HIGH);
    delayMicroseconds(100);
    digitalWrite(ultra_trig, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    dur = pulseIn(ultra_echo, HIGH);
    
    // Calculating the distance
    dist = dur*0.034/2;
    return dist;                      // in cm
  }
void alarmOn()                        // called when alarm rings
{ 
  uint16_t ultra_distance = 0xffff;
                                       
  digitalWrite(motorA_ena, HIGH);     // set motor power pins high
  digitalWrite(motorB_ena, HIGH);

  drive_fwd();

  digitalWrite(ultra_trig, LOW);      // init
  delayMicroseconds(2);
  
  
  while(ultra_distance >= 15)         // in cm
  {
    ultra_distance = calc_distance();
    playsound();
  }
  ultra_distance = 0xffff;
  change_direction();

}

void playsound()
{
  digitalWrite(piezo_pin2, LOW);
  tone(piezo_pin1, 1000, 50);     // 1000 Hz pwm for 50 ms
  
}



// unfinished 6.7.2016, mitja
