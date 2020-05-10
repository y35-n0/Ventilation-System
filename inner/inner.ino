// 실내 아두이노

/**** 기본 설정 ****/
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <DHT.h>


#define AP_SSID     "공유기 아이디"    // WiFi SSID
#define AP_PASS     "공유기 비밀번호"   // WiFi 비밀번호

#define DUST_PIN      14            // A0, 먼지 센서
#define DUST_LED_PIN  2             // PD2, 먼지 LED
#define SAMPLING_TIME 280           // 먼지 측정 관련 시간, micro 단위
#define DELTA_TIME    40            // 먼지 측정 관련 시간, micro 단위
#define SLEEP_TIME    9680          // 먼지 측정 관련 시간, micro 단위

#define DHT_PIN       16            // A2, 온습도 센서
#define DHT_TYPE      DHT11         // 온습도 사용하는 센서 종류




#define DELAY_TIME    2000          // 측정, 대기 시간
#define READING_TIMES 5             // 측정 횟수

DHT dht(DHT_PIN, DHT_TYPE);         // 온습도 측정 변수

float dust = 0.0;                   // 먼지 농도 mg/m3
float humid = 0.0;                  // 습도 %
/*
  float dust[READING_TIMES] = {0.0, };      // 먼지 농도 mg/m3
  float humid[READING_TIMES] = {0.0, };     // 습도 %
  float


  float totalDust = 0;
  float totalHumid = 0;
*/
void setup()
{
  Serial.begin(9600);

  // WiFi
  Serial.println();
  Serial.println("** CONNECT WIFI **");
  connectWiFi();

  // 먼지
  Serial.println("** SET DUST SENSOR **");
  pinMode(DUST_PIN, INPUT);
  pinMode(DUST_LED_PIN, OUTPUT);

  // 온습도
  Serial.println("** SET DHT SENSOR **");
  dht.begin();
}

void loop()
{
  dust = getDust();     // 먼지 체크
  humid = getHumidity(); // 습도 체크
  delay(DELAY_TIME);

}

// 와이파이 연결
void connectWiFi() {

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

}

// 먼지농도 확인
float getDust() {
  float voMeasured = 0.0;     // 먼지 실측값
  float calcVoltage = 0.0;    // 먼지 전압 보정값
  float dustDensity = 0.0;    // 먼지 농도 mg/m3

  // 측정
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(SAMPLING_TIME);
  voMeasured = analogRead(DUST_PIN);
  delayMicroseconds(DELTA_TIME);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(SLEEP_TIME);

  // 수치 보정 계산
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = 0.17 * calcVoltage - 0.1;

  // 결과 출력
  Serial.print("- Raw Value (0-1023): " + String(voMeasured));
  Serial.print(" - Voltage: " + String(calcVoltage));
  Serial.println(" - Dust Density: " + String(dustDensity) + " mg/m3");

  return dustDensity;
}

// 온습도 확인 - 습도 40% ~ 60%
float getHumidity () {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read DHT sensor!");
    return -1;  // 읽지 못 했을 경우 -1
  }

  Serial.print("- Humidity: " + String(h) + "%");
  Serial.println("- Temperature: " + String(t) + "C");

  return h;
}

/*

*/
