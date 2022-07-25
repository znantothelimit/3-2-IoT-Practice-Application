/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 5 , Number 1 */

#include <wiringPi.h>

#define LED_RED_1 5
#define LED_RED_2 6

int main(void)
{
    if(wiringPiSetupGpio() == -1)
    {
        return 1;
    }

    pinMode(LED_RED_1, OUTPUT);   // LED1 PORT SET
    pinMode(LED_RED_2, OUTPUT);   // LED2 PORT SET
    digitalWrite(LED_RED_1, LOW); // LED1 OFF
    digitalWrite(LED_RED_2, LOW); // LED2 OFF

    while(1)
    {
        digitalWrite(LED_RED_1, HIGH); // LED1 ON
        digitalWrite(LED_RED_2, LOW);  // LED2 OFF
        delay(500);
        digitalWrite(LED_RED_1, LOW);  // LED1 OFF
        digitalWrite(LED_RED_2, HIGH); // LED2 ON
        delay(500);
    }

    return 0;
}
