// 실내 아두이노

#include <DHT.h>
#include <MQ135.h>
#include <ArduinoJson.h>
#include "ventilation_system.h"

// JSON 객체
DynamicJsonDocument bufJson(1024);
String bufStr;

// 전송할 실내 아두이노 데이터
const String dataNames[DATA_CNT] = {"i_humid", "i_temp", "i_dust", "i_gas1", "i_gas2"};
float data[DATA_CNT] = {0.0, };

// 센서 관련 객체
DHT dht(DHT_PIN, DHT_TYPE);         // 온습도 측정 센서
MQ135 mq(CO2_PIN);                  // CO2 측정 센서

// 센서 측정 값
float humidity = 0.0;       // 습도 %
float temperature = 0.0;    // 온도 C
float voMeasured = 0.0;     // 먼지 실측값
float calcVoltage = 0.0;    // 먼지 전압 보정값

// 평균값 산출을 위한 합계 변수
float total[DATA_CNT] = {0.0, };

// 센서 값을 읽은 횟수
int times = 0;

void setup()
{
  Serial.begin(9600);
  bufJson["id"] = INNER_ARDUINO;

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
  Serial.println("** Observing Values **");
  // 온습도 측정
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  total[HUM] += humidity;
  total[TEM] += temperature;

  // 먼지 측정
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(SAMPLING_TIME);
  voMeasured = analogRead(DUST_PIN);
  delayMicroseconds(DELTA_TIME);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(SLEEP_TIME);

  // 먼지 수치 보정 계산
  calcVoltage = voMeasured * (5.0 / 1024.0);
  total[DUS] += (0.17 * calcVoltage - 0.047) * 1000;

  // CO2 측정 및 수치 보정
  total[CO2] += mq.getCorrectedPPM(temperature, humidity);

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
    Serial.print(" - CO2: " + String(total[CO2] / READING_TIMES) + " ppm");
    Serial.println("- GAS: " + String(total[GAS] / READING_TIMES));

    // 보낼 관측치 -> JSON -> string
    // 변수 초기화
    Serial.println("** Sending Data ...**");
    Serial.println("- Set Data");
    for (int i = 0; i < DATA_CNT; i++) {
      bufJson[dataNames[i]] = total[i] / READING_TIMES;
      total[i] = 0.0;
    }
    serializeJson(bufJson, bufStr);
    times = 0;

    // 서버 연결
    Serial.printf("- Connect Server %s ... ", HOST);
    if (client.connect(HOST, PORT))
    {
      // 데이터 전송
      client.print(bufStr);


      // 응답확인
      while (client.connected() || client.available())
      {
        if (client.available())
        {
          bufStr = client.readString();
          Serial.println("- Received Str : " + bufStr);
        }
      }

      // 연결 종료
      client.stop();
      Serial.println("- Disconnected");
    }
    else {
      Serial.println("Failed!");
    }
  }

  delay(DELAY_TIME);


}


/*

*/
