// CO2 측정 테스트
/*
#include <MQ135.h>

#define CO2_PIN 14
//#define RLOAD 10.0
//#define RZERO 76.63     // 실외 CO2가 400ppm이 되도록 값 수정 필요
//#define PARA 116.6020682
//#define PARB 2.769034857

MQ135 mq(CO2_PIN);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("** MEASURE CO2 TEST **");

  pinMode(CO2_PIN, INPUT);
}

void loop()
{
  float ppm = mq.getPPM();
  //int co2Measured = analogRead(CO2_PIN);
  //float resistance = ((1023. / (float) co2Measured) * 5. - 1.) * RLOAD;
  //float ppm = PARA * pow((resistance / RZERO), -PARB);

  //Serial.print("- Raw CO2 Value: " + String(co2Measured));
  //Serial.print(" - Resistance: " + String(resistance));
  Serial.println("- CO2: " + String(ppm) + "PPM");

  delay(1000);
}
/*
*/
