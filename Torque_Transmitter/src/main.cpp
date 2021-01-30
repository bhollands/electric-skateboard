#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <U8g2lib.h>
#include <Wire.h>

#define led 12
RF24 radio(7,10);
const byte addresses[][6] = {"00001", "00002"};
boolean speed = 0;
boolean listening = false;
boolean location;

void setup() {
  pinMode(12, OUTPUT);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
}

int reciveData(){
  location = false;
  AdjListeningState(location);
  
  while(!radio.available());
  radio.read(&speed, sizeof(speed));
  return speed;
}

void sendData(){
  location = true;
  AdjListeningState(location);
  
  int potValue = analogRead(A0);
  int angleValue = map(potValue, 0,1023, 0, 180);
  radio.write(&angleValue, sizeof(angleValue));
}
void AdjListeningState(char location){
  if(listening && location){ //if listening and it in in the sendin function
    radio.stopListening(); // stop listening
    listening = false;
    //stop listening
  } else if( !listening && !location) { //if not listening and in the reciving function
    radio.startListening(); //start listening to recive
    listening = true;
    //start listening
  }
}

void loop() {
  delay(5);
  sendData();
  delay(5);
  reciveData();
}
 




