/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 6 , Number 3 */

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

// 핀 꽂을 시 VCC가 red, GND가 brown!!!
#define SERVO 18 // 서보 모터 핀 제어

int main(void)
{
    char str;

    if(wiringPiSetupGpio() == -1)
    {
        return 1;
    }

    // 주기 200ms인 Pwm 생성 -> -90도(반시계방향) 회전 : 2.5%, 0도 회전 : 7.5%, +90도(시계방향) 회전 : 12.5%의 듀티비
    softPwmCreate(SERVO, 0, 200);

    while(1)
    {
        printf("\nCenter(c), Left(l), Right(r), Quit(q) : ");
        str = getchar();
        while(getchar() != '\n');

        if(str == 'c')
        {
            softPwmWrite(SERVO, 15); // 0도 회전 : 7.5% 듀티비 -> 200ms의 7.5%인 15ms
        }
        else if(str == 'r')
        {
            softPwmWrite(SERVO, 25); // +90도(시계방향) 회전 : 12.5% 듀티비 -> 200ms의 12.5%인 25ms 
        }
        else if(str == 'l')
        {
            softPwmWrite(SERVO, 5); // -90도(반시계방향) 회전 : 2.5% 듀티비 -> 200ms의 2.5인 5ms
        }
        else if(str == 'q')
        {
            return 0;
        }
    }

    return 0;
}
