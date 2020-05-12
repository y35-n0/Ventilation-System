// CO2 측정 테스트
/*
#include <MQ135.h>
#include "ventilation_system.h"

MQ135 mq(CO2_PIN);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("** MEASURE CO2 TEST **");

  pinMode(CO2_PIN, INPUT);
}

void loop()
{
  float ppm = mq.getPPM();
  Serial.println("- CO2: " + String(ppm) + "PPM");

  delay(1000);
}
/*
*/
