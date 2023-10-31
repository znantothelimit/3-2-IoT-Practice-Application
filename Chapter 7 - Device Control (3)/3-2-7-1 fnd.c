/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 7 , Number 1 */

#include <wiringPi.h> // wiringPi 라이브러리를 포함

// FND를 선택하는 GPIO 핀의 정의
#define FND_SEL_S0 11
#define FND_SEL_S1 5
#define FND_SEL_S2 6
#define FND_SEL_S3 13
#define FND_SEL_S4 19
#define FND_SEL_S5 26

// FND 디스플레이의 세그먼트를 제어하는 GPIO 핀의 정의
#define FND_DB_A 2
#define FND_DB_B 3
#define FND_DB_C 4
#define FND_DB_D 17
#define FND_DB_E 27
#define FND_DB_F 22
#define FND_DB_G 10
#define FND_DB_DP 9

// 7세그먼트 LED의 각 비트를 나타내는 상수 정의
#define A_BIT 0x01
#define B_BIT 0x02
#define C_BIT 0x04
#define D_BIT 0x08
#define E_BIT 0x10
#define F_BIT 0x20
#define G_BIT 0x40
#define DP_BIT 0x80

// FND 디스플레이의 데이터 핀 개수
#define FND_DB_PIN_NUM 8

// FND 디스플레이의 최대 위치 (0부터 5까지, 6자리 디스플레이)
#define MAX_FND_POSITION 6

// FND 디스플레이의 최대 문자 수 (16진수로 0부터 F까지, 16개의 문자)
#define MAX_CHAR 16

// FND 디스플레이를 선택하는 핀들의 배열
const int FndSel[MAX_FND_POSITION] = {
    FND_SEL_S0,
    FND_SEL_S1,
    FND_SEL_S2,
    FND_SEL_S3,
    FND_SEL_S4,
    FND_SEL_S5,
};

// 각 숫자와 문자에 해당하는 7세그먼트 LED 패턴을 저장하는 배열
const int FndNumberTable[16] = {
    A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT,         // '0'
    B_BIT | C_BIT,                                         // '1'
    A_BIT | B_BIT | D_BIT | E_BIT | G_BIT,                 // '2'
    A_BIT | B_BIT | C_BIT | D_BIT | G_BIT,                 // '3'
    B_BIT | C_BIT | F_BIT | G_BIT,                         // '4'
    A_BIT | C_BIT | D_BIT | F_BIT | G_BIT,                 // '5'
    C_BIT | D_BIT | E_BIT | F_BIT | G_BIT,                 // '6'
    A_BIT | B_BIT | C_BIT,                                 // '7'
    A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT | G_BIT, // '8'
    A_BIT | B_BIT | C_BIT | F_BIT | G_BIT,                 // '9'
    A_BIT | B_BIT | C_BIT | E_BIT | F_BIT | G_BIT,         // 'A'
    C_BIT | D_BIT | E_BIT | F_BIT | G_BIT,                 // 'b'
    A_BIT | D_BIT | E_BIT | F_BIT,                         // 'C'
    B_BIT | C_BIT | D_BIT | E_BIT | G_BIT,                 // 'd'
    A_BIT | D_BIT | E_BIT | F_BIT | G_BIT,                 // 'E'
    A_BIT | E_BIT | F_BIT | G_BIT,
}; // 'F'

// FND 디스플레이 데이터 핀들을 저장하는 배열
const int FndPinTable[8] = {FND_DB_A, FND_DB_B, FND_DB_C, FND_DB_D,
                            FND_DB_E, FND_DB_F, FND_DB_G, FND_DB_DP};

int main(void)
{
    if (wiringPiSetupGpio() == -1)
    {
        return 1; // wiringPi 라이브러리 초기화 실패 시 프로그램 종료
    }

    int i, k;
    int m = 0;

    // FND 디스플레이 데이터 핀들을 출력 모드로 설정
    for (i = 0; i < FND_DB_PIN_NUM; i++)
    {
        pinMode(FndPinTable[i], OUTPUT);
    }

    while (1)
    {
        // FND 디스플레이 위치 초기화 (모두 끄기(비활성화)) -> FndSel에서는 LOW가 활성화 HIGH가 비활성화
        for (i = 0; i < MAX_FND_POSITION; i++)
        {
            digitalWrite(FndSel[i], HIGH);
        }

        // 하나의 FND 디스플레이 선택(활성화) (순환) -> FndSel에서는 LOW가 활성화 HIGH가 비활성화
        digitalWrite(FndSel[m % 6], LOW);
        m++;

        // 겉의 loop는 표현하려는 문자 개수, 속의 loop는 segment수
        // 숫자 및 문자 출력
        for (k = 0; k < MAX_CHAR; k++)
        {
            for (i = 0; i < FND_DB_PIN_NUM; i++)
            {
                // 해당 비트가 켜져 있는지 확인하고 해당 비트의 데이터 핀을 제어

                // FndNumberTable 배열에 저장된 세그먼트 패턴과 비트 연산을 사용하여
                // 현재 출력할 숫자 또는 문자에 대응하는 세그먼트 패턴을 가져옵니다.
                // 예를 들어, '0'에 대응하는 패턴은 A, B, C, D, E, F 세그먼트가 켜져야 하므로
                // A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT와 같습니다.
                // 그리고 '1'에 대응하는 패턴은 B, C 세그먼트가 켜져야 하므로 B_BIT | C_BIT와 같습니다.

                if (FndNumberTable[k] & (1 << i))
                {
                    // 만약 해당 비트가 켜져 있다면 데이터 핀을 HIGH로 설정하여 세그먼트를 켭니다.
                    // -> FndPinTable에서는 HIGH가 출력 LOW가 끔
                    digitalWrite(FndPinTable[i], HIGH);
                }
                else
                {
                    // 해당 비트가 꺼져 있다면 데이터 핀을 LOW로 설정하여 세그먼트를 끕니다.
                    // -> FndPinTable에서는 HIGH가 출력 LOW가 끔
                    digitalWrite(FndPinTable[i], LOW);
                }
            }

            // 현재 숫자 또는 문자 패턴을 FND 디스플레이에 출력했으므로,
            // 이 패턴을 0.5초 동안 유지하여 디스플레이가 해당 숫자 또는 문자를 표시합니다.
            delay(500); // 0.5초 동안 출력
        }
    }

    return 0;
}
