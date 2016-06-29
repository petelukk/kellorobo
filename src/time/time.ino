#include <TimeAlarms.h>
#include <Time.h>
#include <TimeLib.h>

uint8_t hms[3] = {0,0,0}; //hours minutes and seconds  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  set_time(0,0);
}

void set_time(int hours, int minutes)
{
  TimeElements tm;
  tm.Hour = hours;
  tm.Minute = minutes;
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

void loop() {
  // put your main code here, to run repeatedly:
  time_t timet;
  delay(1000);
  timet = now();
  Serial.print(hour(timet));
  Serial.print(":");
  Serial.print(minute(timet));
  Serial.print(":");
  Serial.println(second(timet));
  get_time();
  int i;
  for (i = 0; i<3; i++)
    {
      Serial.println (hms[i]);
    }
  
}
