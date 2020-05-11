// Client JSON 요청 테스트

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>        // 아두이노 JSON parsing

#define AP_SSID            "공유기 아이디"    // WiFi SSID
#define AP_PASS            "공유기 비밀번호"   // WiFi 비밀번호
#define HOST               "실외 IP 주소"       // 실외 아두이노 IP 주소
#define PORT                8090               // 실외 아두이노 포트 번
#define DATA_CNT_MAX        3              // 데이터 개수
#define INNER_ARDUINO       0              // 실내 아두이노 ID

WiFiClient client;
IPAddress ip;

// JSON 객체
DynamicJsonDocument bufJson(1024);
JsonObject dataJson = bufJson.createNestedObject("data");
String bufStr;

// 전송할 실내 아두이노 데이터
String dataNames[DATA_CNT_MAX] = {"데이터1", "데이터2", "데이터3"};
float data[DATA_CNT_MAX] = {0.1, 0.2, 3.0};

void setup()
{
  Serial.begin(9600);

  // 와이파이 연결
  Serial.println();
  Serial.println("** JSON REQUEST TEST **");

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

}

void loop()
{
  // Json 객체 설정
  Serial.println("- Setting Messages");
  bufJson["id"] = INNER_ARDUINO;
  for (int i = 0; i < DATA_CNT_MAX; i++) {
    dataJson[dataNames[i]] = data[i];
  }
  serializeJson(bufJson, bufStr);

  // 서버 연결
  Serial.printf("- Connecting Server %s ... ", HOST);

  if (client.connect(HOST, PORT))  {
    Serial.println("Ready");

    // 메세지 전송
    Serial.println("- Request: " + bufStr);
    client.print(bufStr);

    // 응답 확인
    Serial.println("- Response: ");
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        bufStr = client.readString();
        Serial.println("- Received Str : " + bufStr);
        deserializeJson(bufJson, bufStr);
        Serial.print("- Parsing Json: ");
        serializeJson(bufJson, Serial);
      }
    }
    client.stop();
    Serial.println("- Disconnected");

  } else {
    Serial.println("Failed!");
    client.stop();
  }

  delay(5000);

}
/*
*/
