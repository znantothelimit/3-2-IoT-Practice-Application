/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 5 , Number 4 */

#include <wiringPi.h> // GPIO access library 헤더파일 선언
#include <softPwm.h> // Software PWM library 헤더파일 선언

#define LED_PIN_1 5  // LED 포트 RED1 핀 정의
#define LED_PIN_2 6  // LED 포트 RED2 핀 정의
#define LED_PIN_3 13 // LED 포트 RED3 핀 정의

#define KEYPAD_PB1 23 // KEYPAD 포트 BT1 핀 정의
#define KEYPAD_PB2 24 // KEYPAD 포트 BT2 핀 정의
#define KEYPAD_PB3 25 // KEYPAD 포트 BT3 핀 정의
#define KEYPAD_PB4 8  // KEYPAD 포트 BT4 핀 정의
#define KEYPAD_PB5 7  // KEYPAD 포트 BT5 핀 정의

#define LED_ON 1  // LED ON 정의
#define LED_OFF 0 // LED OFF 정의

#define MAX_LED_NUM 3    // LED 개수 정의
#define MAX_KEY_BT_NUM 5 // KEYPAD 버튼 개수 정의

#define MOTOR_MT_P_PIN 4  // MOTOR 포트 MT_P 핀 정의
#define MOTOR_MT_N_PIN 17 // MOTOR 포트 MT_N 핀 정의

#define LEFT_ROTATE 1  // 반시계방향 회전 상수 정의
#define RIGHT_ROTATE 2 // 시계방향 회전 상수 정의

const int LedPinTable[3] = { LED_PIN_1, LED_PIN_2, LED_PIN_3 };    // LED 핀 테이블 선언
const int KeypadTable[5] = { KEYPAD_PB1, KEYPAD_PB2,
                             KEYPAD_PB3, KEYPAD_PB4, KEYPAD_PB5 }; // KEYPAD 핀 테이블 선언

int KeypadRead(void) // KEYPAD 버튼 눌림을 확인하여 nKeypadstate 상태 변수에 값을 저장
{
    int i = 0;            // 반복문 사용을 위한 정수형 변수 i, 0으로 초기화
    int nKeypadstate = 0; // KEYPAD 상태를 위한 정수형 변수 nKeypadstate, 0으로 초기화

    for(i=0; i<MAX_KEY_BT_NUM; i++)      // 반복문 for, 매개변수 i, 0부터 시작하여 KEYPAD 버튼 최대 개수인 5 미만까지 +1, 아래 코드 반복
    {
        if(!digitalRead(KeypadTable[i])) // 조건문 if, KEYPAD 핀 테이블의 i번째 위치 신호를 읽어서 반전했을때(버튼의 동작 범위 지정)
        {
            nKeypadstate |= (1<<i);      // 현재 i의 값을 1 Left Shift하여 nKeypadstate 값과 OR연산 후 그 결과값을 저장
        }
    }

    return nKeypadstate; // KEYPAD 상태 값 반환
}

void LedControl(int LedNum, int Cmd) // 해당하는 LED 번호와 ON/OFF 명령 변수를 확인하여 LED 핀에 HIGH/LOW 신호를 출력
{
    int i; // 반복문 사용을 위한 정수형 변수 i 선언

    for(i=0; i<MAX_LED_NUM; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 LED 최대 개수인 3 미만까지 +1, 아래 코드 반복
    {
        if(i == LedNum)          // 조건문 if, i와 LED 번호가 일치하면 아래 코드 실행
        {
            if(Cmd == LED_ON)                       // 조건문 if, 명령 변수가 ON이라면 아래 코드 실행
            {
                digitalWrite(LedPinTable[i], HIGH); // LED 핀 테이블의 i번째 위치에 HIGH 신호 할당
            }
            else                                    // 조건문 if-else, 명령 변수가 OFF라면 아래 코드 실행
            {
                digitalWrite(LedPinTable[i], LOW);  // LED 핀 테이블의 i번째 위치에 LOW 신호 할당
            }
        }
    }
}

void MotorStop(void) // 모터 제어 정지 함수 정의
{
    softPwmWrite(MOTOR_MT_N_PIN, 0); // DC 모터 N핀에 PWM Duty Ratio 0% 할당
    softPwmWrite(MOTOR_MT_P_PIN, 0); // DC 모터 P핀에 PWM Duty Ratio 0% 할당
}

void MotorControl(unsigned char speed, unsigned char rotate) // 모터 제어 조작 함수 정의, 속도와 회전 방향을 매개변수로 사용
{
    if(rotate == LEFT_ROTATE)                // 조건문 if, 회전을 반시계방향으로 설정했다면 아래 코드 실행
    {
        digitalWrite(MOTOR_MT_P_PIN, LOW);   // DC 모터 P핀에 LOW 신호 할당
        softPwmWrite(MOTOR_MT_N_PIN, speed); // DC 모터 N핀에 주어진 속도만큼 PWM Duty Ratio 퍼센테이지 할당
    }
    else if(rotate == RIGHT_ROTATE)          // 조건문 if-elseif, 회전을 시계방향으로 설정했다면 아래 코드 실행
    {
        digitalWrite(MOTOR_MT_N_PIN, LOW);   // DC 모터 N핀에 LOW 신호 할당
        softPwmWrite(MOTOR_MT_P_PIN, speed); // DC 모터 P핀에 주어진 속도만큼 PWM Duty Ratio 퍼센테이지 할당
    }
}

int main(void) // 메인 함수 시작
{
    if(wiringPiSetupGpio() == -1) // Wiring Pi의 GPIO를 사용하기 위한 설정
    {
        return 1;                 // Wiring Pi의 GPIO가 설정되지 않았다면 종료
    }

    int i;                    // 반복문 사용을 위한 정수형 변수 i 선언
    int nKeypadstate;         // KEYPAD 상태를 위한 정수형 변수 nKeypadstate 선언
    int rotate;               // 회전 방향 설정을 위한 정수형 변수 rotate 선언
    int pwm = 0;              // PWM Duty Ratio를 위한 정수형 변수 pwm, 0%로 초기화
    int value = RIGHT_ROTATE; // 초기 방향 설정을 위한 정수형 변수 value, 시계방향으로 초기화

    pinMode(MOTOR_MT_N_PIN, OUTPUT); // MT_N 핀 출력 설정
    pinMode(MOTOR_MT_P_PIN, OUTPUT); // MT_P 핀 출력 설정

    softPwmCreate(MOTOR_MT_N_PIN, 0, 100); // MT_N 핀 PWM 설정
    softPwmCreate(MOTOR_MT_P_PIN, 0, 100); // MT_P 핀 PWM 설정

    for(i=0; i<MAX_LED_NUM; i++)         // LED 핀 출력 설정
    {
        pinMode(LedPinTable[i], OUTPUT); // LED 핀 테이블의 i번째 위치에 출력 설정 할당
    }
    for(i=0; i<MAX_KEY_BT_NUM; i++)      // KEYPAD 핀 입력 설정
    {
        pinMode(KeypadTable[i], INPUT);  // KEYPAD 핀 테이블의 i번째 위치에 입력 설정 할당
    }

    while(1) // 반복문 while, 무한 루프
    {
        nKeypadstate = KeypadRead(); // KEYPAD로부터 버튼 입력을 읽어 상태를 변수에 저장
        rotate = value;              // 회전 방향을 설정된 방향으로 지정

        for(i=0; i<MAX_KEY_BT_NUM; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 KEYPAD 버튼 최대 개수인 5 미만까지 +1, 아래 코드 반복
        {
            if((nKeypadstate & (1<<i))) // 조건문 if, 현재 i의 값을 1 Left Shift하여 nKeypadstate 값과 AND연산했을때(버튼의 위치 탐색)
            {
                switch(i) // 조건문 switch, 변수 i에 대하여 아래 케이스 코드 각각 실행
                {
                    case 0:                                  // i = 0 일 때
                        digitalWrite(LED_PIN_1, LED_OFF);    // LED 1 OFF
                        digitalWrite(LED_PIN_2, LED_OFF);    // LED 2 OFF
                        digitalWrite(LED_PIN_3, LED_OFF);    // LED 3 OFF
                        MotorStop();                         // DC 모터 정지
                        break;                               // 해당 케이스 탈출

                    case 1:                                  // i = 1 일 때
                        digitalWrite(LED_PIN_1, LED_ON);     // LED 1 ON
                        digitalWrite(LED_PIN_2, LED_OFF);    // LED 2 OFF
                        digitalWrite(LED_PIN_3, LED_OFF);    // LED 3 OFF
                        pwm = 30;                            // PWM Duty Ratio 30%
                        MotorControl(pwm, rotate);           // DC 모터 조작
                        value = rotate;                      // 현재 회전 방향 저장
                        break;                               // 해당 케이스 탈출
                    
                    case 2:                                  // i = 2 일 때
                        digitalWrite(LED_PIN_1, LED_ON);     // LED 1 ON
                        digitalWrite(LED_PIN_2, LED_ON);     // LED 2 ON
                        digitalWrite(LED_PIN_3, LED_OFF);    // LED 3 OFF
                        pwm = 60;                            // PWM Duty Ratio 60%
                        MotorControl(pwm, rotate);           // DC 모터 조작
                        value = rotate;                      // 현재 회전 방향 저장
                        break;                               // 해당 케이스 탈출
                    
                    case 3:                                  // i = 3 일 때
                        digitalWrite(LED_PIN_1, LED_ON);     // LED 1 ON
                        digitalWrite(LED_PIN_2, LED_ON);     // LED 2 ON
                        digitalWrite(LED_PIN_3, LED_ON);     // LED 3 ON
                        pwm = 90;                            // PWM Duty Ratio 90%
                        MotorControl(pwm, rotate);           // DC 모터 조작
                        value = rotate;                      // 현재 회전 방향 저장
                        break;                               // 해당 케이스 탈출
                    
                    case 4:                                  // i = 4 일 때
                        if(rotate == LEFT_ROTATE)            // 회전을 반시계방향으로 설정했다면 아래 코드 실행
                        {
                            MotorStop();                     // DC 모터 정지
                            delay(2000);                     // 2000ms 딜레이
                            MotorControl(pwm, RIGHT_ROTATE); // DC 모터 조작
                            rotate = RIGHT_ROTATE;           // 회전을 시계방향으로 변경
                            value = rotate;                  // 현재 회전 방향 저장
                        }
                        else if(rotate == RIGHT_ROTATE)      // 회전을 시계방향으로 설정했다면 아래 코드 실행
                        {
                            MotorStop();                     // DC 모터 정지
                            delay(2000);                     // 2000ms 딜레이
                            MotorControl(pwm, LEFT_ROTATE);  // DC 모터 조작
                            rotate = LEFT_ROTATE;            // 회전을 반시계방향으로 변경
                            value = rotate;                  // 현재 회전 방향 저장
                        }
                        break;                               // 해당 케이스 탈출
                }
            }
        }
    }
}
