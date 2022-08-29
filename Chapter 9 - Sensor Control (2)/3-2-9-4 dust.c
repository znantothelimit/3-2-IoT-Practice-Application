/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 9 , Number 4 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define CS_MCP3208 8      // GPIO 8
#define SPI_CHANNEL 0     // SPI Channel
#define SPI_SPEED 1000000 // SPI Speed
#define LED_PIN 17        // LED Pin

int ReadMcp3208ADC(unsigned char adcChannel)
{
    unsigned char buff[3];
    int nAdcValue = 0;

    buff[0] = 0x06 | ((adcChannel & 0x07) >> 2);
    buff[1] = ((adcChannel & 0x07) << 6);
    buff[2] = 0x00;

    digitalWrite(CS_MCP3208, 0); // Low CS Activate
    wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

    buff[1] = 0x0F & buff[1];
    nAdcValue = (buff[1] << 8) | buff[2];

    digitalWrite(CS_MCP3208, 1);

    return nAdcValue;
}

int main(void)
{
    int dustChannel = 3;
    float Vo_Val = 0;
    float Voltage = 0;
    float dustDensity = 0;

    int samplingTime = 280; // 0.28ms
    int delayTime = 40;     // 0.32 - 0.28 = 0.04ms
    int offTime = 9680;     // 10 - 0.32 = 9.68ms

    if(wiringPiSetupGpio() == -1)
    {
        fprintf(stdout, "Not start wiringPi : %s\n", strerror(errno));

        return 1;
    }

    // wiringPi Init
    if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
    {
        fprintf(stdout, "wiringPiSPISetup Failed : %s\n", strerror(errno));

        return 1;
    }

    pinMode(CS_MCP3208, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    while(1)
    {
        digitalWrite(LED_PIN, LOW); // IRED On
        delayMicroseconds(samplingTime);

        Vo_Val = ReadMcp3208ADC(dustChannel); // Sensor Read
        
        delayMicroseconds(delayTime);
        digitalWrite(LED_PIN, HIGH); // IRED off
        delayMicroseconds(offTime);

        Voltage = (Vo_Val * 3.3 / 1024.0) / 3.0;
        dustDensity = (0.172 *  (Voltage) - 0.0999) * 1000;

        printf("Voltage : %f , ", Voltage);
        printf("Value : %f (ug/m3)\n", dustDensity);

        delay(1000);
    }
}
