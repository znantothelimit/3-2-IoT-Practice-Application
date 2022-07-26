/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 5 , Number 2 */

#include <wiringPi.h> // GPIO Access Library 헤더파일 포함

#define LED_PIN_1 5   // LED 포트 RED 핀 정의
#define LED_PIN_2 6

#define KEYPAD_PB1 23 // KEYPAD 포트 BT1 핀 정의
#define KEYPAD_PB2 24

#define LED_ON 1      // LED ON/OFF 정의
#define LED_OFF 0

#define MAX_LED_NUM 2 // LED 개수 및 KEYPAD 버튼 수 정의
#define MAX_KEY_BT_NUM 2

const int LedPinTable[2] = { LED_PIN_1, LED_PIN_2 };   // LED 핀 테이블 선언
const int KeypadTable[2] = { KEYPAD_PB1, KEYPAD_PB2 }; // KEYPAD 핀 테이블 선언

int KeypadRead(void) // KEYPAD 버튼 눌림을 확인하여 nKeypadstate 상태 변수에 값을 저장
{
    int i, nKeypadstate;
    nKeypadstate = 0;

    for(i=0; i<MAX_KEY_BT_NUM; i++)
    {
        if(!digitalRead(KeypadTable[i]))
        {
            nKeypadstate |= (1<<i);
        }
    }

    return nKeypadstate;
}

void LedControl(int LedNum, int Cmd) // 해당하는 LED 번호와 ON/OFF 명령 변수를 확인하여 LED 핀에 HIGH/LOW 신호를 출력
{
    int i;

    for(i=0; i<MAX_LED_NUM; i++)
    {
        if(i == LedNum)
        {
            if(Cmd == LED_ON)
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

int main(void)
{
    if(wiringPiSetupGpio() == -1) // Wiring Pi의 GPIO를 사용하기 위한 설정
    {
        return 1;
    }

    int i;
    int nKeypadstate;

    for(i=0; i<MAX_LED_NUM; i++)    // LED 핀 출력 설정
    {
        pinMode(LedPinTable[i], OUTPUT);
    }

    for(i=0; i<MAX_KEY_BT_NUM; i++) // KEYPAD 핀 입력 설정
    {
        pinMode(KeypadTable[i], INPUT);
    }

    while(1)
    {
        nKeypadstate = KeypadRead();    // KEYPAD로부터 버튼 입력을 읽어 상태를 변수에 저장

        for(i=0; i<MAX_KEY_BT_NUM; i++) // KEYPAD 상태 변수를 확인하여 LED ON/OFF 제어
        {
            if(nKeypadstate & (1<<i))
            {
                LedControl(i, LED_ON);
            }
            else
            {
                LedControl(i, LED_OFF);
            }
        }
    }

    return 0;
}
