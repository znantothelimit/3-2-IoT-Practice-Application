/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 6 , Number 4 */

#include <wiringPi.h> // GPIO access library 헤더파일 선언
#include <softTone.h> // Tone wave signal 관련 헤더파일 선언

#define BUZZER_PIN 17 // Buzzer Pin 정의
#define DO_L 523      // 7음계에서 각 음에 해당하는 주파수 값 정의
#define RE 587
#define MI 659
#define FA 698
#define SOL 784
#define RA 880
#define SI 987
#define DO_H 1046

// scale로 들어온 숫자에 따라 C Major Scale 순서대로 해당하는 주파수 값 반환
unsigned int SevenScale(unsigned char scale) 
{
    unsigned int _ret = 0;

    switch(scale)
    {
        case 0:
            _ret = DO_L;
            break;
        case 1:
            _ret = RE;
            break;
        case 2:
            _ret = MI;
            break;
        case 3:
            _ret = FA;
            break;
        case 4:
            _ret = SOL;
            break;
        case 5:
            _ret = RA;
            break;
        case 6:
            _ret = SI;
            break;
        case 7:
            _ret = DO_H;
            break;
    }

    return _ret;
}

void Change_FREQ(unsigned int freq) // 주파수를 변경하는 함수
{
    softToneWrite(BUZZER_PIN, freq);
}

void STOP_FREQ(void) // 주파수 발생 정지 함수
{
    softToneWrite(BUZZER_PIN, 0);
}

void BUZZER_Init(void) // Buzzer 초기 설정
{
    softToneCreate(BUZZER_PIN);
    STOP_FREQ();
}

int main(void)
{
    if(wiringPiSetupGpio() == -1)
    {
        return 1;
    }

    BUZZER_Init(); // Buzzer 초기화

    int i;

    for(i=0; i<8; i++) // C Major Scale 출력 위해 반복 
    {
        Change_FREQ(SevenScale(i)); // 각 음계에 해당하는 음 발생
        delay(500);                 // 500ms 지연
        STOP_FREQ();                // 음 발생 정지
    }

    while(1)
    {

    }

    return 0;
}
