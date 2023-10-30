#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>

// 펄스 HIGH, LOW 카운트 총 80bit 송수신 위해 83으로 설정 (3bit의 준비 과정 소요)
#define MAXTIMINGS 83 
#define DHTPIN 27 // DHTPIN GPIO 핀 정의

// DHT11데이터를 저장할 배열, 각 데이터(요소) int형 8bit
int dht11_dat[5] = { 0, }; 

void read_dht11_dat()
{
    uint8_t laststate = HIGH; // 이전 상태 저장할 변수, HIGH로 초기화
    uint8_t counter = 0; // 시간 측정을 위한 카운터(ms 단위)
    uint8_t j = 0; // 데이터를 읽기 위한 인덱스 변수, 데이터 8bit에서 bit인덱스
    uint8_t i;

    // DHT11 데이터 배열 각 요소를 0으로 초기화
    /* dht11_dat[0] 습도(정수), dht11_dat[1] : 습도(소수) 
       dht11_dat[2] : 온도(정수), dht11_dat[3] : 온도(소수)
       dht11_dat[4] : checksum */
    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    // DHT11 센서 핀 설정(라즈베리(mcu) -> DHT11 ; 출력)
    pinMode(DHTPIN, OUTPUT); 
    // MCU 시작 신호 전송(18ms의 LOW 신호)
    digitalWrite(DHTPIN, LOW);
    delay(18);

    // MCU 시작 신호 받으면 20us의 응답
    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(30);
    // 이후 DHT11 센서 핀 설정 (DHT11 -> 라즈베리 ; 입력)
    pinMode(DHTPIN, INPUT);

    for(i=0; i<MAXTIMINGS; i++)
    {
        counter = 0; // 온 펄스 당 시간 측정을 위한 counter로 반복마다 0으로 초기화됨

        // 읽어온 dht11 신호와 이전 상태와 동일할 시 반복(현재 읽어들여온 신호의 카운터)
        while(digitalRead(DHTPIN) == laststate) 
        {
            counter++; // 시간 카운터 증가
            delayMicroseconds(1); // 1ms마다 시간 카운트

            if(counter == 200)
            {
                break; // 만약 동일한 채로 200ms되면 break;
            }
        }

        // 이전 상태에 일단 읽어온 데이터를 집어넣음
        laststate = digitalRead(DHTPIN);

        if(counter == 200)
        {
            break; // 만약 200ms되면 break;
        }

        // 처음 3개 준비 신호 버리고, 짝수 펄스만 받음 -> pulse가 HIGH일때만 보겠다
        if((i >= 4) && ((i % 2) == 0)) 
        {
            dht11_dat[j / 8] <<= 1; // 해당 필터링된 신호 8bit 변수 안에 0번째부터 1bit씩 좌측 시프트, 초기에 0이므로 

            // 만약 pulse 폭이 20us 이상이다? -> 너 1이네
            if(counter > 20)
            {
                dht11_dat[j / 8] |= 1;  // |= : 비트OR 연산자, 만약 들어온 신호가 1이면, 00000001이랑 비트 OR연산해서 맨 우측자리 1로 만들어줄게
            }

            j++; // dht11_dat[j/8] 인덱스 증가, 만약 j가 9야 그럼 그냥 두 번째 요소 dht11_dat[1] 참조인거야 
        }
    }

    /* j가 40 이상(5(배열 크기) * 8bit)이고, 
       체크섬값이 일치하면(데이터 모두 더한 것과32bit 모두 1로 채워진 0xff와 AND연산했을 때 일치)하면 출력*/
    if((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xff)))
    {
        printf("humidity = %d.%d %% Temperature = %d.%d *C\n", dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
    }
    else
    {
        printf("Data get failed\n");
    }
}

int main(void)
{
    printf("dht11 Raspberry pi\n");

    if(wiringPiSetupGpio() == -1)
    {
        return 1;
    }

    while(1)
    {
        read_dht11_dat();
        delay(2000);
    }

    return 0;
}
