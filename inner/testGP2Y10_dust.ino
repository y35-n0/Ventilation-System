// 먼지 센서 테스트
/*
#include "ventilation_system.h"

float voMeasured = 0.0;
float calcVoltage = 0.0;
float dustDensity = 0.0;

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("** MEASURE DUST TEST **");

  pinMode(DUST_PIN, INPUT);
  pinMode(DUST_LED_PIN, OUTPUT);
}


void loop()
{
  // 측정
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(DUST_PIN);
  delayMicroseconds(deltaTime);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(sleepTime);

  // 수치 보정 계산
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = (0.17 * calcVoltage * 0.047) * 1000;

  // 결과 출력
  Serial.print("- Raw Dust Value (0-1023): " + String(voMeasured));
  Serial.print(" - Voltage: " + String(calcVoltage));
  Serial.println(" - Dust Density: " + String(dustDensity) + " ug/m3");

  delay(1000);
}
/*
*/
