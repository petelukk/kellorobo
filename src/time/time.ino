#include <TimeAlarms.h>
#include <Time.h>
#include <TimeLib.h>

uint8_t alarm_time[2] = {13,80}; // hours and minutes
uint8_t hms[3] = {0,0,0}; //hours minutes and seconds  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  set_time(0,0);
  set_alarm(0,1);
}

void set_time(int hours, int minutes)
{
  TimeElements tm;
  tm.Year = 1;
  tm.Month = 1;
  tm.Day = 1;
  tm.Hour = hours;
  tm.Minute = minutes;
  tm.Second = 0;
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

void mitjanfunkkari()
{
  Serial.println("tsup nigas");
}

void loop() {
  // put your main code here, to run repeatedly:
  Alarm.delay(1000);
  get_time();
  if (hms[0] == alarm_time[0] && hms[1] == alarm_time[1] && hms[2] == 0) // alarm check
  {
    mitjanfunkkari();
  }
  char stringi[9];
  sprintf(stringi, "%02d:%02d:%02d", hms[0], hms[1], hms[2]);
  Serial.println(stringi);
  
}
