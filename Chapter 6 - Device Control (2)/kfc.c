#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>

#define SERVO 18

#define KEYPAD_SIZE 12
#define KEYPAD_PB1 5   // 1   5
#define KEYPAD_PB2 6   // 2   6
#define KEYPAD_PB3 7   // 3   7   
#define KEYPAD_PB5 9   // 4   9
#define KEYPAD_PB6 10   // 5   10
#define KEYPAD_PB7 11  // 6   11
#define KEYPAD_PB9 12   // 7  12
#define KEYPAD_PB10 13  // 8  13
#define KEYPAD_PB11 19 // 9   19
#define KEYPAD_PB13 23 // *   23
#define KEYPAD_PB14 20  // 0   
#define KEYPAD_PB15 21 // #


#define BUZZER_PIN 17 // Buzzer Pin 정의
#define BUZ 784       // 버저 G 주파수

#define DO_L 523
#define RE 587
#define MI 659
#define FA 698
#define SOL 784
#define RA 880
#define SI 987
#define DO_H 1046

// gpio 번호 할당
int keypadPins[KEYPAD_SIZE] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
char keypadChars[KEYPAD_SIZE] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};
int init_pwd[4] = {0, 0, 0, 0};

void STOP_FREQ(void) // 주파수 발생 정지 함수
{
    softToneWrite(BUZZER_PIN, 0);
}

void Buzzer_Init(void) // Buzzer 초기 설정
{
    softToneCreate(BUZZER_PIN);
    STOP_FREQ();
}

int main()
{
    bool passwordMatch;
    bool edit_pwd = false;
    int err_count = 0;

    if (wiringPiSetupGpio() == -1)
    {
        fprintf(stderr, "Unable to setup WiringPi\n");
        return 1;
    }

    softPwmCreate(SERVO, 0, 200);
    Buzzer_Init(); // Buzzer 초기화

    printf("DoorLock\n");

    for (int i = 0; i < KEYPAD_SIZE; i++)
    {
        pinMode(keypadPins[i], INPUT);
        pullUpDnControl(keypadPins[i], PUD_UP);
    }

    char pwd[4] = {'0', '0', '0', '0'};
    char code[4] = {'0', '0', '0', '0'};
    char tmp;

    while (1)
    {
        if (err_count >= 3)
        {
            printf("10초 후 부터 입력 가능");
            delay(5000);
            err_count = 0;
        }

        pwd[4] = '\0'; // Null-terminate the string

        for (int i = 0; i < 4; i++)
        {
            int key = -1;
            if (edit_pwd == false)
            {
                // 문자를 입력받기 위해 대기하는 while문
                while (key == -1)
                {
                    for (int j = 0; j < KEYPAD_SIZE; j++)
                    {
                        if (digitalRead(keypadPins[j]) == LOW)
                        {
                            key = j;
                            delay(200); // Debounce
                            break;
                        }
                    }
                }
                // 해당 인덱스에 해당하는 키값(1 = 1)을 code에 저장
                tmp = keypadChars[key];
                //printf("%c", tmp);
            }

            if (edit_pwd == false)
            {
                switch (tmp)
                {
                case '*': // 비밀번호 확인
                    passwordMatch = true;
                    for (int l = 0; l < 4; l++)
                    {
                        if (pwd[l] != code[l])
                        {
                            printf("비밀번호가 아닙니다.\n");
                            passwordMatch = false;
                            err_count++;
                            printf("현재 비밀번호 : %s\n", pwd);
                            i = -1;
                            // for (int k = 0; k < 4; k++)
                            // {
                            //     code[k] = '0';
                            // }
                            break;
                        }
                    }
                    if (passwordMatch)
                    {
                        printf("문이 열립니다.\n");
                        softToneWrite(BUZZER_PIN, BUZ);
                        delay(500);
                        STOP_FREQ();
                        softPwmWrite(SERVO, 25);
                        delay(1000);
                        softPwmWrite(SERVO, 15);
                        i = -1;
                        for (int k = 0; k < 4; k++)
                        {
                            code[k] = '0';
                        }
                        err_count = 0;
                    }
                    break;
                case '#':
                    edit_pwd = true;
                    i = -1;
                    break; // 비밀번호 설정 / 설정완료
                default:

                    if (i > 3)
                    {
                        printf("비밀번호를 처음부터 다시 입력해주세요.");
                        i = -1;
                    }
                    else{code[i] = tmp;}
                    printf("%c", tmp);
                    // else
                    // {
                    //     i++;
                    // }
                    break;
                }
            }
            else if (edit_pwd)
            {
                if (tmp == '#')
                {
                    bool validCode = true;
                    for (int l = 0; l < 4; l++)
                    {
                        if (code[l] < '0' || code[l] > '9')
                        {
                            printf("비밀번호가 잘못되었습니다. 숫자를 입력해주세요.\n");
                            validCode = false;
                            i = -1;
                            break;
                        }
                    }
                    if (validCode)
                    {
                        printf("새로운 비밀번호를 설정합니다.\n");
                        //printf("입력된 비밀번호 : %s\n", code);
                        for (int l = 0; l < 4; l++)
                        {
                            pwd[l] = code[l];
                        }
                        //printf("새로운 비밀번호 : %s\n ", pwd);
                        i = -1;
                        edit_pwd = false;
                    }
                }
                else
                {
                    if (i < 4)
                    {
                        code[i] = tmp;
                        //printf("%c\n", tmp);
                    }
                    else
                    {
                        printf("4자리를 초과했습니다. 다시 설정하려면 #을 눌러주세요\n");
                        i = -1;
                    }
                }
            }
        }
    }

    return 0;
}
