// Client 테스트
/*
#include <ESP8266WiFi.h>

#define AP_SSID "공유기 아이디"    // WiFi SSID
#define AP_PASS "공유기 비밀번호"   // WiFi 비밀번호
#define HOST "실외 IP 주소"       // 실외 아두이노 IP 주소
#define MSG_MAX 3
WiFiClient client;
IPAddress ip;

int i = 0;
String messages[MSG_MAX] = {"1\r\n", "2\r\n", "3\r\n"};

void setup()
{
  Serial.begin(9600);

  // 와이파이 연결
  Serial.println();
  Serial.println("** CLIENT TEST **");

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
  // 서버 연결
  Serial.printf("- Connecting Server %s ... ", HOST);

  if (client.connect(HOST, 80))  {
    Serial.println("Ready");

    // 메세지 전송
    Serial.println("- Request: " + messages[i]);
    client.print(messages[i]);
    if (i < MSG_MAX - 1) {
      i++;
    } else {
      i = 0;
    }

    // 응답 확인
    Serial.println("- Response: ");
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(line);
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
