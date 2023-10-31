#include <wiringPi.h> 
#include <softPwm.h>  
#include <softTone.h> 
#include <lcd.h>      

#define KEYPAD_SIZE 12
#define KEYPAD_PB1 26  
#define KEYPAD_PB2 19  
#define KEYPAD_PB3 13  
#define KEYPAD_PB5 10  
#define KEYPAD_PB6 9   
#define KEYPAD_PB7 11  
#define KEYPAD_PB9 25  
#define KEYPAD_PB10 8  
#define KEYPAD_PB11 7  
#define KEYPAD_PB13 22 
#define KEYPAD_PB14 23 
#define KEYPAD_PB15 24 

#define MAX_KEY_BT_NUM 12 

#define SERVO 18 

#define BUZZER_PIN 17 
#define DO_L 523      
#define RE 587        
#define MI 659        
#define FA 698        
#define SOL 784       
#define RA 880        
#define SI 987        
#define DO_H 1046     
#define RE_H 1174
#define MI_H 1318
#define FA_H 1396
#define SOL_H 1567

const int KeypadTable[12] = {KEYPAD_PB1, KEYPAD_PB2, KEYPAD_PB3,
                             KEYPAD_PB5, KEYPAD_PB6, KEYPAD_PB7,
                             KEYPAD_PB9, KEYPAD_PB10, KEYPAD_PB11,
                             KEYPAD_PB13, KEYPAD_PB14, KEYPAD_PB15};                  
const char number[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '>', '0', 'E'}; 

void BUZZER_Init(void);
unsigned int SevenScale(unsigned char scale);
void Change_FREQ(unsigned int freq);
void STOP_FREQ(void);
int s = 1; 

int KeypadRead(int s) 
{
    int i = 0;            
    int nKeypadstate = 0; 

    BUZZER_Init();                       
    for (i = 0; i < MAX_KEY_BT_NUM; i++) 
    {
        if (!digitalRead(KeypadTable[i])) 
        {
            nKeypadstate |= (1 << i); 
            if(s==1){
                Change_FREQ(SevenScale(i));
                delay(100);
                STOP_FREQ();
            }
        }
    }

    return nKeypadstate; 
}

unsigned int SevenScale(unsigned char scale) 
{
    unsigned int _ret = 0; 

    switch (scale) 
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

    case 8:
        _ret = RE_H;
        break;
    case 9:
        _ret = MI_H;
        break;
    case 10:
        _ret = FA_H;
        break;
    case 11:
        _ret = SOL_H;
        break;
    }

    return _ret; 
}

void Change_FREQ(unsigned int freq) 
{
    softToneWrite(BUZZER_PIN, freq); 
}

void STOP_FREQ(void) 
{
    softToneWrite(BUZZER_PIN, 0); 
}

void BUZZER_Init(void) 
{
    softToneCreate(BUZZER_PIN); 
    STOP_FREQ();                
}

void ServoControl(unsigned char str) 
{
    if (str == 'C') 
    {
        softPwmWrite(SERVO, 15); 
    }
    else if (str == 'R') 
    {
        softPwmWrite(SERVO, 25); 
    }
    else if (str == 'L') 
    {
        softPwmWrite(SERVO, 5); 
    }
}

int main(void) 
{
    if (wiringPiSetupGpio() == -1) 
    {
        return 1; 
    }

    int i;               
    int j = 0;           
    int k = 0;           
    int c = 0;           
    int s = 1;           
    int nKeypadstate;    
    int inkey;           
    int prekey = -1;     
    int numcount = 0;    
    int misscount = 0;   
    int input_PW = 0;    
    int set_PW = 0;      
    int change_PW = 0;   
    int sound_ONOFF = 0; 
    int page = 0;        
    int disp;            
    int keke = 0;

    char pw[1][5] = {
        '0',
        '0',
        '0',
        '0',
    }; 
    char num[6] = {
        '*',
        '*',
        '*',
        '*',
    }; 

    char as_is_pw[1][5] = {
        '*',
        '*',
        '*',
        '*',
    }; 
    char to_be_pw[1][5] = {
        '*',
        '*',
        '*',
        '*',
    }; 

    softPwmCreate(SERVO, 0, 200);                               
    BUZZER_Init();                                              
    disp = lcdInit(2, 16, 4, 2, 4, 20, 21, 12, 16, 0, 0, 0, 0); 
    lcdClear(disp);                                             

    for (i = 0; i < MAX_KEY_BT_NUM; i++) 
    {
        pinMode(KeypadTable[i], INPUT); 
    }

    while (1) 
    {
        nKeypadstate = KeypadRead(s); 
        inkey = -1;                  

        for (i = 0; i < MAX_KEY_BT_NUM; i++) 
        {
            if (nKeypadstate & (1 << i)) 
            {
                inkey = i; 
            }
        }

        if ((prekey != inkey) && (inkey != -1)) 
        {
            if ((page == 0) && (inkey == 9)) 
            {
                page = 1; 
            }
            else if ((page == 1) && (inkey == 9)) 
            {
                page = 0; 
            }

            if ((page == 0) && (inkey == 0)) 
            {
                input_PW = 1; 
                page = -1;    
            }
            else if ((page == 0) && (inkey == 1)) 
            {
                set_PW = 1; 
                page = -1;  
            }
            else if ((page == 1) && (inkey == 2)) 
            {
                change_PW = 1; 
                page = -1;     
            }
            else if ((page == 1) && (inkey == 4)) 
            {
                sound_ONOFF = 1; 
                page = -1;       
            }

            delay(50); 
        }
        else if ((prekey != inkey) && (inkey == -1)) 
        {
            delay(50); 
        }

        if ((prekey != inkey) && (inkey != -1)) 
        {
        SET:                 
            if (set_PW == 1) 
            {
                lcdClear(disp);             
                lcdPosition(disp, 0, 0);    
                lcdPuts(disp, "2. Set PW"); 
                delay(50);                  

                num[j] = number[inkey]; 
                j++;                    

                if (j == 6) 
                {
                    if (inkey == 11) 
                    {
                        set_PW = 0; 
                        page = 0;   
                        j = 0;      

                        Change_FREQ(SI * s); 
                        delay(100);          
                        STOP_FREQ();         

                        lcdClear(disp);          
                        lcdPosition(disp, 0, 0); 
                        lcdPuts(disp, "Done");   

                        for (i = 0; i < 4; i++) 
                        {
                            pw[0][i] = num[i + 1]; 
                        }

                        for (i = 0; i < 4; i++) 
                        {
                            lcdPosition(disp, i, 1);  
                            lcdPuts(disp, &pw[0][i]); 
                            delay(50);                
                        }

                        delay(2000); 
                    }
                    else 
                    {
                        Change_FREQ(FA * s); 
                        delay(100);          
                        STOP_FREQ();         

                        lcdClear(disp);                    
                        lcdPosition(disp, 0, 0);           
                        lcdPuts(disp, "Invalid password"); 
                        delay(2000);                       

                        j = 0; 

                        goto SET; 
                    }
                }
                else if (inkey == 9) 
                {
                    j = 1; 
                }
                else if (inkey == 11) 
                {
                    j = 1; 
                }
            }
            else if (change_PW == 1) 
            {
                lcdClear(disp);                
                lcdPosition(disp, 0, 0);       
                lcdPuts(disp, "3. Change PW"); 
                lcdPosition(disp, 0, 1);       
                

                if (numcount % 2 == 0)
                {
                    lcdPuts(disp, "Input past PW");
                }
                else
                {
                    lcdPuts(disp, "Input new PW");
                }

                delay(50); 

                num[c] = number[inkey]; 
                c++;                    

                if (numcount == 1) 
                {
                    if (c == 6) 
                    {
                        if (inkey == 11) 
                        {
                            lcdClear(disp);           
                            lcdPosition(disp, 0, 0);  
                            lcdPuts(disp, "As is: "); 
                            lcdPosition(disp, 0, 1);  
                            lcdPuts(disp, "To be: "); 

                            for (i = 0; i < 4; i++) 
                            {
                                lcdPosition(disp, i + 7, 0);    
                                lcdPuts(disp, &as_is_pw[0][i]); 
                                delay(50);                      
                            }

                            for (i = 0; i < 4; i++) 
                            {
                                to_be_pw[0][i] = num[i + 1]; 
                            }

                            for (i = 0; i < 4; i++) 
                            {
                                lcdPosition(disp, i + 7, 1);    
                                lcdPuts(disp, &to_be_pw[0][i]); 
                                delay(50);                      
                            }

                            delay(2000); 

                            for (i = 0; i < 4; i++)
                            {
                                pw[0][i] = to_be_pw[0][i]; 
                            }

                            Change_FREQ(RA * s); 
                            delay(100);          
                            STOP_FREQ();         

                            lcdClear(disp);          
                            lcdPosition(disp, 0, 0); 
                            lcdPuts(disp, "Done");   

                            delay(2000); 

                            change_PW = 0; 
                            page = 0;      
                            numcount = 0;  
                            c = 0;         
                        }
                        else 
                        {
                            Change_FREQ(FA * s); 
                            delay(100);          
                            STOP_FREQ();         

                            lcdClear(disp);                    
                            lcdPosition(disp, 0, 0);           
                            lcdPuts(disp, "Invalid password"); 

                            delay(2000); 
                        }

                        c = 1; 
                    }
                    else if (inkey == 9) 
                    {
                        c = 1; 
                    }
                    else if (inkey == 11) 
                    {
                        c = 1; 
                    }
                }
                else if (c == 6) 
                {
                    if (inkey == 11) 
                    {
                        for (i = 0; i < 4; i++) 
                        {
                            as_is_pw[0][i] = num[i + 1]; 
                        }

                        for (i = 0; i < 4; i++) 
                        {
                            if (pw[0][i] != as_is_pw[0][i]) 
                            {
                                i = -1; 
                                c = 0;  

                                Change_FREQ(FA * s); 
                                delay(100);          
                                STOP_FREQ();         

                                lcdClear(disp);                    
                                lcdPosition(disp, 0, 0);           
                                lcdPuts(disp, "Invalid password"); 

                                delay(2000); 

                                lcdPosition(disp, 0, 1);        
                                lcdPuts(disp, "Retry Press E"); 
                                delay(50);                      

                                break; 
                            }
                        }

                        if (i == 4) 
                        {
                            Change_FREQ(SI * s); 
                            delay(100);          
                            STOP_FREQ();         

                            lcdClear(disp);                
                            lcdPosition(disp, 0, 0);       
                            lcdPuts(disp, "Correct!");     
                            lcdPosition(disp, 0, 1);       
                            lcdPuts(disp, "Input New PW"); 

                            c = 0;        
                            numcount = 1; 
                            
                        }
                    }
                }

                delay(50); 
            }
            else if (sound_ONOFF == 1) 
            {
                if (s == 1) 
                {
                    s = 0;           
                    sound_ONOFF = 0; 
                    page = 0;        

                    lcdClear(disp);             
                    lcdPosition(disp, 0, 0);    
                    lcdPuts(disp, "Sound off"); 

                    delay(2000); 
                }
                else if (s == 0) 
                {
                    s = 1;           
                    sound_ONOFF = 0; 
                    page = 0;        

                    lcdClear(disp);            
                    lcdPosition(disp, 0, 0);   
                    lcdPuts(disp, "Sound on"); 

                    delay(2000); 
                }
            }
            else if (input_PW == 1) 
            {
                lcdClear(disp);               
                lcdPosition(disp, 0, 0);      
                lcdPuts(disp, "1. Input PW"); 
                delay(50);                    

                num[k] = number[inkey]; 
                k++;                    

                if (k == 6) 
                {
                    if (inkey == 11) 
                    {
                        int match = 1;
                        for (i = 0; i < 4; i++) 
                        {
                            if (pw[0][i] != num[i + 1]) 
                            {
                                i = -1;      
                                misscount++; 

                                Change_FREQ(FA * s); 
                                delay(100);          
                                STOP_FREQ();         

                                lcdClear(disp);                    
                                lcdPosition(disp, 0, 0);           
                                lcdPuts(disp, "Invalid password"); 

                                if (misscount == 3) 
                                {
                                    lcdClear(disp);            
                                    lcdPosition(disp, 0, 0);   
                                    lcdPuts(disp, "No more!"); 

                                    Change_FREQ(SOL * s); 
                                    delay(400);           
                                    STOP_FREQ();          
                                    Change_FREQ(RA * s);  
                                    delay(400);           
                                    STOP_FREQ();          
                                    Change_FREQ(SI * s);  
                                    delay(400);           
                                    STOP_FREQ();          

                                    lcdPosition(disp, 0, 1);          
                                    lcdPuts(disp, "Retry after 10s"); 
                                    delay(100);                       

                                    for (i = 0; i < 10; i++) 
                                    {
                                        delay(1000);         
                                        Change_FREQ(RA * s); 
                                        delay(100);          
                                        STOP_FREQ();         
                                    }

                                    delay(900);          
                                    Change_FREQ(RE * s); 
                                    delay(100);          
                                    STOP_FREQ();         

                                    misscount = 0; 
                                    input_PW = 0;  
                                    page = 0;      
                                    k = 0;         
                                }

                                delay(2000); 

                                lcdPosition(disp, 0, 1);        
                                lcdPuts(disp, "Retry press E"); 
                                delay(50);                      

                                k = 1; 

                                match = 0;
                                break; 
                            }

                            if (match) 
                            {
                                Change_FREQ(DO_H * s); 
                                delay(100);            
                                STOP_FREQ();           

                                ServoControl('R'); 

                                lcdClear(disp);               
                                lcdPosition(disp, 0, 0);      
                                lcdPuts(disp, "Door opened"); 

                                delay(2000); 

                                Change_FREQ(DO_H * s); 
                                delay(100);            
                                STOP_FREQ();           

                                ServoControl('C'); 

                                misscount = 0; 
                                input_PW = 0;  
                                page = 0;      
                                k = 0;         
                                break;
                            }
                            else 
                            {
                                k = 1; 
                            }
                        }
                    }
                    else if (inkey != 11) 
                    {
                        k = 1; 

                        lcdClear(disp);                    
                        lcdPosition(disp, 0, 0);           
                        lcdPuts(disp, "Invalid password"); 

                        delay(50); 

                        Change_FREQ(FA * s); 
                        delay(100);          
                        STOP_FREQ();         

                        delay(2000); 

                        lcdPosition(disp, 0, 1);        
                        lcdPuts(disp, "Retry press E"); 
                        delay(50);                      
                    }
                }
                else if (inkey == 9) 
                {
                    k = 1; 
                }
                else if (inkey == 11) 
                {
                    k = 1; 
                }

                delay(50); 
            }
        }
        else if ((prekey != inkey) && (inkey == -1)) 
        {
            delay(50); 
        }

        if (page == 0) 
        {
            lcdClear(disp);                    
            lcdPosition(disp, 0, 0);           
            lcdPuts(disp, "1. Input PW");      
            lcdPosition(disp, 0, 1);           
            lcdPuts(disp, "2. Set PW   *"); 
            delay(100);                        
        }
        else if (page == 1) 
        {
            lcdClear(disp);                     
            lcdPosition(disp, 0, 0);            
            lcdPuts(disp, "3. Change PW");      
            lcdPosition(disp, 0, 1);            
            lcdPuts(disp, "4. Sound on/off"); 
            delay(100);                         
        }

        prekey = inkey; 

        STOP_FREQ(); 
    }
}
