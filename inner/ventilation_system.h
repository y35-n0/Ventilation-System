// 와이파이 설정

#define AP_SSID            "공유기 아이디"    // WiFi SSID
#define AP_PASS            "공유기 비밀번호"   // WiFi 비밀번호
#define HOST               "실외 IP 주소"       // 실외 아두이노 IP 주소
#define PORT                8090              // 실외 아두이노 포트 번호

// id - client 식별자

#define INNER_ARDUINO   0       // 실내 아두이노 ID
#define ANDROID         1       // 안드로이드 ID

// cmd - 안드로이드 명령

#define REQUEST         0       // 데이터 요청
#define AUTO            1       // 자동 조작
#define MANUAL          2       // 수동 조작  
#define WEAK_VT         3       // 약한 환기
#define STRONG_VT       4       // 강한 환기

// 데이터 전송 설정

#define TDATA_CNT     3              // 전송할 데이터 수
#define DATA_CNT      5             // 전송할 데이터 종류 (온도, 습도, 실외먼지, 유해가스, 가연성가스)

// 데이터 번호
#define HUM         0
#define TEM         1
#define DUS         2
#define GAS         3
#define CO2         4

// 센서 핀번호 및 관련 설정

#define DHT_PIN       16            // A2, 온습도 센서
#define DHT_TYPE      DHT11         // 온습도 사용하는 센서 종류

#define DUST_PIN      15            // A1, 먼지 센서
#define DUST_LED_PIN  2             // PD2, 먼지 LED
#define SAMPLING_TIME 280           // 먼지 측정 관련 시간, micro 단위
#define DELTA_TIME    40            // 먼지 측정 관련 시간, micro 단위
#define SLEEP_TIME    9680          // 먼지 측정 관련 시간, micro 단위

#define CO2_PIN       14            // A0, CO2 센서

#define GAS_PIN       17            // A3, 가스 센서

#define MTA_PIN       9            // 모터A                   
#define MTB_PIN       8            // 모터B    

// 측정 관련 설정
#define DELAY_TIME    2000          // 측정, 대기 시간
#define READING_TIMES 5             // 측정 횟수
