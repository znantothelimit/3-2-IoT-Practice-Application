#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

#define LED_PIN_1 5
#define LED_PIN_2 6
#define LED_PIN_3 13
#define KEYPAD_PB1 20
#define KEYPAD_PB2 21
#define KEYPAD_PB3 22
#define KEYPAD_PB4 23
#define KEYPAD_PB5 24
#define LED_ON 1
#define LED_OFF 0
#define MAX_LED_NUM 3
#define MAX_KEY_BT_NUM 5
const int LedPinTable[3] = {LED_PIN_1, LED_PIN_2, LED_PIN_3};
const int KeypadTable[5] = {KEYPAD_PB1, KEYPAD_PB2, KEYPAD_PB3, KEYPAD_PB4, KEYPAD_PB5};

#define MOTOR_MT_P_PIN 4
#define MOTOR_MT_N_PIN 17
#define LEFT_ROTATE 1
#define RIGHT_ROTATE 2

int KeypadRead(void)
{
    int i, nKeypadstate;
    nKeypadstate = 0;

    for (i = 0; i < MAX_KEY_BT_NUM; i++)
    {
        if (!digitalRead(KeypadTable[i]))
        {
            nKeypadstate |= (1 << i);
        }
    }

    return nKeypadstate;
}

void LedControl(int LedNum, int Cmd)
{
    int i;

    for (i = 0; i < MAX_LED_NUM; i++)
    {
        if (i == LedNum)
        {
            if (Cmd == LED_ON)
            {
                digitalWrite(LedPinTable[i], HIGH);
            }
            else
            {
                digitalWrite(LedPinTable[i], LOW);
            }
        }
    }
}

void MotorStop()
{
    softPwmWrite(MOTOR_MT_N_PIN, 0);
    softPwmWrite(MOTOR_MT_P_PIN, 0);
}

void MotorControl(unsigned char speed, unsigned char rotate)
{
    if (rotate == LEFT_ROTATE)
    {
        digitalWrite(MOTOR_MT_P_PIN, LOW);
        softPwmWrite(MOTOR_MT_N_PIN, speed);
    }
    else if (rotate == RIGHT_ROTATE)
    {
        digitalWrite(MOTOR_MT_N_PIN, LOW);
        softPwmWrite(MOTOR_MT_P_PIN, speed);
    }
}

void off_led()
{
    for (int j = 0; j < MAX_LED_NUM; j++)
    {
        LedControl(j, LED_OFF);
    }
}

int main()
{
    if (wiringPiSetupGpio() == -1)
        return 1;
    int nKeypadstate;
    int i;
    int rot = 0;
    int past_i = 0;
    for (i = 0; i < MAX_LED_NUM; i++)
    {
        pinMode(LedPinTable[i], OUTPUT);
    }

    for (i = 0; i < MAX_KEY_BT_NUM; i++)
    {
        pinMode(KeypadTable[i], INPUT);
    }

    pinMode(MOTOR_MT_N_PIN, OUTPUT);
    pinMode(MOTOR_MT_P_PIN, OUTPUT);
    softPwmCreate(MOTOR_MT_N_PIN, 0, 100);
    softPwmCreate(MOTOR_MT_P_PIN, 0, 100);

    while (1)
    {
        nKeypadstate = KeypadRead();

        for (i = 0; i < MAX_KEY_BT_NUM; i++)
        {
            if ((nKeypadstate & (1 << i)))
            {
                if (i == 0)
                {
                    // 1번스위치는 종료
                    for (int j = 0; j < MAX_LED_NUM; j++)
                    {
                        LedControl(j, LED_OFF);
                    }
                    MotorStop();
                    past_i = i;
                }
                else if (i >= 1 && i <= 3)
                {
                    // 2번, 3번, 4번 버튼: 바람세기 설정
                    int speed = i * 20; // 바람세기 설정 (20, 40, 60)
                    if (rot == 0)
                    {
                        MotorControl(speed, RIGHT_ROTATE); // 시계방향 회전
                    }
                    else
                    {
                        MotorControl(speed, LEFT_ROTATE); // 반시계방향 회전
                    }
                    off_led(); // 모든 LED 소등
                    for (int j = 0; j < i; j++)
                    {
                        LedControl(j, LED_ON); // 설정한 바람세기에 따라 LED 점등
                    }
                    past_i = i;
                }
                else if (i == 4)
                {
                    rot = !rot;
                    MotorStop();
                    delay(2000);
                    if (rot == 0)
                        MotorControl(past_i * 20, RIGHT_ROTATE);
                    else
                        MotorControl(past_i * 20, LEFT_ROTATE);
                    off_led();
                    LedControl(past_i - 1, LED_ON);
                    if (past_i == 1)
                    {
                        LedControl(past_i - 1, LED_ON);
                    }
                    else if (past_i == 2)
                    {
                        LedControl(past_i - 2, LED_ON);
                        LedControl(past_i - 1, LED_ON);
                    }
                    else if (past_i == 3)
                    {
                        LedControl(past_i - 3, LED_ON);
                        LedControl(past_i - 2, LED_ON);
                        LedControl(past_i - 1, LED_ON);
                    }
                }
                else
                    break;
            }
        }
    }

    return 0;
}
