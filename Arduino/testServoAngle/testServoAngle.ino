#include <Servo.h>

Servo test;

void setup(){
  Serial.begin(115200);
  test.attach(9);
}

int i = 0;

void loop(){
  boolean change = false;
  while(Serial.available()){
    Serial.read();
    change = true;
  }
  
  test.write(i*45);
  
  i++;
  i = i % 8;
}
