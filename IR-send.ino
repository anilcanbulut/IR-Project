#include <IRremote.h>
//10054

IRsend irsend;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //Power//
  Serial.println("Ac");
  irsend.sendNEC(0x88C0051, 28); //Power Code
//0x88C0051
}
