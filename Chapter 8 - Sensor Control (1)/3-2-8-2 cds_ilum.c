/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 8 , Number 2 */

#include <stdio.h>       // 입출력 관련 헤더파일 포함
#include <string.h>      // 문자열 관련 헤더파일 포함
#include <errno.h>       // 에러 관련 헤더파일 포함
#include <wiringPi.h>    // GPIO access library 헤더파일 포함
#include <wiringPiSPI.h> // ADC SPI 통신 헤더파일 포함 (SPI : 서로 다른 컴퓨터 간 데이터 통신을 위한 규격, Master-Slave 구조의 전이중 통신)

#define CS_MCP3208 8      // MCP3208 CS 핀 정의
#define SPI_CHANNEL 0     // SPI 통신 채널 정의
#define SPI_SPEED 1000000 // SPI 통신 속도 정의

int ReadMcp3208ADC(unsigned char adcChannel) // MCP3208을 통한 A/D Converter Read 함수
{
    unsigned char buff[3];                       // 3 Bytes Command Data
    int nAdcValue = 0;                           

    /* Rpi to ADC 송신(Din) */
    buff[0] = 0x06 | ((adcChannel & 0x07) >> 2); // 0 0 0 0 0 Start SGL D2 (start와 sgl은 1 set해주고, d2값은 채널에 따라)
    buff[1] = ((adcChannel & 0x07) << 6);        // D1 D0 x x x x x x (d1, d0값은 채널에 따라, 현채널 0이므로 d2 d1 d0 모두 0
    )buff[2] = 0x00;                              // x x x x x x x x

    digitalWrite(CS_MCP3208, 0); // Chip Select Low 신호 출력

    /* ADC to Rpi 수신(Dout) */
    wiringPiSPIDataRW(SPI_CHANNEL, buff, 3); // 설정한 채널로부터 데이터 획득

    buff[1] = 0x0F & buff[1];             // 0 0 0 0 B11 B10 B9 B8 (하위 4자리 masking)  
        // BUFF[2]에는 현재 B7 B6 B5 B4 B3 B2 B1 B1 저장되어있음
    nAdcValue = (buff[1] << 8) | buff[2]; // buff 8bit 밀어줘서 자릿수 맞춰주고 OR연산해서 자릿수 그대로 내려오도록 12비트 변환 값 저장

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
