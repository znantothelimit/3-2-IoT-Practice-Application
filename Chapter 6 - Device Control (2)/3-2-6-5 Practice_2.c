/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 6 , Number 5 */

#include <stdio.h>    // 표준입출력 라이브러리 헤더파일 선언
#include <wiringPi.h> // GPIO access library 헤더파일 선언
#include <softPwm.h>  // Software PWM library 헤더파일 선언
#include <softTone.h> // Tone wave signal 관련 헤더파일 선언

#define KEYPAD_PB1 2   // KEYPAD 포트 BT1 핀 정의 , 1
#define KEYPAD_PB2 3   // KEYPAD 포트 BT2 핀 정의 , 2
#define KEYPAD_PB3 4   // KEYPAD 포트 BT3 핀 정의 , 3
#define KEYPAD_PB4 17  // KEYPAD 포트 BT4 핀 정의 , 4
#define KEYPAD_PB5 27  // KEYPAD 포트 BT5 핀 정의 , 5
#define KEYPAD_PB6 22  // KEYPAD 포트 BT6 핀 정의 , 6
#define KEYPAD_PB7 10  // KEYPAD 포트 BT7 핀 정의 , 7
#define KEYPAD_PB8 9   // KEYPAD 포트 BT8 핀 정의 , 8
#define KEYPAD_PB9 11  // KEYPAD 포트 BT9 핀 정의 , 9
#define KEYPAD_PB10 5  // KEYPAD 포트 BT10 핀 정의 , * (아스테리스크)
#define KEYPAD_PB11 6  // KEYPAD 포트 BT11 핀 정의 , 0
#define KEYPAD_PB12 13 // KEYPAD 포트 BT12 핀 정의 , # (넘버 사인)

#define MAX_KEY_BT_NUM 12 // KEYPAD 버튼 개수 정의

#define SERVO 19 // DC Servo Motor PWM(Orange) 핀 정의

#define BUZZER_PIN 26 // Buzzer Pin 정의 , 7음계에서 각 음에 해당하는 주파수 값 정의
#define DO_L 523      // 낮은 도
#define RE 587        // 레
#define MI 659        // 미
#define FA 698        // 파
#define SOL 784       // 솔
#define RA 880        // 라
#define SI 987        // 시
#define DO_H 1046     // 높은 도

const int KeypadTable[12] = { KEYPAD_PB1, KEYPAD_PB2, KEYPAD_PB3, KEYPAD_PB4, KEYPAD_PB5,
                              KEYPAD_PB6, KEYPAD_PB7, KEYPAD_PB8, KEYPAD_PB9, KEYPAD_PB10,
                              KEYPAD_PB11, KEYPAD_PB12 }; // KEYPAD 핀 테이블 선언
const char number[12] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#' }; // Number 테이블 선언

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

unsigned int SevenScale(unsigned char scale) // 각 음계에 해당하는 주파수 값을 선택하는 함수
{
    unsigned int _ret = 0; // 음계 주파수 선택을 위한 부호 없는 정수형 변수 _ret, 0으로 초기화

    switch(scale) // 조건문 switch, 매개변수 scale에 대하여 아래 케이스 코드 각각 실행
    {
        case 0:          // scale = 0 일 때
            _ret = DO_L; // 음계 주파수 선택 : 낮은 도
            break;       // 해당 케이스 탈출
        
        case 1:          // scale = 1 일 때
            _ret = RE;   // 음계 주파수 선택 : 레
            break;       // 해당 케이스 탈출
        
        case 2:          // scale = 2 일 때
            _ret = MI;   // 음계 주파수 선택 : 미
            break;       // 해당 케이스 탈출
        
        case 3:          // scale = 3 일 때
            _ret = FA;   // 음계 주파수 선택 : 파
            break;       // 해당 케이스 탈출
        
        case 4:          // scale = 4 일 때
            _ret = SOL;  // 음계 주파수 선택 : 솔
            break;       // 해당 케이스 탈출
        
        case 5:          // scale = 5 일 때
            _ret = RA;   // 음계 주파수 선택 : 라
            break;       // 해당 케이스 탈출
        
        case 6:          // scale = 6 일 때
            _ret = SI;   // 음계 주파수 선택 : 시
            break;       // 해당 케이스 탈출
        
        case 7:          // scale = 7 일 때
            _ret = DO_H; // 음계 주파수 선택 : 높은 도
            break;       // 해당 케이스 탈출
    }

    return _ret; // 선택된 음계 주파수 값 반환
}

void Change_FREQ(unsigned int freq)  // 주파수를 변경하는 함수
{
    softToneWrite(BUZZER_PIN, freq); // Buzzer 핀에 주어진 주파수만큼 할당
}

void STOP_FREQ(void)              // 주파수 발생 정지 함수
{
    softToneWrite(BUZZER_PIN, 0); // Buzzer 핀에 주파수 0 Hz 할당
}

void BUZZER_Init(void)          // Buzzer 초기 설정
{
    softToneCreate(BUZZER_PIN); // Buzzer 핀에 주파수 신호 출력 설정
    STOP_FREQ();                // 주파수 발생 정지
}

int main(void) // 메인 함수 시작
{
    if(wiringPiSetupGpio() == -1) // Wiring Pi의 GPIO를 사용하기 위한 설정
    {
        return 1;                 // Wiring Pi의 GPIO가 설정되지 않았다면 종료
    }

    int i;             // 반복문 사용을 위한 정수형 변수 i 선언
    int nKeypadstate;  // KEYPAD 상태를 위한 정수형 변수 nKeypadstate 선언
    int check;         // 비밀번호 일치여부 확인을 위한 정수형 변수 check 선언
    int inkey;         // 입력키 저장을 위한 정수형 변수 inkey 선언
    int prekey = -1;   // 이전키 저장을 위한 정수형 변수 prekey, -1로 초기화
    int numcount = 0;  // 숫자 입력 개수 파악을 위한 정수형 변수 numcount, 0으로 초기화
    int misscount = 0; // 틀린 횟수 파악을 위한 정수형 변수 misscount, 0으로 초기화
    int keymode = 0;   // '#' 클릭 여부 확인을 위한 정수형 변수 keymode, 0으로 초기화

    char pw[4] = {};  // 비밀번호 저장을 위한 문자형 배열 pw, 크기는 4
    char num[5] = {}; // 비밀번호 입력을 위한 문자형 배열 num, 크기는 5

    softPwmCreate(SERVO, 0, 200); // DC Servo Motor 핀 PWM 설정
    BUZZER_Init();                // Buzzer 초기화

    for(i=0; i<MAX_KEY_BT_NUM; i++)     // KEYPAD 핀 입력 설정
    {
        pinMode(KeypadTable[i], INPUT); // KEYPAD 핀 테이블의 i번째 위치에 입력 설정 할당
    }

    while(1) // 반복문 while, 무한 루프
    {
        nKeypadstate = KeypadRead(); // KEYPAD로부터 버튼 입력을 읽어 상태를 변수에 저장
        inkey = -1;                  // 입력키에 -1 저장

        for(i=0; i<MAX_KEY_BT_NUM; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 KEYPAD 버튼 최대 개수인 12 미만까지 +1, 아래 코드 반복
        {
            if(nKeypadstate & (1<<i))   // 조건문 if, 현재 i의 값을 1 Left Shift하여 nKeypadstate 값과 AND연산했을때(버튼의 위치 탐색)
            {
                inkey = i;                  // 입력키에 i 저장
                Change_FREQ(SevenScale(0)); // 버튼 누를때마다 '낮은 도' 출력
                delay(10);                  // 10ms 동안 지속
                STOP_FREQ();                // 소리 출력 정지
            }
        }

        if((inkey != -1) && (inkey != prekey)) // 중복 입력 방지 처리 후 아래 코드 실행
        {
            if((inkey != 9) && (inkey != 11)) // '*'와 '#'은 배열에 저장하지 않음
            {
                num[numcount] = number[inkey];                  // 입력한 숫자를 비밀번호 입력 배열에 저장
                printf("Input Password : %c\n", num[numcount]); // 저장된 입력 숫자 메시지 출력
                numcount++;                                     // 숫자 입력 개수 1 증가
            }

            if((inkey == 11) && (keymode == -1)) // 마지막 입력이 '#'이면 아래 코드 실행(비밀번호 설정 시퀀스 진행)
            {
                for(int j=0; j<4; j++) // 반복문 for, 정수형 매개변수 j 선언, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                {
                    pw[j] = num[j];    // 입력 배열 내용을 비밀번호 저장 배열에 저장
                }

                for(int j=0; j<4; j++) // 반복문 for, 정수형 매개변수 j 선언, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                {
                    num[j] = -1;       // 입력 배열 초기화
                }

                keymode = 0;  // '#' 클릭 여부 확인 초기화
                numcount = 0; // 숫자 입력 개수 초기화
                printf("Password Setup Complete : %c %c %c %c\n", pw[0], pw[1], pw[2], pw[3]); // 비밀번호 설정 완료 및 확인 메시지 출력

                for(int j=0; j<=6;) // 반복문 for, 정수형 배개변수 j 선언, 0부터 시작하여 6 이하까지, 아래 코드 반복
                {
                    Change_FREQ(SevenScale(j)); // j에 해당하는 소리 출력
                    delay(300);                 // 300ms 동안 지속
                    STOP_FREQ();                // 소리 출력 정지
                    a += 3;                     // a에 3 증가 후 저장, 결과적인 음계 조합 '도-파-시'
                }
            }
            else if(inkey == 11) // 마지막 입력이 '#'이면 아래 코드 실행(비밀번호 설정 시퀀스 진입)
            {
                keymode = -1; // 비밀번호 설정 시퀀스 진행 상태로 설정
                numcount = 0; // 숫자 입력 개수 초기화
                printf("Password Setup\n"); // 비밀번호 설정 시퀀스 진행 메시지 출력
            }
            else if((inkey == 9) && (keymode == 0)) // 마지막 입력이 '*'이면 아래 코드 실행(비밀번호 입력 후 일치 판별 시퀀스)
            {
                for(int j=0; j<=4; j++) // 반복문 for, 정수형 매개변수 j 선언, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                {
                    if(num[j] != pw[j]) // 비밀번호가 틀렸을 시
                    {
                        check = -1; // 비밀번호 일치여부 -1 (불일치 판정)
                        break;      // 해당 반복문 탈출
                    }
                    else if(num[j] == pw[j]) // 비밀번호가 맞았을 시
                    {
                        check = 0; // 비밀번호 일치여부 0 (일치 판정)
                    }
                }

                if(check == 0) // 비밀번호 일치 판정일 시
                {
                    printf("Door is Open.\n"); // 문 열림 메시지 출력
                    misscount = 0;             // 틀린 횟수 0으로 초기화

                    Change_FREQ(SevenScale(3)); // '파' 출력
                    delay(250);                 // 250ms 동안 지속
                    STOP_FREQ();                // 소리 출력 정지
                    Change_FREQ(SevenScale(1)); // '레' 출력
                    delay(250);                 // 250ms 동안 지속
                    STOP_FREQ();                // 소리 출력 정지
                    Change_FREQ(SevenScale(5)); // '라' 출력
                    delay(250);                 // 250ms 동안 지속
                    STOP_FREQ();                // 소리 출력 정지
                    Change_FREQ(SevenScale(3)); // '파' 출력
                    delay(250);                 // 250ms 동안 지속
                    STOP_FREQ();                // 소리 출력 정지
                    Change_FREQ(SevenScale(7)); // '높은 도' 출력
                    delay(250);                 // 250ms 동안 지속
                    STOP_FREQ();                // 소리 출력 정지

                    softPwmWrite(SERVO, 25); // 90 degree
                    delay(2000);             // 2000ms 딜레이
                    softPwmWrite(SERVO, 15); // 0 degree
                }
                else if(check == -1) // 비밀번호 불일치 판정일 시
                {
                    printf("Wrong Password\n"); // 비밀번호 불일치 메시지 출력
                    misscount++;                // 틀린 횟수 1 증가
                    
                    if(misscount == 3) // 틀린 횟수가 3일 시
                    {
                        Change_FREQ(SevenScale(7)); // '높은 도' 출력
                        delay(300);                 // 300ms 동안 지속
                        STOP_FREQ();                // 소리 출력 정지
                        Change_FREQ(SevenScale(5)); // '라' 출력
                        delay(300);                 // 300ms 동안 지속
                        STOP_FREQ();                // 소리 출력 정지
                        Change_FREQ(SevenScale(3)); // '파' 출력
                        delay(300);                 // 300ms 동안 지속
                        STOP_FREQ();                // 소리 출력 정지

                        misscount = 0; // 틀린 횟수 0으로 초기화
                        printf("Please try again after 10 seconds."); // 10초 후 재입력 지시 메시지 출력
                        
                        for(int j=0; j<10; j++) // 반복문 for, 정수형 매개변수 j 선언, 0부터 시작하여 10 미만까지 +1, 아래 코드 반복
                        {
                            delay(1000);                  // 1000ms 딜레이
                            printf("%d seconds\n", 10-j); // 남은 시간 메시지 출력, 10초부터 1초씩 감소
                        }

                        printf("Try Again.\n"); // 재입력 지시 메시지 출력
                    }
                    else // 틀린 횟수가 1 or 2일 시
                    {
                        Change_FREQ(SevenScale(6)); // '시' 출력
                        delay(1000);                // 1000ms 동안 지속
                        STOP_FREQ();                // 소리 출력 정지
                    }
                }

                numcount = 0; // 숫자 입력 개수 초기화
            }

            prekey = inkey; // 이전키와 입력키 초기화
        }
    }
}
