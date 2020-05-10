// 먼지 센서 테스트
/*
#define DUST_PIN 15       // A1
#define DUST_LED_PIN 2    // PD2

#define DELAY_TIME 500

float voMeasured = 0.0;
float calcVoltage = 0.0;
float dustDensity = 0.0;

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("** MEASURE DUST TEST **");

  pinMode(DUST_PIN, INPUT);
  pinMode(DUST_LED_PIN, OUTPUT);
}


void loop()
{
  // 측정
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(DUST_PIN);
  delayMicroseconds(deltaTime);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(sleepTime);

  // 수치 보정 계산
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = 0.17 * calcVoltage - 0.1;

  // 결과 출력
  Serial.print("- Raw Dust Value (0-1023): " + String(voMeasured));
  Serial.print(" - Voltage: " + String(calcVoltage)); 
  Serial.println("- Dust Density: " + String(dustDensity) + " mg/m3");
}
*/
