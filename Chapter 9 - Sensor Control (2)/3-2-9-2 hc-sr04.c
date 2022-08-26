/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 9 , Number 2 */

#include <stdio.h>    // 입출력 관련 헤더파일 포함
#include <wiringPi.h> // GPIO access library 헤더파일 포함

#define TP 12 // Trigger 핀 정의
#define EP 16 // Echo 핀 정의

float getDistance(void) // 초음파 센서를 이용한 거리 측정 함수
{
    float fDistance;          // 거리 값 변수
    int nStartTime, nEndTime; // 측정 시간 변수

    digitalWrite(TP, LOW);  // TP에 LOW(0) 출력
    delayMicroseconds(2);   // pull the Trig pin to high level for more than 10us impulse
    digitalWrite(TP, HIGH); // TP에 HIGH(1) 출력
    delayMicroseconds(10);
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

int main(void)
{
    if(wiringPiSetupGpio() == -1) // wiring Pi의 GPIO를 사용하기 위한 설정
    {
        return 1;
    }

    pinMode(TP, OUTPUT); // Trigger 핀 출력 설정
    pinMode(EP, INPUT);  // Echo 핀 출력 설정

    while(1)
    {
        float fDistance = getDistance(); // 함수로부터 반환된 거리 값 저장

        printf("Distance : %.2f cm\n", fDistance); // 거리 값 모니터링

        delay(1000);
    }

    return 0;
}
