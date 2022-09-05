/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 9 , Number 5 */

#include <stdio.h>       // 표준입출력 라이브러리 헤더파일 선언
#include <string.h>      // 문자열 라이브러리 헤더파일 선언
#include <error.h>       // 오류 라이브러리 헤더파일 선언
#include <wiringPi.h>    // GPIO access library 헤더파일 선언
#include <wiringPiSPI.h> // GPIO SPI library 헤더파일 선언
#include <softTone.h>    // Tone wave signal 관련 헤더파일 선언
#include <lcd.h>         // Text LCD(Character LCD) 관련 헤더파일 선언

#define BUZZER_PIN 18 // Buzzer Pin 정의
#define FREQ 1500     // 주파수 값 정의

#define CS_MCP3208 8      // ADC Pin 정의
#define SPI_CHANNEL 0     // SPI Channel 정의
#define SPI_SPEED 1000000 // SPI 통신 속도 정의

#define TP 12 // Trigger 핀 정의
#define EP 16 // Echo 핀 정의

void Change_FREQ(unsigned int freq)  // 주파수를 변경하는 함수
{
    softToneWrite(BUZZER_PIN, freq); // Buzzer 핀에 주어진 주파수만큼 할당
}

void STOP_FREQ(void)              // 주파수 발생 정지 함수
{
    softToneWrite(BUZZER_PIN, 0); // Buzzer 핀에 주파수 0 Hz 할당
}

void BUZZER_Init(void)          // Buzzer 초기 설정
{
    softToneCreate(BUZZER_PIN); // Buzzer 핀에 주파수 신호 출력 설정
    STOP_FREQ();                // 주파수 발생 정지
}

int ReadMcp3208ADC(unsigned char adcChannel) // MCP3208을 통한 A/D Converter Read 함수
{
    unsigned char buff[3];                       // 3 Bytes Command Data
    int nAdcValue = 0;                           // 0 0 0 0 0 Start SGL D2

    buff[0] = 0x06 | ((adcChannel & 0x07) >> 2); // D1 D0 x x x x x x
    buff[1] = ((adcChannel & 0x07) << 6);        // x x x x x x x x
    buff[2] = 0x00;                              // Chip Select Low 신호 출력

    digitalWrite(CS_MCP3208, 0);             // 설정한 채널로부터 데이터 획득
    wiringPiSPIDataRW(SPI_CHANNEL, buff, 3); // 획득한 데이터를 버퍼에 설정한 길이로 임시 저장

    buff[1] = 0x0F & buff[1];             // 하위 4자리 masking
    nAdcValue = (buff[1] << 8) | buff[2]; // 12비트 변환 값 저장

    digitalWrite(CS_MCP3208, 1); // Chip Select High 신호 출력

    return nAdcValue; // 변환 값 반환
}

float getDistance(void) // 초음파 센서를 이용한 거리 측정 함수
{
    float fDistance;          // 거리 값 변수
    int nStartTime, nEndTime; // 측정 시간 변수

    digitalWrite(TP, LOW);  // TP에 LOW(0) 출력
    delayMicroseconds(2);   // pull the Trig pin to high level for more than 10us impulse
    digitalWrite(TP, HIGH); // TP에 HIGH(1) 출력
    delayMicroseconds(10);  // 10μs 딜레이
    digitalWrite(TP, LOW);  // TP에 LOW(0) 출력

    while(digitalRead(EP) == LOW) // EP를 읽어 LOW인 동안 대기
    {
        nStartTime = micros(); // EP가 HIGH일 때, 시간 측정
    }

    while(digitalRead(EP) == HIGH) // EP를 읽어 HIGH인 동안 대기
    {
        nEndTime = micros(); // EP가 LOW일 때, 시간 측정
    }

    fDistance = (nEndTime - nStartTime) * 0.034 / 2.; // EP가 HIGH인 시간을 측정하여 거리 산출 (cm)

    return fDistance; // 거리 값 반환
}

int main(void) // 메인 함수 시작
{
    if(wiringPiSetupGpio() == -1) // Wiring Pi의 GPIO를 사용하기 위한 설정
    {
        fprintf(stdout, "Not start wiringPi : %s\n", strerror(errno)); // Wiring Pi가 설정되지 않았다면 에러 출력
        return 1;                                                      // 종료
    }
    if(wiringPiSetupSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) // Wiring Pi의 SPI를 사용하기 위한 설정
    {
        fprintf(stdout, "Not start wiringPi : %s\n", strerror(errno)); // Wiring Pi가 설정되지 않았다면 에러 출력
        return 1;                                                      // 종료
    }

    BUZZER_Init(); // Buzzer 초기화

    int disp; // LCD 사용을 위한 정수형 변수 disp 선언
    disp = lcdInit(2, 16, 4, 18, 23, 12, 16, 20, 21, 0, 0, 0, 0); // LCD 초기화

    float fDistance; // 초음파 센서 사용을 위한 실수형 변수 fDistance 선언
    char dist[10];   // 측정된 거리 저장을 위한 문자형 배열 dist, 크기는 10

    pinMode(TP, OUTPUT); // Trigger 핀 출력 설정
    pinMode(EP, INPUT);  // Echo 핀 출력 설정

    while(1) // 반복문 while, 무한 루프
    {
        fDistance = getDistance(); // 거리를 측정 후 변수에 저장

        if((fDistance <= 100) && (fDistance > 70)) // 거리가 100 이하 70 초과일 시, 
        {
            Change_FREQ(FREQ); // 주파수 출력
            delay(100);        // 100ms 동안 지속
            STOP_FREQ();       // 소리 출력 정지
            delay(900);        // 900ms 딜레이
        }
        else if((fDistance <= 70) && (fDistance > 50)) // 거리가 70 이하 50 초과일 시, 
        {
            Change_FREQ(FREQ); // 주파수 출력
            delay(100);        // 100ms 동안 지속
            STOP_FREQ();       // 소리 출력 정지
            delay(400);        // 400ms 딜레이
        }
        else if((fDistance <= 50) && (fDistance > 30)) // 거리가 50 이하 30 초과일 시, 
        {
            Change_FREQ(FREQ); // 주파수 출력
            delay(100);        // 100ms 동안 지속
            STOP_FREQ();       // 소리 출력 정지
            delay(150);        // 150ms 딜레이
        }
        else if(fDistance <= 30) // 거리가 30 이하일 시, 
        {
            Change_FREQ(FREQ); // 주파수 출력
            delay(100);        // 50ms 동안 지속
            STOP_FREQ();       // 소리 출력 정지
            delay(1);          // 150ms 딜레이
        }

        sprintf(dist, "%.2f", fDistance); // 측정된 거리값을 설정 형식으로 배열에 저장

        lcdClear(disp);          // LCD 클리어
        lcdPosition(disp, 0, 0); // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
        lcdPuts(disp, dist);     // LCD에 문자열 표시
        lcdPosition(disp, 5, 0); // LCD 위치 상단라인(0번째 줄) 5번째 셀로 조정
        lcdPuts(disp, "cm");     // LCD에 문자열 표시
    }

    return 0; // 프로그램 종료
}
