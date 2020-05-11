// Server JSON 요청 및 전송 테스트

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define AP_SSID "공유기 아이디"    // WiFi SSID
#define AP_PASS "공유기 비밀번호"   // WiFi 비밀번호
#define PORT 8090               // 서버 포트 번호
#define DATA_CNT_MAX 6          // 데이터 개수

// ID 값
#define INNER_ARDUINO   0       // 실내 아두이노 ID
#define ANDROID         1       // 안드로이드 ID

// CMD 값
#define REQUEST         0       // 데이터 요청
#define AUTO            1       // 자동 조작
#define MANUAL          2       // 수동 조작  
#define WEAK_VT         3       // 약한 환기
#define STRONG_VT       4       // 강한 환기

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
String dataNames[DATA_CNT_MAX] = {"데이터1", "데이터2", "데이터3"};
float data[DATA_CNT_MAX] = {1.1, 2.2, 3.3};

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
  for (int i = 0; i < DATA_CNT_MAX; i++) {
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
            for (int i = 0; i < DATA_CNT_MAX; i++)
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
