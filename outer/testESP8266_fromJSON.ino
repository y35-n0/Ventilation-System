// Server JSON 요청 및 전송 테스트
/*
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "ventilation_system.h"

WiFiServer server(PORT);
IPAddress ip;

// JSON 객체
DynamicJsonDocument bufJson(1024);
DynamicJsonDocument dataJson(1024);
JsonObject object = dataJson.to<JsonObject>();
JsonObject outer = object.createNestedObject("outer");
JsonObject inner = object.createNestedObject("inner");
String bufStr;


// 전송할 실외 아두이노 데이터
String dataNames[TDATA_CNT] = {"데이터1", "데이터2", "데이터3"};
float data[TDATA_CNT] = {1.1, 2.2, 3.3};

boolean isAuto = true;  // 자동 모드 여부
int id = -1;            // client id
int cmd = -1;           // 아두이노 명령

void setup()
{
  Serial.begin(9600);

  // 와이파이 연결
  Serial.println();
  Serial.println("** SERVER JSON TEST **");

  Serial.print("- Connecting AP ");
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" Ready");
  Serial.print("- IP address: ");
  ip = WiFi.localIP();
  Serial.println(ip);


  // 서버 설정
  Serial.println("- Setting Server :" + ip.toString());
  server.begin();
}

void loop()
{
  Serial.println("- Setting Data");
  for (int i = 0; i < TDATA_CNT; i++) {
    outer[dataNames[i]] = data[i];
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
            for (int i = 0; i < TDATA_CNT; i++)
            {
              inner[dataNames[i]] = bufJson[dataNames[i]];
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

                Serial.print(bufStr);
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
                break;
              case STRONG_VT:
                Serial.println("Ventilate strongly...");
                break;
              default:
                Serial.println("Unkown usage");
                client.println("Unkown usage");

            }
            break;

          default:
            Serial.println("- Unkown User");
            client.println("- Unkown User");
            break;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("- Client disconnected");
  }

}
/*
*/
