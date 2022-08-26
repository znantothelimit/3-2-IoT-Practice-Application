/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 9 , Number 3 */

#include <stdio.h>    // 입출력 관련 헤더파일 포함
#include <wiringPi.h> // GPIO access library 헤더파일 포함

#define INPUT_PIN 27 // INPUT 핀 정의

int g_nPirState = LOW; // 이전 센싱 상태 저장 변수
int g_nVal = 0; // 센싱 값 저장 변수

int main(void)
{
    if(wiringPiSetupGpio() == -1) // Wiring Pi의 GPIO를 사용하기 위한 설정
    {
        return 1;
    }

    pinMode(INPUT_PIN, INPUT); // INPUT 핀 입력 설정

    while(1)
    {
        g_nVal = digitalRead(INPUT_PIN); // 센서로부터 입력 값 읽어서 변수에 저장

        if(g_nVal == HIGH) // 센싱 값이 HIGH인 경우, 
        {
            if(g_nPirState == LOW) // 이전 센싱 상태가 LOW인 경우, 
            {
                printf("Motion Detected!\n"); // 움직임 상태 검출 메시지 출력
            }

            g_nPirState = HIGH; // 이전 상태 변수에 현재 상태 저장
        }
        else // 센싱 값이 LOW인 경우, 
        {
            if(g_nPirState == HIGH) // 이전 센싱 상태가 HIGH인 경우, 
            {
                printf("Motion ended.\n"); // 움직임 상태 종료 메시지 출력
            }

            g_nPirState = LOW; // 이전 상태 변수에 현재 상태 저장
        }
    }

    return 0;
}
