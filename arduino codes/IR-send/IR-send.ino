#include <IRremote.h>
//10054

IRsend irsend;

void setup() {
  Serial.begin(9600);
}
//908
//B8C  
void loop() {
  //Power//
  Serial.println("Ac");
  irsend.sendRC5(0x80C, 12); //Power Code
//0x88C0051
}
