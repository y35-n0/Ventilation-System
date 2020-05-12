// 실외 아두이노 종합
/*
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <MQ135.h>
#include "ventilation_system.h"

// WiFi 및 TCP 연결 객체
WiFiServer server(PORT);
IPAddress ip;

// JSON 객체
DynamicJsonDocument bufJson(1024);    // 받을 데이터
DynamicJsonDocument dataJson(1024);   // 보낼 데이터
String bufStr;


// 아두이노 데이터
const String dataNames[DATA_CNT * 2] = {"i_temp", "i_humid", "i_dust", "i_gas1", "i_gas2",
                                        "o_temp", "o_humid", "o_dust", "o_gas1", "o_gas2"
                                       };

// 센서 관련 객체
DHT dht(DHT_PIN, DHT_TYPE);         // 온습도 측정 센서
MQ135 mq(CO2_PIN);                  // CO2 측정 센서

// 센서 측정
float humidity = 0.0;       // 습도 %
float temperature = 0.0;    // 온도 C
float voMeasured = 0.0;     // 먼지 실측값
float calcVoltage = 0.0;    // 먼지 전압 보정값
float dustDensity = 0.0;    // 먼지 농도 ug/m3
int gas = 0;                // 가스


float total[DATA_CNT] = {0.0, };      // 평균값 산출을 위한 합계 변수
float data[DATA_CNT * 2] = {0.0, };   // 실내외 데이터

// 센서 값을 읽은 횟수
int times = 0;

// 기타 운영 관련 변수
boolean isAuto = true;  // 자동 모드 여부
int id = -1;            // client id
int cmd = -1;           // 아두이노 명령

// 멀티스레드처럼 이용하기 위함
unsigned long now = 0;
unsigned long timeObserved = 0;
unsigned long timeVentilated = 0;
unsigned long timeToVentilate = 0;


void setup()
{
  Serial.begin(9600);

  // 와이파이 연결
  Serial.println();
  Serial.println("** CONNECT WIFI **");

  Serial.print("- Connecting AP ");
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" Ready");
  Serial.print("- IP address: ");
  Serial.println(WiFi.localIP());

  // 서버 설정
  Serial.println("- Setting Server");
  server.begin();

  // 온습도
  Serial.println("** SET DHT SENSOR **");
  dht.begin();

  // 먼지
  Serial.println("** SET DUST SENSOR **");
  pinMode(DUST_PIN, INPUT);
  pinMode(DUST_LED_PIN, OUTPUT);

  // 가스
  Serial.println("** SET GAS SENSOR **");
  pinMode(GAS_PIN, INPUT);

  // 모터
  Serial.println("- Set motor");
  pinMode(MTA_PIN, OUTPUT);
  pinMode(MTB_PIN, OUTPUT);

}

void loop()
{
  // 센서 값 측정을 위한 시간 확인
  now = millis();
  if ((now - timeObserved) >= DELAY_TIME)
  {
    timeObserved = millis();
    observeValues();
  }

  // 환기를 하는 중이고 환기가 끝났을 때
  if (timeToVentilate > 0 && ((now - timeVentilated) >= timeToVentilate) )
  {
    timeToVentilate = 0;

    // 모터 끄기
    digitalWrite(MTA_PIN, LOW);
    digitalWrite(MTB_PIN, LOW);
  }


  // client와 연결
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("- Client connected");
    while (client.connected())
    {
      if (client.available())
      {
        bufStr = client.readString();
        Serial.println("- Received Str: " + bufStr);
        deserializeJson(bufJson, bufStr);

        // request에 따라 응답을 달리함
        id = bufJson["id"];
        switch (id) {
          // 실내 아두이노 데이터 수집
          case INNER_ARDUINO:
            Serial.println("- INNER ARDUINO, Parsing received data ...");
            for (int i = 0; i < DATA_CNT; i++)
            {
              dataJson[dataNames[i]] = bufJson[dataNames[i]];
            }
            break;

          // 안드로이드
          case ANDROID:
            Serial.print("- ANDROID, ");
            cmd = bufJson["cmd"];
            //cmd
            switch (cmd)
            {
              // 데이터 전송
              case REQUEST:
                Serial.println("Sending data...");
                Serial.print("- Data : ");
                serializeJson(dataJson, bufStr);
                Serial.println(bufStr);
                client.print(bufStr);
                break;
              // 자동 조작
              case AUTO:
                Serial.println("Automatic mode...");
                isAuto = true;
                break;
              case MANUAL:
                Serial.println("Manual mode...");
                isAuto = false;
                break;
              // 약한 환기
              case WEAK_VT:
                Serial.println("Ventilate weakly...");

                // 모터 작동
                turnOnMotor(WEAK_TIME);
                break;
              case STRONG_VT:
                Serial.println("Ventilate strongly...");

                // 모터 작동
                turnOnMotor(STRONG_TIME);
                break;
              default:
                Serial.println("Unknown usage");
                client.println("Unknown usage");
            }
            break;

          default:
            Serial.println("- Unknown User");
            client.println("- Unknown User");
            break;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("- Client disconnected");
  }

}

// 실외 센서 데이터 관측
void observeValues()
{
  Serial.println("** Observing Values **");

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

  // 가스 측정
  total[GAS] += analogRead(GAS_PIN);

  times++;
  if (times == READING_TIMES)
  {
    // 평균 결과 출력
    Serial.println("** Avg Observed Values **");

    Serial.print("- Humidity: " + String(total[HUM] / READING_TIMES) + "%");
    Serial.print(" - Temperature: " + String(total[TEM] / READING_TIMES) + "C");
    Serial.print(" - Dust Density: " + String(total[DUS] / READING_TIMES) + " ug/m3");
    Serial.println("- GAS: " + String(total[GAS] / READING_TIMES));

    // 보낼 관측치 -> JSON
    // 변수 초기화
    for (int i = 0; i < DATA_CNT; i++) {
      dataJson[dataNames[DATA_CNT + i]] = total[i];
      total[i] = 0.0;
    }
    times = 0;

    if (isAuto && dataJson.containsKey(dataNames[0]))
    {
      for (int i = 0; i < DATA_CNT * 2; i++) {
        data[i] = dataJson[dataNames[i]];
      }
      // 실내 미세먼지 >= 36 and 실내 미세먼지 > 실외 미세먼지
      // 실외 미세먼지 > 75 : 약식 환기 / 실외 미세먼지 <= 75 : 환기
      if (data[DUS] >= 36 && data[DUS] > data[DATA_CNT + DUS]) {
        turnOnMotor(data[DATA_CNT + DUS] > 75 ? WEAK_TIME : STRONG_TIME);
        return;
      }
      // 실내 CO2 >= 1000
      // 실외 미세먼지 > 75 : 약식 환기 / 실외 미세먼지 <= 75 : 환기
      if (data[CO2] >= 1000) {
        turnOnMotor(data[DATA_CNT + DUS] > 75 ? WEAK_TIME : STRONG_TIME);
        return;
      }
      
      // 실내 유해가스 >= 1000
      // 실외 미세먼지 > 75 : 약식 환기 / 실외 미세먼지 <= 75 : 환기
      if (data[GAS] >= 200) {
        turnOnMotor(data[DATA_CNT + DUS] > 75 ? WEAK_TIME : STRONG_TIME);
        return;
      }

    }
  }
}

void turnOnMotor(int t) {
  timeVentilated = millis();
  timeToVentilate = t;
  digitalWrite(MTA_PIN, HIGH);
  digitalWrite(MTB_PIN, LOW);

}

/*
*/
