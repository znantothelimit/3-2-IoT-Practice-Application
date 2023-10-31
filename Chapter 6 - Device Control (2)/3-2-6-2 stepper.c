/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 6 , Number 2 */

#include <stdio.h>
#include <wiringPi.h>

#define PINA 20
#define PINB 21
#define PINC 19
#define PIND 26

void setsteps(int w1, int w2, int w3, int w4)
{
    digitalWrite(PINA, w1);
    digitalWrite(PINB, w2);
    digitalWrite(PINC, w3);
    digitalWrite(PIND, w4);
}

/* 2상 자화 방식, 개방 루프 제어 : 현재 상태 고려 없이 명령 값을 주고 실제 값은 명령 값을 따라가고 있다고 생각하며 제어
   2상 자화 방식은 한 주기에 4 steps, step 당 1.8도 회전, 1회전 당 200 steps*/
void forward(int del, int steps)
{
    int i;

    for(i=0; i<steps; i++)
    {
        setsteps(1, 1, 0, 0); // A,B에 HIGH
        delay(del); 
        setsteps(0, 1, 1, 0); // B,C에 HIGH
        delay(del);
        setsteps(0, 0, 1, 1); // C,D에 HIGH
        delay(del);
        setsteps(1, 0, 0, 1); // D,A에 HIGH
        delay(del);
    }
}

// forward()함수와 역으로 신호 인가해주면 역방향 회전함
void backward(int del, int steps)
{
    int k;

    for(k=0; k<steps; k++)
    {
        setsteps(1, 0, 0, 1); // D,A에 HIGH
        delay(del);
        setsteps(0, 0, 1, 1); // C,D에 HIGH
        delay(del);
        setsteps(0, 1, 1, 0); // B,C에 HIGH
        delay(del);
        setsteps(1, 1, 0, 0); // A,B에 HIGH
        delay(del);
    }
}

int main(void)
{
    if(wiringPiSetupGpio() == -1)
    {
        return 1;
    }

    // 출력모드 설정
    pinMode(PINA, OUTPUT);
    pinMode(PINB, OUTPUT);
    pinMode(PINC, OUTPUT);
    pinMode(PIND, OUTPUT);

    for(;;) // 무한반복
    {
        forward(5, 512); // delay 5ms, 512 steps 
        delay(1000);
        backward(5, 512);
        delay(1000);
    }

    return 0;
}
