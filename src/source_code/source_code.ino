#include <LiquidCrystal.h>
#include <TimeAlarms.h>
#include <Time.h>
#include <TimeLib.h>

// Kellorobo

uint8_t time_h    = 12;
uint8_t time_min  = 34;
uint8_t alarm_h   = 98;
uint8_t alarm_min = 76;

const uint8_t piezo_pin1 = 5;
const uint8_t piezo_pin2 = 4;

const uint8_t rot_pin1   = 2; // rotary encoder pins
const uint8_t rot_pin2   = 3;
const uint8_t rot_button = 0;

const uint8_t motorA_ena = 11;  // motor pins
const uint8_t motorB_ena = 10;
const uint8_t motorA_in1 = 9;
const uint8_t motorA_in2 = 8;
const uint8_t motorB_in3 = 7;
const uint8_t motorB_in4 = 6;

const uint8_t ultra_trig = 13;    // ultrasonic sensor pins
const uint8_t ultra_echo = 12;

// menu related
volatile int rot_pos = 128;
volatile uint8_t screen = 0; // 0 = time, 1 = alarm, 2 = demo
char temp1[17];
char temp2[17];
int choice = 0;

//alarm related variables
uint16_t  dist = 0xffff;
unsigned long dur;
const uint16_t turn_time   = 700;    // milliseconds, 700 = hattuvakio

bool alarm            = true;
bool turn_right_bool  = false;          // for motors

//time related
uint8_t alarm_time[2] = {13,80}; // hours and minutes
uint8_t hms[3]        = {0,0,0}; //hours minutes and seconds  

LiquidCrystal lcd(A5, A4, A0, A1, A2, A3); //RS, EN, D4, D5, D6, D7
                 // arduino pins           // LCD PINS

void setup()
{
  pinMode(piezo_pin1, OUTPUT);      // initialize pins
  pinMode(piezo_pin2, OUTPUT);
  pinMode(rot_pin1,   INPUT_PULLUP);
  pinMode(rot_pin2,   INPUT_PULLUP);
  pinMode(rot_button, INPUT_PULLUP);
  pinMode(motorA_ena, OUTPUT);
  pinMode(motorB_ena, OUTPUT);
  pinMode(motorA_in1, OUTPUT);
  pinMode(motorB_in3, OUTPUT);
  pinMode(motorA_in2, OUTPUT);
  pinMode(motorB_in4, OUTPUT);

  pinMode(ultra_trig, OUTPUT);
  pinMode(ultra_echo, INPUT);
  
  lcd.begin(16, 2);                          // init display

  attachInterrupt(digitalPinToInterrupt(2), myEncoder, FALLING);  // encoder pin on interrupt 0 - pin 2
  
  stop_motors();
  set_time(0,0);
  set_alarm(0,1);
}

int click() // Function to detect click on rotary encoder
{
  return digitalRead(rot_button);
}

void releaseClick() // Function to wait until user releases button
{
  while(1)
  {
    if (click() == 1)
    {
      Alarm.delay(25);
      break;
    }
  }
}

void myEncoder()
{
  noInterrupts();
  int i;
  int sum = 0;
  int j;
  for (i=0; i< 10 ; i++){
    j = PIND & 0b00001100;
    sum += j;
  }
  if (sum == 0) {
    rot_pos--;
  }
  else if (sum == 80) {
    rot_pos++;
  }
  //delay(5);
  interrupts();
}

void menu() {
  lcd.clear();

  if(rot_pos % 3 == 0) {
    sprintf(temp1, " Time is %02d:%02d ", hms[0], hms[1]);
    sprintf(temp2, "                ");
  }
  else if(rot_pos % 3 == 1) {
    sprintf(temp1, "Alarm time %02d:%02d", alarm_h, alarm_min);
    sprintf(temp2, "Alarm on/off");
  }
  else {
    sprintf(temp1, " Push button to ");
    sprintf(temp2, "   start demo   ");
  }
  lcd.setCursor(0, 0);
  lcd.print(temp1);
  lcd.setCursor(0, 1);
  lcd.print(temp2);
  Alarm.delay(200);
}

void demodrive(int direction)
{
  lcd.clear();
  lcd.print("CLICK!");
  
  digitalWrite(motorA_ena, HIGH);
  digitalWrite(motorB_ena, HIGH);
  
  if (direction == 1) // Forward
  {
    digitalWrite(motorA_in1, HIGH);
    digitalWrite(motorA_in2, LOW);
    digitalWrite(motorB_in3, HIGH);
    digitalWrite(motorB_in4, LOW);
    
    Alarm.delay(2000);
  }
  
  else if (direction == 2) // Backward
  {
    digitalWrite(motorA_in1, LOW);
    digitalWrite(motorA_in2, HIGH);
    digitalWrite(motorB_in3, LOW);
    digitalWrite(motorB_in4, HIGH);
    
    Alarm.delay(2000);
  }
  
  else if (direction == 4) // Left
  {
    digitalWrite(motorA_ena, LOW);
    
    digitalWrite(motorB_in3, HIGH);
    digitalWrite(motorB_in4, LOW);
    
    Alarm.delay(700);
  }
  
  else if (direction == 3) // Right
  {
    digitalWrite(motorA_in1, HIGH);
    digitalWrite(motorA_in2, LOW);
    
    digitalWrite(motorB_ena, LOW);
    
    Alarm.delay(700);
  }

  digitalWrite(motorA_ena, LOW);
  digitalWrite(motorB_ena, LOW);
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, LOW);
  digitalWrite(motorB_in3, LOW);
  digitalWrite(motorB_in4, LOW);
}

void set_time(int hours, int minutes)
{
  TimeElements tm;
  tm.Year = 1;
  tm.Month = 1;
  tm.Day = 1;
  tm.Hour = hours;
  tm.Minute = minutes;
  tm.Second = 55;
  setTime(makeTime(tm));
}

void get_time()
{
  time_t timet;
  
  timet = now();
  hms[0] = hour(timet);
  hms[1] = minute(timet);
  hms[2] = second(timet);
}

void set_alarm(int hours, int minutes)
{
  alarm_time[0] = hours;
  alarm_time[1] = minutes;  
}

void change_direction()
{
  if (turn_right_bool == false)
  {
    turn_left();
    Alarm.delay(turn_time);
    turn_right_bool = true;
    drive_fwd();
  }
  else
  {
    turn_right();
    turn_right_bool = false;
    Alarm.delay(turn_time);
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

    // Sets the trigPin on HIGH state for 1 millisecond
    digitalWrite(ultra_trig, HIGH);
    Alarm.delay(1);
    digitalWrite(ultra_trig, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    dur = pulseIn(ultra_echo, HIGH);

    // Calculating the distance in cm
    dist = dur * 0.034 / 2;
  }
}

void playsound()
{
  digitalWrite(piezo_pin2, LOW);
  tone(piezo_pin1, 2500, 50);     // 2500 Hz pwm for 50 ms
}

void alarm_on() // called when alarm rings
{
  digitalWrite(motorA_ena, HIGH);     // set motor power pins high
  digitalWrite(motorB_ena, HIGH);

  drive_fwd();

  digitalWrite(ultra_trig, LOW);      // init
  Alarm.delay(100);
  while (alarm)
  {
    calc_distance();
    change_direction();
    dist = 0xffff;
  }
  stop_motors();
}

void loop()
{
  // setup(); This is Arduino, this should be done anyway by default

  Alarm.delay(1000);
  get_time();
  if (hms[0] == alarm_time[0] && hms[1] == alarm_time[1] && hms[2] == 0) // alarm check
  {
    alarm_on();
  }
  menu();
}

