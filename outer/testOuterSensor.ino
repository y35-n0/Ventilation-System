// 실외 아두이노 센서
/*
#include <SoftwareSerial.h>
#include <DHT.h>
#include <MQ135.h>
#include "ventilation_system.h"

DHT dht(DHT_PIN, DHT_TYPE);         // 온습도 측정 센서
MQ135 mq(CO2_PIN);                  // CO2 측정 센서

// 센서 측정
float humidity = 0.0;       // 습도 %
float temperature = 0.0;    // 온도 C
float voMeasured = 0.0;     // 먼지 실측값
float calcVoltage = 0.0;    // 먼지 전압 보정값
float dustDensity = 0.0;    // 먼지 농도 ug/m3
int co2 = 0;                // co2 ppm

// 평균값 산출을 위한 합계 변수
float total[DATA_CNT] = {0.0, };

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

  Serial.println("** SET MOTOR **");
  pinMode(MTA_PIN, OUTPUT);
  pinMode(MTB_PIN, OUTPUT);

  // 모듈 확인
  testOneTime();
}

void loop()
{
  Serial.println("** Observing Values **");
  // 모터 작
  digitalWrite(MTA_PIN, HIGH);
  digitalWrite(MTB_PIN, LOW);
  delay(500);

  // 온습도 측정
  total[HUM] += dht.readHumidity();
  total[TEM] += dht.readTemperature();

  // 먼지 측정
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(SAMPLING_TIME);
  voMeasured = analogRead(DUST_PIN);
  delayMicroseconds(DELTA_TIME);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(SLEEP_TIME);

  // 먼지 수치 보정 계산
  calcVoltage = voMeasured * (5.0 / 1024.0);
  total[DUS] += (0.17 * calcVoltage * 0.047) * 1000;

  // CO2 측정 및 수치 보정
  total[CO2] += mq.getCorrectedPPM(temperature, humidity);

  times++;
  if (times == READING_TIMES)
  {
    // 평균 결과 출력
    Serial.println("** Avg Observed Values **");

    Serial.print("- Humidity: " + String(total[HUM] / READING_TIMES) + "%");
    Serial.print(" - Temperature: " + String(total[TEM] / READING_TIMES) + "C");
    Serial.print(" - Dust Density: " + String(total[DUS] / READING_TIMES) + " ug/m3");
    Serial.println(" - CO2: " + String(total[CO2] / READING_TIMES) + " ppm");

    times = 0;
    // 변수 초기화
    for (int i = 0; i < DATA_CNT; i++) {
      total[i] = 0.0;
    }
  }

  // 모터 끄
  digitalWrite(MTA_PIN, LOW);
  digitalWrite(MTB_PIN, LOW);
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
  dustDensity = (0.17 * calcVoltage * 0.047) * 1000;

  // 먼지 결과 출력
  Serial.print("- Raw Value (0-1023): " + String(voMeasured));
  Serial.print(" - Voltage: " + String(calcVoltage));
  Serial.println(" - Dust Density: " + String(dustDensity) + " ug/m3");
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(SAMPLING_TIME);

  // CO2 측정 및 수치 보정
  co2 = mq.getCorrectedPPM(temperature, humidity);

  // CO2 결과 출력
  Serial.println("- CO2: " + String(co2) + " ppm");

  // 모터
  Serial.println("- Motor on");
  digitalWrite(MTA_PIN, HIGH);
  digitalWrite(MTB_PIN, LOW);
  delay(500);
  Serial.println("- Motor off");
  digitalWrite(MTA_PIN, LOW);
  digitalWrite(MTB_PIN, LOW);
  delay(500);
  Serial.println("** FIRST TEST ENDED **");


  delay(DELAY_TIME);
}


/*

*/
