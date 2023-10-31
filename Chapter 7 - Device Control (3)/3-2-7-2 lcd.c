/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 7 , Number 2 */

#include <stdio.h>
#include <wiringPi.h>
#include <lcd.h>

int main(void)
{
    int disp1;

    if(wiringPiSetupGpio() == -1)
    {
        return 1;
    }

    /* <lcd.h>내 라이브러리 함수
       int lcdInit (int rows, int cols, int bits, int rs, int strb, int d4, int d5, int d6, int d7, int d0, int d1, int d2, int d3);
       rows = 2, cols = 16, rs = rs핀번호, strb = E(Strobe)핀번호
       4bit 모드를 사용하므로 bits = 4, d4~d7의 데이터 핀만 인수로 넣어주고 d0~d3까지는 0으로 넣어줌
       disp1 = lcdInit(2, 16, 4, 2, 4, 20, 21, 12, 16, 0, 0, 0, 0); // lcdInit()이후 disp1은 핸들이 됨 */
    lcdClear(disp1); // lcd 화면 clear

    lcdPosition(disp1, 0, 0); // lcd 위치 조정 
    lcdPuts(disp1, "Hello World"); // lcd 출력

    lcdPosition(disp1, 0, 1);
    lcdPuts(disp1, "Have nice day!");

    delay(1000);

    return 0;
}
