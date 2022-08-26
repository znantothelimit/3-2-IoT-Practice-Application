/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 8 , Number 2 */

#include <stdio.h>       // 입출력 관련 헤더파일 포함
#include <string.h>      // 문자열 관련 헤더파일 포함
#include <errno.h>       // 에러 관련 헤더파일 포함
#include <wiringPi.h>    // GPIO access library 헤더파일 포함
#include <wiringPiSPI.h> // ADC SPI 통신 헤더파일 포함

#define CS_MCP3208 8      // MCP3208 CS 핀 정의
#define SPI_CHANNEL 0     // SPI 통신 채널 정의
#define SPI_SPEED 1000000 // SPI 통신 속도 정의

int ReadMcp3208ADC(unsigned char adcChannel) // MCP3208을 통한 A/D Converter Read 함수
{
    unsigned char buff[3];                       // 3 Bytes Command Data
    int nAdcValue = 0;                           // 0 0 0 0 0 Start SGL D2

    buff[0] = 0x06 | ((adcChannel & 0x07) >> 2); // D1 D0 x x x x x x
    buff[1] = ((adcChannel & 0x07) << 6);        // x x x x x x x x
    buff[2] = 0x00;                              // Chip Select Low 신호 출력

    digitalWrite(CS_MCP3208, 0); // 설정한 채널로부터 데이터 획득
    wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

    buff[1] = 0x0F & buff[1];             // 하위 4자리 masking
    nAdcValue = (buff[1] << 8) | buff[2]; // 12비트 변환 값 저장

    digitalWrite(CS_MCP3208, 1); // Chip Select High 신호 출력

    return nAdcValue; // 변환 값 반환
}

int main(void)
{
    int nCdsChannel = 0; // ADC 채널 변수 선언
    int nCdsValue = 0; // ADC 값 변수 선언

    if(wiringPiSetupGpio() == -1) // Wiring Pi의 GPIO를 사용하기 위한 설정
    {
        fprintf(stdout, "Not start wiringPi : %s\n", strerror(errno));

        return 1;
    }

    if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) // Wiring Pi의 SPI를 사용하기 위한 설정
    {
        fprintf(stdout, "wiringPiSPISetup Failed : %s\n", strerror(errno));

        return 1;
    }

    pinMode(CS_MCP3208, OUTPUT); // MCP3208 CS 핀 출력 설정

    while(1)
    {
        nCdsValue = ReadMcp3208ADC(nCdsChannel); // ADC 채널에 입력되는 값을 변환하여 ADC 값 변수에 저장

        printf("Cds Sensor Value = %u\n", nCdsValue); // 값 모니터링

        delay(1000);
    }

    return 0;
}
