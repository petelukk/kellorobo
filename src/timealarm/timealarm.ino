#include <Time.h>
#include <TimeAlarms.h>
#include <string.h>
void setup() 
{
  Serial.begin(9600);
  Alarm.alarmRepeat(0,0,5,lol);
  setTime(0,0,0,0,0,0);
}
void loop(){
  time_t aika;
  aika = now();
  Serial.print(minute(aika));
  Serial.print(":");
  Serial.println(second(aika));
  
}
void lol()
{
  for(;;)
  Serial.print ("niga");
}


