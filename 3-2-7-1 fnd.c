/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Chapter 7 , Number 1 */

#include <wiringPi.h>

#define FND_SEL_S0 11
#define FND_SEL_S1 5
#define FND_SEL_S2 6
#define FND_SEL_S3 13
#define FND_SEL_S4 19
#define FND_SEL_S5 26

#define FND_DB_A 2
#define FND_DB_B 3
#define FND_DB_C 4
#define FND_DB_D 17
#define FND_DB_E 27
#define FND_DB_F 22
#define FND_DB_G 10
#define FND_DB_DP 9

#define A_BIT 0x01
#define B_BIT 0x02
#define C_BIT 0x04
#define D_BIT 0x08
#define E_BIT 0x10
#define F_BIT 0x20
#define G_BIT 0x40
#define DP_BIT 0x80

#define FND_DB_PIN_NUM 8
#define MAX_FND_POSITION 6
#define MAX_CHAR 16

const int FndSel[MAX_FND_POSITION] = { FND_SEL_S0, FND_SEL_S1, FND_SEL_S2, 
                                       FND_SEL_S3, FND_SEL_S4, FND_SEL_S5, };

const int FndNumberTable[16] = { A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT ,         // '0'
                                 B_BIT | C_BIT ,                                         // '1'
                                 A_BIT | B_BIT | D_BIT | E_BIT | G_BIT ,                 // '2'
                                 A_BIT | B_BIT | C_BIT | D_BIT | G_BIT ,                 // '3'
                                 B_BIT | C_BIT | F_BIT | G_BIT ,                         // '4'
                                 A_BIT | C_BIT | D_BIT | F_BIT | G_BIT ,                 // '5'
                                 C_BIT | D_BIT | E_BIT | F_BIT | G_BIT ,                 // '6'
                                 A_BIT | B_BIT | C_BIT ,                                 // '7'
                                 A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT | G_BIT , // '8'
                                 A_BIT | B_BIT | C_BIT | F_BIT | G_BIT ,                 // '9'
                                 A_BIT | B_BIT | C_BIT | E_BIT | F_BIT | G_BIT ,         // 'A'
                                 C_BIT | D_BIT | E_BIT | F_BIT | G_BIT ,                 // 'b'
                                 A_BIT | D_BIT | E_BIT | F_BIT ,                         // 'C'
                                 B_BIT | C_BIT | D_BIT | E_BIT | G_BIT ,                 // 'd'
                                 A_BIT | D_BIT | E_BIT | F_BIT | G_BIT ,                 // 'E'
                                 A_BIT | E_BIT | F_BIT | G_BIT , };                      // 'F'

const int FndPinTable[8] = { FND_DB_A, FND_DB_B, FND_DB_C, FND_DB_D,
                             FND_DB_E, FND_DB_F, FND_DB_G, FND_DB_DP };

int main(void)
{
    if(wiringPiSetupGpio() == -1)
    {
        return 1;
    }

    int i, k;
    int m = 0;

    for(i=0; i<FND_DB_PIN_NUM; i++)
    {
        pinMode(FndPinTable[i], OUTPUT); // Fnd Data Pin OUTPUT Set
    }

    while(1)
    {
        for(i=0; i<MAX_FND_POSITION; i++)
        {
            digitalWrite(FndSel[i], HIGH); // Initialize FND position
        }

        digitalWrite(FndSel[m % 6], LOW);
        m++;

        for(k=0; k<MAX_CHAR; k++)
        {
            for(i=0; i<FND_DB_PIN_NUM; i++)
            {
                if(FndNumberTable[k] & (1<<i))
                {
                    digitalWrite(FndPinTable[i], HIGH);
                }
                else
                {
                    digitalWrite(FndPinTable[i], LOW);
                }
            }

            delay(500);
        }
    }

    return 0;
}
