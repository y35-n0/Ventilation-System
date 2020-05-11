// Server 테스트
/*
#include <ESP8266WiFi.h>
#include "ventilation_system.h"

WiFiServer server(PORT);
IPAddress ip;

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


  // 서버 설정
  Serial.println("- Setting Server :" + ip.toString());
  server.begin();
}

void loop()
{
  // client와 연결
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("- Client connected");
    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        
        // request에 따라 응답을 달리함
        switch (line[0]) {
          case '1':
            Serial.println("Received : 1");
            client.println("Received : 1");
            break;

          case '2':
            Serial.println("Received : 2");
            client.println("Received : 2");
            break;

          default:
            Serial.println("Usage : ...");
            client.println("Usage : ...");
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
