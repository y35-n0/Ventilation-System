// 와이파이 테스트
/*
#include <ESP8266WiFi.h>
#include "ventilation_system.h"

      
void setup()
{
  Serial.begin(9600);
  
  Serial.println();
  Serial.println("** CONNECT WIFI TEST **");
  
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

void loop()
{
  
}
/*
 */// 와이파이 테스트
/*
#include <ESP8266WiFi.h>

#define AP_SSID "공유기 아이디"   // WiFi SSID
#define AP_PASS "공유기 비밀번호"     // WiFi 비밀번호
      
void setup()
{
  Serial.begin(9600);
  
  Serial.println();
  Serial.println("** CONNECT WIFI TEST **");
  
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

void loop()
{
  
}
/*
 */
