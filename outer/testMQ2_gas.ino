// 가스 측정 테스트
/*
#include "ventilation_system.h"

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("** MEASURE GAS TEST **");
  
  pinMode(GAS_PIN, INPUT);
}

void loop()
{
  int gas = analogRead(GAS_PIN);
  
  Serial.println("- GAS: " + String(gas));
  
  delay(1000);
}
/*
 * 
 */
