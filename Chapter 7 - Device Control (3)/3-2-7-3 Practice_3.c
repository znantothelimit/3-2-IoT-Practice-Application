/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 7 , Number 3 */

#include <wiringPi.h> // GPIO access library 헤더파일 선언
#include <softPwm.h>  // Software PWM library 헤더파일 선언
#include <softTone.h> // Tone wave signal 관련 헤더파일 선언
#include <lcd.h>      // Text LCD(Character LCD) 관련 헤더파일 선언

#define KEYPAD_PB1 2   // KEYPAD 포트 BT1 핀 정의 , 1
#define KEYPAD_PB2 3   // KEYPAD 포트 BT2 핀 정의 , 2
#define KEYPAD_PB3 4   // KEYPAD 포트 BT3 핀 정의 , 3
#define KEYPAD_PB5 17  // KEYPAD 포트 BT5 핀 정의 , 4
#define KEYPAD_PB6 27  // KEYPAD 포트 BT6 핀 정의 , 5
#define KEYPAD_PB7 22  // KEYPAD 포트 BT7 핀 정의 , 6
#define KEYPAD_PB9 10  // KEYPAD 포트 BT9 핀 정의 , 7
#define KEYPAD_PB10 9  // KEYPAD 포트 BT10 핀 정의 , 8
#define KEYPAD_PB11 11 // KEYPAD 포트 BT11 핀 정의 , 9
#define KEYPAD_PB13 5  // KEYPAD 포트 BT13 핀 정의 , ↓ (텍스트 하향 스크롤링 키)
#define KEYPAD_PB14 6  // KEYPAD 포트 BT14 핀 정의 , 0
#define KEYPAD_PB15 13 // KEYPAD 포트 BT15 핀 정의 , E (엔터키)

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

const int KeypadTable[12] = { KEYPAD_PB1, KEYPAD_PB2, KEYPAD_PB3,
                              KEYPAD_PB5, KEYPAD_PB6, KEYPAD_PB7,
                              KEYPAD_PB9, KEYPAD_PB10, KEYPAD_PB11,
                              KEYPAD_PB13, KEYPAD_PB14, KEYPAD_PB15 };                  // KEYPAD 핀 테이블 선언
const char number[12] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '>', '0', 'E' }; // Number 테이블 선언

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

void ServoControl(unsigned char str) // DC Servo Motor 동작 함수
{
    if(str == 'C')               // 중심 정렬
    {
        softPwmWrite(SERVO, 15); // 0 degree
    }
    else if(str == 'R')          // 우측 회전
    {
        softPwmWrite(SERVO, 25); // 90 degree
    }
    else if(str == 'L')          // 좌측 회전
    {
        softPwmWrite(SERVO, 5);  // -90 degree
    }
}

int main(void) // 메인 함수 시작
{
    if(wiringPiSetupGpio() == -1) // Wiring Pi의 GPIO를 사용하기 위한 설정
    {
        return 1;                 // Wiring Pi의 GPIO가 설정되지 않았다면 종료
    }

    int i;               // 반복문 사용을 위한 정수형 변수 i 선언
    int j = 0;           // 비밀번호 동작을 위한 정수형 변수 j, 0으로 초기화
    int k = 0;           // 비밀번호 입력을 위한 정수형 변수 k, 0으로 초기화
    int c = 0;           // 비밀번호 변경을 위한 정수형 변수 c, 0으로 초기화
    int s = 1;           // 소리 출력을 위한 정수형 변수 s, 1로 초기화
    int nKeypadstate;    // KEYPAD 상태를 위한 정수형 변수 nKeypadstate 선언
    int inkey;           // 입력키 저장을 위한 정수형 변수 inkey 선언
    int prekey = -1;     // 이전키 저장을 위한 정수형 변수 prekey, -1로 초기화
    int numcount = 0;    // 숫자 입력 개수 파악을 위한 정수형 변수 numcount, 0으로 초기화
    int misscount = 0;   // 틀린 횟수 파악을 위한 정수형 변수 misscount, 0으로 초기화
    int input_PW = 0;    // Input PW Mode 진입을 위한 정수형 변수 input_PW, 0으로 초기화
    int set_PW = 0;      // Set PW Mode 진입을 위한 정수형 변수 set_PW, 0으로 초기화
    int change_PW = 0;   // Change PW Mode 진입을 위한 정수형 변수 change_PW, 0으로 초기화
    int sound_ONOFF = 0; // Sound ON/OFF mode 진입을 위한 정수형 변수 sound_ONOFF, 0으로 초기화
    int page = 0;        // 페이지 동작을 위한 정수형 변수 page, 0으로 초기화
    int disp;            // LCD 사용을 위한 정수형 변수 disp 선언

    char pw[1][5] = { '0', '0', '0', '0', }; // 비밀번호 저장을 위한 문자형 2차원 배열 pw, 열1 + 행5, '0'으로 초기화
    char num[6] = { '*', '*', '*', '*', };   // 비밀번호 입력을 위한 문자형 배열 num, 크기는 6, '*'으로 초기화

    char as_is_pw[1][5] = { '*', '*', '*', '*', }; // 현재 비밀번호 입력을 위한 문자형 2차원 배열 as_is_pw, 열1 + 행5, '*'으로 초기화
    char to_be_pw[1][5] = { '*', '*', '*', '*', }; // 변경 패스워드 입력을 위한 문자형 2차원 배열 to_be_pw, 열1 + 행5, '*'으로 초기화

    softPwmCreate(SERVO, 0, 200);                                 // DC Servo Motor 핀 PWM 설정
    BUZZER_Init();                                                // Buzzer 초기화
    disp = lcdInit(2, 16, 4, 18, 23, 12, 16, 20, 21, 0, 0, 0, 0); // LCD 초기화
    lcdClear(disp);                                               // LCD 클리어

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
                inkey = i;              // 입력키에 i 저장
            }
        }

        if((prekey != inkey) && (inkey != -1)) // 버튼 입력 동작과 중복 입력 처리
        {
            if((page == 0) && (inkey == 9)) // 현재 페이지가 0번째이고 ↓ 버튼을 눌렀으면,
            {
                page = 1;                   // 현재 페이지를 1번째 페이지로 변경
            }
            else if((page == 1) && (inkey == 9)) // 현재 페이지가 1번째이고 ↓ 버튼을 눌렀으면,
            {
                page = 0;                        // 현재 페이지를 0번째 페이지로 변경
            }

            if((page == 0) && (inkey == 0)) // 현재 페이지가 0번째이고 1번 버튼(배열 0번째)을 눌렀으면,
            {
                input_PW = 1; // Input PW Mode 진입
                page = -1;    // 페이지 초기화
            }
            else if((page == 0) && (inkey == 1)) // 현재 페이지가 0번째이고 2번 버튼(배열 1번째)을 눌렀으면,
            {
                set_PW = 1; // Set PW Mode 진입
                page = -1;  // 페이지 초기화
            }
            else if((page == 1) && (inkey == 2)) // 현재 페이지가 1번째이고 3번 버튼(배열 2번째)을 눌렀으면,
            {
                change_PW = 1; // Change PW Mode 진입
                page = -1;     // 페이지 초기화
            }
            else if((page == 1) && (inkey == 3)) // 현재 페이지가 1번째이고 4번 버튼(배열 3번째)을 눌렀으면,
            {
                sound_ONOFF = 1; // Sound on/off Mode 진입
                page = -1;       // 페이지 초기화
            }

            delay(50); // 50ms 딜레이
        }
        else if((prekey != inkey) && (inkey == -1)) // 중복 입력 처리와 버튼 입력 동작 이상 판단
        {
            delay(50); // 50ms 딜레이
        }

        if((prekey != inkey) && (inkey != -1)) // 버튼 입력 동작과 중복 입력 처리
        {
            SET: // 포인트 지점 설정
                if(set_PW == 1) // Set PW Mode
                {
                    lcdClear(disp);             // LCD 클리어
                    lcdPosition(disp, 0, 0);    // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                    lcdPuts(disp, "2. Set PW"); // LCD에 문자열 표시
                    delay(50);                  // 50ms 딜레이

                    num[j] = number[inkey]; // 입력 버튼의 값을 비밀번호 입력 배열에 저장
                    j++;                    // 비밀번호 입력 배열 인덱스 이동

                    if(j == 6) // 비밀번호 입력이 끝났을 시,
                    {
                        if(inkey == 11) // 마지막에 E 버튼이 눌렸을 시,
                        {
                            set_PW = 0; // Set PW Mode 탈출
                            page = 0;   // 0번째 페이지로 이동
                            j = 0;      // 동작 변수 초기화

                            Change_FREQ(SI * s); // '시' s번 출력
                            delay(100);          // 100ms 동안 지속
                            STOP_FREQ();         // 소리 출력 정지

                            lcdClear(disp);          // LCD 클리어
                            lcdPosition(disp, 0, 0); // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                            lcdPuts(disp, "Done");   // LCD에 문자열 표시

                            for(i=0; i<4; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                            {
                                pw[0][i] = num[i+1]; // 입력 배열 내용을 비밀번호 저장 배열에 저장
                            }

                            for(i=0; i<4; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                            {
                                lcdPosition(disp, i, 1);  // LCD 위치 하단라인(1번째 줄) i번째 셀로 조정
                                lcdPuts(disp, &pw[0][i]); // LCD에 문자 표시
                                delay(50);                // 50ms 딜레이
                            }

                            delay(2000); // 2000ms 딜레이
                        }
                        else // 마지막에 E 버튼이 눌리지 않았을 시,
                        {
                            Change_FREQ(FA * s); // '파' s번 출력
                            delay(100);          // 100ms 동안 지속
                            STOP_FREQ();         // 소리 출력 정지

                            lcdClear(disp);                    // LCD 클리어
                            lcdPosition(disp, 0, 0);           // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                            lcdPuts(disp, "Invalid password"); // LCD에 문자열 표시
                            delay(2000);                       // 2000ms 딜레이

                            j = 0; // 동작 변수 초기화

                            goto SET; // 포인트 지점으로 이동
                        }
                    }
                    else if(inkey == 9) // ↓ 버튼이 눌렸을 시,
                    {
                        j = 1; // 동작 변수를 재입력으로 설정
                    }
                    else if(inkey == 11) // E 버튼이 눌렸을 시,
                    {
                        j = 1; // 동작 변수를 재입력으로 설정
                    }
                }
                else if(change_PW == 1) // Change PW Mode
                {
                    lcdClear(disp);                 // LCD 클리어
                    lcdPosition(disp, 0, 0);        // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                    lcdPuts(disp, "3. Change PW");  // LCD에 문자열 표시
                    lcdPosition(disp, 0, 1);        // LCD 위치 하단라인(1번째 줄) 가장 좌측(0번째 셀)으로 조정
                    lcdPuts(disp, "Input past PW"); // LCD에 문자열 표시
                    delay(50);                      // 50ms 딜레이

                    num[c] = number[inkey]; // 입력 버튼의 값을 비밀번호 입력 배열에 저장
                    c++;                    // 비밀번호 입력 배열 인덱스 이동

                    if(numcount == 1) // 숫자 입력 개수 파악이 정상적일 시,
                    {
                        if(c == 6) // 비밀번호 입력이 끝났을 시,
                        {
                            if(inkey == 11) // 마지막에 E 버튼이 눌렸을 시,
                            {
                                lcdClear(disp);           // LCD 클리어
                                lcdPosition(disp, 0, 0);  // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                                lcdPuts(disp, "As is: "); // LCD에 문자열 표시
                                lcdPosition(disp, 0, 1);  // LCD 위치 하단라인(1번째 줄) 가장 좌측(0번째 셀)으로 조정
                                lcdPuts(disp, "To be: "); // LCD에 문자열 표시

                                for(i=0; i<4; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                                {
                                    lcdPosition(disp, i+7, 0);      // LCD 위치 상단라인(0번째 줄) i+7번째 셀로 조정
                                    lcdPuts(disp, &as_is_pw[0][i]); // LCD에 문자 표시
                                    delay(50);                      // 50ms 딜레이
                                }

                                for(i=0; i<4; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                                {
                                    to_be_pw[0][i] = num[i+1]; // 입력 배열 내용을 변경 비밀번호 배열에 저장
                                }

                                for(i=0; i<4; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                                {
                                    lcdPosition(disp, i+7, 0);      // LCD 위치 상단라인(0번째 줄) i+7번째 셀로 조정
                                    lcdPuts(disp, &to_be_pw[0][i]); // LCD에 문자 표시
                                    delay(50);                      // 50ms 딜레이
                                }

                                delay(2000); // 2000ms 딜레이

                                for(i=0; i<4; i++)
                                {
                                    pw[0][i] = to_be_pw[0][i]; // 변경 비밀번호 배열 내용을 비밀번호 배열에 저장
                                }

                                Change_FREQ(RA * s); // '라' s번 출력
                                delay(100);          // 100ms 동안 지속
                                STOP_FREQ();         // 소리 출력 정지

                                lcdClear(disp);          // LCD 클리어
                                lcdPosition(disp, 0, 0); // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                                lcdPuts(disp, "Done");   // LCD에 문자열 표시

                                delay(2000); // 2000ms 딜레이

                                change_PW = 0; // Change PW Mode 탈출
                                page = 0;      // 0번째 페이지로 이동
                                numcount = 0;  // 숫자 입력 횟수 초기화
                                c = 0;         // 변경 변수 초기화
                            }
                            else // 마지막에 E 버튼이 눌리지 않았을 시,
                            {
                                Change_FREQ(FA * s); // '파' s번 출력
                                delay(100);          // 100ms 동안 지속
                                STOP_FREQ();         // 소리 출력 정지

                                lcdClear(disp);                    // LCD 클리어
                                lcdPosition(disp, 0, 0);           // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                                lcdPuts(disp, "Invalid password"); // LCD에 문자열 표시

                                delay(2000); // 2000ms 딜레이
                            }

                            c = 1; // 변경 변수 재설정
                        }
                        else if(inkey == 9) // ↓ 버튼이 눌렸을 시,
                        {
                            c = 1; // 변경 변수를 재입력으로 설정
                        }
                        else if(inkey == 11) // E 버튼이 눌렸을 시,
                        {
                            c = 1; // 변경 변수를 재입력으로 설정
                        }
                    }
                    else if(c == 6) // 비밀번호 입력이 끝났을 시,
                    {
                        if(inkey == 11) // 마지막에 E 버튼이 눌렸을 시,
                        {
                            for(i=0; i<4; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                            {
                                as_is_pw[0][i] = num[i+1]; // 입력 배열 내용을 현재 비밀번호 배열에 저장
                            }

                            for(i=0; i<4; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                            {
                                if(pw[0][i] != as_is_pw[0][i]) // 현재 비밀번호 배열 내용이 입력 비밀번호 배열 내용과 일치하지 않으면, 
                                {
                                    i = -1; // 반복 변수 설정
                                    c = 0;  // 변경 변수 초기화

                                    Change_FREQ(FA * s); // '파' s번 출력
                                    delay(100);          // 100ms 동안 지속
                                    STOP_FREQ();         // 소리 출력 정지

                                    lcdClear(disp);                    // LCD 클리어
                                    lcdPosition(disp, 0, 0);           // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                                    lcdPuts(disp, "Invalid password"); // LCD에 문자열 표시

                                    delay(2000); // 2000ms 딜레이

                                    lcdPosition(disp, 0, 1);         // LCD 위치 하단라인(1번째 줄) 가장 좌측(0번째 셀)으로 조정
                                    lcdPuts(disp, "Retry Press E");  // LCD에 문자열 표시
                                    delay(50);                       // 50ms 딜레이

                                    break; // 반복문 for 탈출
                                }
                            }

                            if(i == 4) // 현재 비밀번호 배열 내용과 입력 비밀번호 배열 내용이 일치하면,
                            {
                                Change_FREQ(SI * s); // '시' s번 출력
                                delay(100);          // 100ms 동안 지속
                                STOP_FREQ();         // 소리 출력 정지

                                lcdClear(disp);                // LCD 클리어
                                lcdPosition(disp, 0, 0);       // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                                lcdPuts(disp, "Correct!");     // LCD에 문자열 표시
                                lcdPosition(disp, 0, 1);       // LCD 위치 하단라인(1번째 줄) 가장 좌측(0번째 셀)으로 조정
                                lcdPuts(disp, "Input New PW"); // LCD에 문자열 표시

                                c = 0;        // 변경 변수 초기화
                                numcount = 1; // 숫자 입력 횟수 재설정
                            }
                        }
                    }

                    delay(50); // 50ms 딜레이
                }
                else if(sound_ONOFF == 1) // Sound on/off Mode
                {
                    if(s == 1) // 소리 출력 변수가 1이면,(ON 상태)
                    {
                        s = 0;           // 0으로 변경(OFF 상태)
                        sound_ONOFF = 0; // Sound on/off Mode 탈출
                        page = 0;        // 0번째 페이지로 이동

                        lcdClear(disp);             // LCD 클리어
                        lcdPosition(disp, 0, 0);    // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                        lcdPuts(disp, "Sound off"); // LCD에 문자열 표시

                        delay(2000); // 2000ms 딜레이
                    }
                    else if(s == 0) // 소리 출력 변수가 0이면,(OFF 상태)
                    {
                        s = 1;           // 0으로 변경(OFF 상태)
                        sound_ONOFF = 0; // Sound on/off Mode 탈출
                        page = 0;        // 0번째 페이지로 이동

                        lcdClear(disp);            // LCD 클리어
                        lcdPosition(disp, 0, 0);   // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                        lcdPuts(disp, "Sound on"); // LCD에 문자열 표시

                        delay(2000); // 2000ms 딜레이
                    }
                }
                else if(input_PW == 1) // Input PW Mode
                {
                    lcdClear(disp);               // LCD 클리어
                    lcdPosition(disp, 0, 0);      // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                    lcdPuts(disp, "1. Input PW"); // LCD에 문자열 표시
                    delay(50);                    // 50ms 딜레이

                    num[k] = number[inkey]; // 입력 버튼의 값을 비밀번호 입력 배열에 저장
                    k++;                    // 비밀번호 입력 배열 인덱스 이동

                    if(k == 6) // 비밀번호 입력이 끝났을 시,
                    {
                        if(inkey == 11) // 마지막에 E 버튼이 눌렸을 시,
                        {
                            for(i=0; i<4; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 4 미만까지 +1, 아래 코드 반복
                            {
                                if(pw[0][i] != num[i+1]) // 입력 비밀번호 배열 내용이 저장 비밀번호 배열 내용과 일치하지 않으면, 
                                {
                                    i = -1;      // 반복 변수 설정
                                    misscount++; // 틀린 횟수 1 증가

                                    Change_FREQ(FA * s); // '파' s번 출력
                                    delay(100);          // 100ms 동안 지속
                                    STOP_FREQ();         // 소리 출력 정지

                                    lcdClear(disp);                    // LCD 클리어
                                    lcdPosition(disp, 0, 0);           // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                                    lcdPuts(disp, "Invalid password"); // LCD에 문자열 표시

                                    if(misscount == 3) // 틀린 횟수가 3번이 되면, 
                                    {
                                        lcdClear(disp);            // LCD 클리어
                                        lcdPosition(disp, 0, 0);   // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                                        lcdPuts(disp, "No more!"); // LCD에 문자열 표시

                                        Change_FREQ(SOL * s); // '솔' s번 출력
                                        delay(400);           // 400ms 동안 지속
                                        STOP_FREQ();          // 소리 출력 정지
                                        Change_FREQ(RA * s);  // '라' s번 출력
                                        delay(400);           // 400ms 동안 지속
                                        STOP_FREQ();          // 소리 출력 정지
                                        Change_FREQ(SI * s);  // '시' s번 출력
                                        delay(400);           // 400ms 동안 지속
                                        STOP_FREQ();          // 소리 출력 정지

                                        lcdPosition(disp, 0, 1);          // LCD 위치 하단라인(1번째 줄) 가장 좌측(0번째 셀)으로 조정
                                        lcdPuts(disp, "Retry after 10s"); // LCD에 문자열 표시
                                        delay(100);                       // 100ms 딜레이

                                        for(i=0; i<10; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 10 미만까지 +1, 아래 코드 반복
                                        {
                                            delay(1000);         // 1000ms 동안 지속 (프로그램 흐름)
                                            Change_FREQ(RA * s); // '라' s번 출력
                                            delay(100);          // 100ms 동안 지속 (버저 소리)
                                            STOP_FREQ();         // 소리 출력 정지
                                        }

                                        delay(900);          // 900ms 동안 지속 (프로그램 흐름)
                                        Change_FREQ(RE * s); // '레' s번 출력
                                        delay(100);          // 100ms 동안 지속 (버저 소리)
                                        STOP_FREQ();         // 소리 출력 정지

                                        misscount = 0; // 틀린 횟수 초기화
                                        input_PW = 0;  // Input PW Mode 탈출
                                        page = 0;      // 0번째 페이지로 이동
                                        k = 0;         // 입력 변수 초기화
                                    }

                                    delay(2000); // 2000ms 딜레이

                                    lcdPosition(disp, 0, 1);        // LCD 위치 하단라인(1번째 줄) 가장 좌측(0번째 셀)으로 조정
                                    lcdPuts(disp, "Retry press E"); // LCD에 문자열 표시
                                    delay(50);                      // 50ms 딜레이

                                    k = 1; // 입력 변수 재설정

                                    break; // 반복문 for 탈출
                                }

                                if(i == 4) // 현재 비밀번호 배열 내용과 입력 비밀번호 배열 내용이 일치하면,
                                {
                                    Change_FREQ(DO_H * s); // '높은 도' s번 출력
                                    delay(100);            // 100ms 동안 지속
                                    STOP_FREQ();           // 소리 출력 정지

                                    ServoControl('R'); // DC Servo Motor 우측 회전

                                    lcdClear(disp);               // LCD 클리어
                                    lcdPosition(disp, 0, 0);      // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                                    lcdPuts(disp, "Door opened"); // LCD에 문자열 표시

                                    delay(2000); // 2000ms 딜레이

                                    Change_FREQ(DO_H * s); // '높은 도' s번 출력
                                    delay(100);            // 100ms 동안 지속
                                    STOP_FREQ();           // 소리 출력 정지

                                    ServoControl('C'); // DC Servo Motor 중심 정렬

                                    misscount = 0; // 틀린 횟수 초기화
                                    input_PW = 0;  // Input PW Mode 탈출
                                    page = 0;      // 0번째 페이지로 이동
                                    k = 0;         // 입력 변수 초기화
                                }
                                else // 이외의 상황에는, 
                                {
                                    k = 1; // 입력 변수 재설정
                                }
                            }
                        }
                        else if(inkey != 11) // 마지막에 E 버튼이 눌리지 않았을 시, 
                        {
                            k = 1; // 입력 변수 재설정

                            lcdClear(disp);                    // LCD 클리어
                            lcdPosition(disp, 0, 0);           // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
                            lcdPuts(disp, "Invalid password"); // LCD에 문자열 표시

                            delay(50); // 50ms 딜레이

                            Change_FREQ(FA * s); // '파' s번 출력
                            delay(100);          // 100ms 동안 지속
                            STOP_FREQ();         // 소리 출력 정지

                            delay(2000); // 2000ms 딜레이

                            lcdPosition(disp, 0, 1);        // LCD 위치 하단라인(1번째 줄) 가장 좌측(0번째 셀)으로 조정
                            lcdPuts(disp, "Retry press E"); // LCD에 문자열 표시
                            delay(50);                      // 50ms 딜레이
                        }
                    }
                    else if(inkey == 9) // ↓ 버튼이 눌렸을 시,
                    {
                        k = 1; // 입력 변수를 재입력으로 설정
                    }
                    else if(inkey == 11) // E 버튼이 눌렸을 시,
                    {
                        k = 1; // 입력 변수를 재입력으로 설정
                    }

                    delay(50); // 50ms 딜레이
                }
        }
        else if((prekey != inkey) && (inkey == -1)) // 버튼 입력 장애와 중복 입력 처리
        {
            delay(50); // 50ms 딜레이
        }

        if(page == 0) // 0번째 페이지일 시,
        {
            lcdClear(disp);                    // LCD 클리어
            lcdPosition(disp, 0, 0);           // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
            lcdPuts(disp, "1. Input PW");      // LCD에 문자열 표시
            lcdPosition(disp, 0, 1);           // LCD 위치 하단라인(1번째 줄) 가장 좌측(0번째 셀)으로 조정
            lcdPuts(disp, "2. Set PW     ↓ "); // LCD에 문자열 표시
            delay(100);                        // 100ms 딜레이
        }
        else if(page == 1) // 1번째 페이지일 시,
        {
            lcdClear(disp);                     // LCD 클리어
            lcdPosition(disp, 0, 0);            // LCD 위치 상단라인(0번째 줄) 가장 좌측(0번째 셀)으로 조정
            lcdPuts(disp, "3. Change PW");      // LCD에 문자열 표시
            lcdPosition(disp, 0, 1);            // LCD 위치 하단라인(1번째 줄) 가장 좌측(0번째 셀)으로 조정
            lcdPuts(disp, "4. Sound on/off ↓"); // LCD에 문자열 표시
            delay(100);                         // 100ms 딜레이
        }

        prekey = inkey; // 이전키와 입력키 초기화

        STOP_FREQ(); // 소리 출력 정지
    }
}
