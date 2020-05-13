// 실내 아두이노
/*
#include <DHT.h>
#include <MQ135.h>
#include "ventilation_system.h"

DHT dht(DHT_PIN, DHT_TYPE);         // 온습도 측정 센서
MQ135 mq(CO2_PIN);                  // CO2 측정 센서

float humidity = 0.0;       // 습도 %
float temperature = 0.0;    // 온도 C
float voMeasured = 0.0;     // 먼지 실측값
float calcVoltage = 0.0;    // 먼지 전압 보정값
float dustDensity = 0.0;    // 먼지 농도 ug/m3
float co2 = 0.0;            // CO2 ppm
int gas = 0;                // 가

// 평균값 산출을 위한 합계 변수
float totalHumi = 0.0;
float totalDust = 0.0;
float totalTemp = 0.0;
float totalCo2 = 0.0;
int totalGas = 0;

// 센서 값을 읽은 횟수
int times = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("***** SENSOR TEST ******");

  // 온습도
  Serial.println("** SET DHT SENSOR **");
  dht.begin();

  // 먼지
  Serial.println("** SET DUST SENSOR **");
  pinMode(DUST_PIN, INPUT);
  pinMode(DUST_LED_PIN, OUTPUT);

  // CO2
  Serial.println("** SET CO2 SENSOR **");
  pinMode(CO2_PIN, INPUT);

  // 가스
  Serial.println("** SET GAS SENSOR **");
  pinMode(GAS_PIN, INPUT);

  // 센서 확
  testOneTime();
}

void loop()
{
  // 온습도 측정
  totalHumi += dht.readHumidity();
  totalTemp += dht.readTemperature();

  // 먼지 측정
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(SAMPLING_TIME);
  voMeasured = analogRead(DUST_PIN);
  delayMicroseconds(DELTA_TIME);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(SLEEP_TIME);

  // 먼지 수치 보정 계산
  calcVoltage = voMeasured * (5.0 / 1024.0);
  totalDust += (0.17 * calcVoltage *0.047) * 1000;

  // CO2 측정 및 수치 보정
  totalCo2 += mq.getCorrectedPPM(temperature, humidity);

  // 가스 측정
  totalGas += analogRead(GAS_PIN);

  times++;
  
  // 평균 결과 출력
  if (times == READING_TIMES)
  {
    Serial.println();
    Serial.println("- Humidity: " + String(totalHumi/READING_TIMES) + "%");
    Serial.println("- Temperature: " + String(totalTemp/READING_TIMES) + "C");
    Serial.println("- Dust Density: " + String(totalDust/READING_TIMES) + " ug/m3");
    Serial.println("- CO2: " + String(totalCo2/READING_TIMES) + " ppm");
    Serial.println("- GAS: " + String(totalGas/READING_TIMES));

    // 변수 초기화
    times = 0;
    totalHumi = 0.0;
    totalTemp = 0.0;
    totalDust = 0.0;
    totalCo2 = 0.0;
    totalGas = 0.0;
  }
  
  delay(DELAY_TIME);

}

void testOneTime()
{
  Serial.println();
  Serial.println("** FIRST TEST START **");

  // 온습도 측정
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // 온습도 결과 출력
  Serial.print("- Humidity: " + String(humidity) + "%");
  Serial.println("- Temperature: " + String(temperature) + "C");

  // 먼지 측정
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(SAMPLING_TIME);
  voMeasured = analogRead(DUST_PIN);
  delayMicroseconds(DELTA_TIME);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(SLEEP_TIME);

  // 먼지 수치 보정 계산
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = (0.17 * calcVoltage *0.047) * 1000;

  // 먼지 결과 출력
  Serial.print("- Raw Value (0-1023): " + String(voMeasured));
  Serial.print(" - Voltage: " + String(calcVoltage));
  Serial.println(" - Dust Density: " + String(dustDensity) + " ug/m3");// 측정
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(SAMPLING_TIME);

  // CO2 측정 및 수치 보정
  co2 = mq.getCorrectedPPM(temperature, humidity);

  // CO2 결과 출력
  Serial.println("- CO2: " + String(co2) + " ppm");

  // 가스 측정
  gas = analogRead(GAS_PIN);
  Serial.println("- GAS: " + String(gas));
  Serial.println("** FIRST TEST ENDED **");

  delay(DELAY_TIME);
}


/*

*/
