/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 5 , Number 3 */

#include <wiringPi.h> // GPIO access library 헤더파일 선언
#include <softPwm.h> // Software PWM library 헤더파일 선언

#define MOTOR_MT_P_PIN 4  // MOTOR 포트 MT_P 핀 정의
#define MOTOR_MT_N_PIN 17 // MOTOR 포트 MT_N 핀 정의

#define LEFT_ROTATE 1     // 반시계방향 회전 상수 정의
#define RIGHT_ROTATE 2    // 시계방향 회정 상수 정의

void MotorStop(void); // 모터 제어 관련 함수 프로토타입 정의
void MotorControl(unsigned char speed, unsigned char rotate);

int main(void)
{
    if(wiringPiSetupGpio() == -1) // Wiring Pi의 GPIO를 사용하기 위한 설정
    {
        return 1;
    }

    pinMode(MOTOR_MT_N_PIN, OUTPUT); // MT_N 핀 출력 설정
    pinMode(MOTOR_MT_P_PIN, OUTPUT); // MT_P 핀 출력 설정

    softPwmCreate(MOTOR_MT_N_PIN, 0, 100); // MT_N 핀 PWM 설정
    softPwmCreate(MOTOR_MT_P_PIN, 0, 100); // MT_P 핀 PWM 설정

    while(1)
    {
        MotorControl(30, LEFT_ROTATE);  // 모터 제어
        delay(2000);                    // Duty Ratio 30% , 반시계방향 회전
        MotorStop();                    // 모터 정지

        MotorControl(80, RIGHT_ROTATE); // 모터 제어
        delay(2000);                    // Duty Ratio 80% , 시계방향 회전
        MotorStop();                    // 모터 정지

        delay(5000);
    }

    return 0;
}

void MotorStop()
{
    softPwmWrite(MOTOR_MT_N_PIN, 0);
    softPwmWrite(MOTOR_MT_P_PIN, 0);
}

void MotorControl(unsigned char speed, unsigned char rotate)
{
    if(rotate == LEFT_ROTATE)
    {
        digitalWrite(MOTOR_MT_P_PIN, LOW);
        softPwmWrite(MOTOR_MT_N_PIN, speed);
    }
    else if(rotate == RIGHT_ROTATE)
    {
        digitalWrite(MOTOR_MT_N_PIN, LOW);
        softPwmWrite(MOTOR_MT_P_PIN, speed);
    }
}
