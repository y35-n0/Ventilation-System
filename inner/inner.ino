// 실내 아두이노
/*
 *  1. AP_SSID, AP_PASS에 WiFi 이름, 비밀번호 입력해야 합니다.
 *  2. DHT_PIN, DUST_PIN, DUST_LED_PIN, CO2_PIN, GAS_PIN 핀번호가 맞는지 확인해주세요.
 */


/**** 기본 설정 ****/
/*
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <MQ135.h>


#define AP_SSID     "공유기 아이디"    // WiFi SSID
#define AP_PASS     "공유기 비밀번호"   // WiFi 비밀번호

#define DHT_PIN       16            // A2, 온습도 센서
#define DHT_TYPE      DHT11         // 온습도 사용하는 센서 종류

#define DUST_PIN      15            // A1, 먼지 센서
#define DUST_LED_PIN  2             // PD2, 먼지 LED
#define SAMPLING_TIME 280           // 먼지 측정 관련 시간, micro 단위
#define DELTA_TIME    40            // 먼지 측정 관련 시간, micro 단위
#define SLEEP_TIME    9680          // 먼지 측정 관련 시간, micro 단위

#define CO2_PIN       14            // A0, CO2 센서

#define GAS_PIN       17            // A3, 가스 센서

#define DELAY_TIME    2000          // 측정, 대기 시간
#define READING_TIMES 5             // 측정 횟수


DHT dht(DHT_PIN, DHT_TYPE);         // 온습도 측정 센서
MQ135 mq(CO2_PIN);                  // CO2 측정 센서

float humidity = 0.0;          // 습도 %
float temperature = 0.0;           // 온도 C
float voMeasured = 0.0;     // 먼지 실측값
float calcVoltage = 0.0;    // 먼지 전압 보정값
float dustDensity = 0.0;    // 먼지 농도 mg/m3
float co2 = 0.0;                     // CO2 ppm
int gas = 0;


//  float dust[READING_TIMES] = {0.0, };      // 먼지 농도 mg/m3
//  float humid[READING_TIMES] = {0.0, };     // 습도 %
//  float


//  float totalDust = 0;
//  float totalHumid = 0;

void setup()
{
  Serial.begin(9600);

  // WiFi
  Serial.println();
  Serial.println("** CONNECT WIFI **");

  Serial.print("- Connecting ");
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" Ready");
  Serial.print("- IP address: ");
  Serial.println(WiFi.localIP());

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
}

void loop()
{
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
  dustDensity = 0.17 * calcVoltage - 0.1;

  // 먼지 결과 출력
  Serial.print("- Raw Value (0-1023): " + String(voMeasured));
  Serial.print(" - Voltage: " + String(calcVoltage));
  Serial.println(" - Dust Density: " + String(dustDensity) + " mg/m3");// 측정
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(SAMPLING_TIME);

  // CO2 측정 및 수치 보정
  co2 = mq.getCorrectedPPM(temperature, humidity);

  // CO2 결과 출력
  Serial.println("- CO2: " + String(co2) + " ppm");

  // 가스 측정
  gas = analogRead(GAS_PIN);
  Serial.println("- GAS: " + String(gas));
  
  delay(DELAY_TIME);

}


/*

*/
