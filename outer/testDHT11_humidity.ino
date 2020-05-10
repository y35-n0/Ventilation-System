// 온습도 센서 테스트
/*
#include <DHT.h>

#define DHT_PIN 16 // A2
#define DHT_TYPE DHT11

#define DELAY_TIME 500

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  
  Serial.println();
  Serial.println("** READ DHT SENSOR TEST **");
  dht.begin();

}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();  
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read DHT sensor!");
    return;
  }

  Serial.print("Humidity: " + String(h) + "%");
  Serial.print("Temperature: " + String(t) + "C");

  delay(DELAY_TIME);
}
/*
 * 
 */
