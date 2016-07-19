#include <avr/interrupt.h>

ISR(PCINT2_vect)
{
  digitalWrite(13, !digitalRead(13));
  delay(100);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(0,INPUT_PULLUP); //inttipinni PD0, RXD, PCINT16
  //                  The pin change interrupt PCI2 will trigger if any enabled PCINT[23:16] pin toggles.
  //                  The PCMSK2, PCMSK1 and PCMSK0 Registers control which pins contribute to the pin change interrupts
  PCICR |= 0b00000100; //PCIE2: Pin Change Interrupt Enable 2
  PCMSK2 |= 0b00000001;
  sei();
   
  pinMode(13,OUTPUT); //ledi
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
}

void loop() {
    sei();
    for(;;)
    {
      ;
    }
}
