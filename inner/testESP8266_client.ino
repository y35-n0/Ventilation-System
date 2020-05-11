// Client 테스트
/*
#include <ESP8266WiFi.h>
#include "ventilation_system.h"

WiFiClient client;
IPAddress ip;

int i = 0;
String messages[TDATA_CNT] = {"1\r\n", "2\r\n", "3\r\n"};

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
    if (i < TDATA_CNT - 1) {
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
