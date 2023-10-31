/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 6 , Number 1 */

#include <wiringPi.h>

#define FAN_MT_P_PIN 20
#define FAN_MT_N_PIN 21

// FAN은 단방향 회전만 가능, 속도 제어 불가
void FanOn(void) // FAN 동작하도록 함 | P핀에 HIGH, N핀에 LOW 인가
{
    digitalWrite(FAN_MT_P_PIN, HIGH);
    digitalWrite(FAN_MT_N_PIN, LOW);
}

void FanOff(void) // FAN 정지토록 함 | P핀, N핀에 LOW 인가
{
    digitalWrite(FAN_MT_P_PIN, LOW);
    digitalWrite(FAN_MT_N_PIN, LOW);
}

int main(void)
{
    if(wiringPiSetupGpio() == -1)
    {
        return 1;
    }

    pinMode(FAN_MT_P_PIN, OUTPUT);
    pinMode(FAN_MT_N_PIN, OUTPUT);

    while(1)
    {
        FanOn();
        delay(2000);

        FanOff();
        delay(2000);
    }

    return 0;
}



