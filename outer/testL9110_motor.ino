// L9110 모터 테스트
/*
#include <SoftwareSerial.h>


#define MTA_PIN 9   
#define MTB_PIN 8   


void setup()
{
  Serial.begin(9600);

  // 와이파이 연결
  Serial.println();
  Serial.println("** MOTOR TEST **");

  Serial.println("- Set motor");
  pinMode(MTA_PIN, OUTPUT);
  pinMode(MTB_PIN, OUTPUT);
}

void loop()
{
  digitalWrite(MTA_PIN, HIGH);
  digitalWrite(MTB_PIN, HIGH);
  delay(1000);
  digitalWrite(MTA_PIN, LOW);
  digitalWrite(MTB_PIN, HIGH);
  delay(1000);
  digitalWrite(MTA_PIN, HIGH);
  digitalWrite(MTB_PIN, LOW);
  delay(1000);
  digitalWrite(MTA_PIN, LOW);
  digitalWrite(MTB_PIN, LOW);
  delay(1000);
  
}
/*
*/