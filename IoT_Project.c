/* SKU CoE ITE - ParkSooYoung */
/* Grade 3 , Semester 2 , Project : Smart Car */

#include <stdio.h> 		 // 표준입출력 라이브러리 헤더파일 선언
#include <stdlib.h> 	 // C 표준 라이브러리 헤더파일 선언
#include <string.h> 	 // 문자열 라이브러리 헤더파일 선언
#include <stdint.h> 	 // 정수데이터 라이브러리 헤더파일 선언
#include <errno.h> 		 // 오류 라이브러리 헤더파일 선언
#include <pthread.h> 	 // POSIX Thread 라이브러리 헤더파일 선언
#include <wiringPi.h> 	 // GPIO access library 헤더파일 선언
#include <wiringPiSPI.h> // GPIO SPI library 헤더파일 선언
#include <softPwm.h> 	 // Software PWM library 헤더파일 선언
#include <softTone.h> 	 // Tone wave signal 관련 헤더파일 선언
#include <lcd.h> 		 // Text LCD(Character LCD) 관련 헤더파일 선언

#define LED_PIN_1 5 // LED 포트 RED1 핀 정의
#define LED_PIN_2 6 // LED 포트 RED2 핀 정의

#define MOTOR_MT_P_PIN 4  // MOTOR 포트 MT_P 핀 정의
#define MOTOR_MT_N_PIN 17 // MOTOR 포트 MT_N 핀 정의

#define LEFT_ROTATE 1  // 반시계방향 회전 상수 정의
#define RIGHT_ROTATE 2 // 시계방향 회전 상수 정의

#define SERVO 18 // DC Servo Motor PWM(Orange) 핀 정의

#define BUZZER_PIN 21 // Buzzer Pin 정의 , 7음계에서 각 음에 해당하는 주파수 값 정의
#define DO_L 523 	  // 낮은 도
#define DO_S 554 	  // 도 #
#define RE 587 		  // 레
#define RE_S 622 	  // 레 #
#define MI 659 		  // 미
#define FA 698 		  // 파
#define FA_S 740 	  // 파 #
#define SOL 784 	  // 솔
#define SOL_S 831 	  // 솔 #
#define RA 880 		  // 라
#define RA_S 932 	  // 라 #
#define SI 987 		  // 시
#define DO_H 1046 	  // 높은 도

// 온습도센서 DHT11
#define MAXTIMINGS 83 // 40개 데이터 비트 * 2 (LOW, HIGH 반복 -> 데이터 1개 Read) + 3개의 첫 비트 (Response Signal, Pullup Ready Signal, Start to Transmit Bit)
#define DHTPIN 23 	  // DHT11 핀 정의

// ADC MCP3208 -> CDS Cell
#define CS_MCP3208 8 	  // MCP3208 CS 핀 정의
#define SPI_CHANNEL 0 	  // SPI 통신 채널 정의
#define SPI_SPEED 1000000 // SPI 통신 속도 정의

// 초음파센서 HC-SR04
#define TP 19 // Trigger 핀 정의
#define EP 26 // Echo 핀 정의

void MotorStop(void) // 모터 제어 정지 함수 정의
{
	softPwmWrite(MOTOR_MT_N_PIN, 0); // DC 모터 N핀에 PWM Duty Ratio 0% 할당
	softPwmWrite(MOTOR_MT_P_PIN, 0); // DC 모터 P핀에 PWM Duty Ratio 0% 할당
}

void MotorControl(unsigned char speed, unsigned char rotate) // 모터 제어 조작 함수 정의, 속도와 회전 방향을 매개변수로 사용
{
	if(rotate == LEFT_ROTATE) // 조건문 if, 회전을 반시계방향으로 설정했다면 아래 코드 실행
	{
		digitalWrite(MOTOR_MT_P_PIN, LOW);   // DC 모터 P핀에 LOW 신호 할당
		softPwmWrite(MOTOR_MT_N_PIN, speed); // DC 모터 N핀에 주어진 속도만큼 PWM Duty Ratio 퍼센테이지 할당
	}
	else if(rotate == RIGHT_ROTATE) // 조건문 if-elseif, 회전을 시계방향으로 설정했다면 아래 코드 실행
	{
		digitalWrite(MOTOR_MT_P_PIN, LOW);   // DC 모터 N핀에 LOW 신호 할당
		softPwmWrite(MOTOR_MT_N_PIN, speed); // DC 모터 P핀에 주어진 속도만큼 PWM Duty Ratio 퍼센테이지 할당
	}
}

unsigned int SevenScale (unsigned char scale) // 각 음계에 해당하는 주파수 값을 선택하는 함수
{
	unsigned int _ret = 0; // 음계 주파수 선택을 위한 부호 없는 정수형 변수 _ret, 0으로 초기화

	switch(scale) // 조건문 switch, 매개변수 scale에 대하여 아래 케이스 코드 각각 실행
	{
		case 0: 		  // scale = 0 일 때
			_ret = DO_L;  // 음계 주파수 선택 : 낮은 도
			break; 		  // 해당 케이스 탈출
		
		case 1: 		  // scale = 1 일 때
			_ret = DO_S;  // 음계 주파수 선택 : 도 #
			break; 		  // 해당 케이스 탈출
		
		case 2: 		  // scale = 2 일 때
			_ret = RE; 	  // 음계 주파수 선택 : 레
			break; 		  // 해당 케이스 탈출
		
		case 3: 		  // scale = 3 일 때
			_ret = RE_S;  // 음계 주파수 선택 : 레 #
			break; 		  // 해당 케이스 탈출
		
		case 4: 		  // scale = 4 일 때
			_ret = MI; 	  // 음계 주파수 선택 : 미
			break; 		  // 해당 케이스 탈출
		
		case 5: 		  // scale = 5 일 때
			_ret = FA; 	  // 음계 주파수 선택 : 파
			break; 		  // 해당 케이스 탈출
		
		case 6: 		  // scale = 6 일 때
			_ret = FA_S;  // 음계 주파수 선택 : 파 #
			break; 		  // 해당 케이스 탈출
		
		case 7: 		  // scale = 7 일 때
			_ret = G; 	  // 음계 주파수 선택 : 솔
			break; 		  // 해당 케이스 탈출
		
		case 8: 		  // scale = 8 일 때
			_ret = SOL_S; // 음계 주파수 선택 : 솔 #
			break; 		  // 해당 케이스 탈출
		
		case 9: 		  // scale = 9 일 때
			_ret = RA; 	  // 음계 주파수 선택 : 라
			break; 		  // 해당 케이스 탈출
		
		case 10: 		  // scale = 10 일 때
			_ret = RA_S;  // 음계 주파수 선택 : 라 #
			break; 		  // 해당 케이스 탈출
		
		case 11: 		  // scale = 11 일 때
			_ret = SI; 	  // 음계 주파수 선택 : 시
			break;		  // 해당 케이스 탈출
		
		case 12: 		  // scale = 12 일 때
			_ret = DO_H;  // 음계 주파수 선택 : 높은 도
			break; 		  // 해당 케이스 탈출
	}

	return _ret; // 선택된 음계 주파수 값 반환
}

void Change_FREQ(unsigned int freq)  // 주파수를 변경하는 함수
{
	softToneWrite(BUZZER_PIN, freq); // Buzzer 핀에 주어진 주파수만큼 할당
}

void STOP_FREQ(void) 			  // 주파수 발생 정지 함수
{
	softToneWrite(BUZZER_PIN, 0); // Buzzer 핀에 주파수 0 Hz 할당
}

void BUZZER_Init(void) 			// Buzzer 초기 설정
{
	softToneCreate(BUZZER_PIN); // Buzzer 핀에 주파수 신호 출력 설정
	STOP_FREQ(); 				// 주파수 발생 정지
}

int ReadMcp3208ADC(unsigned char adcChannel) // MCP3208을 통한 A/D Converter Read 함수
{
	unsigned char buff[3]; 						 // 3 Bytes Command Data
	int nAdcValue = 0; 	   						 // 0 0 0 0 0 Start SGL D2

	buff[0] = 0x06 | ((adcChannel & 0x07) >> 2); // D1 D0 x x x x x x
	buff[1] = ((adcChannel & 0x07) << 6); 		 // x x x x x x x x
	buff[2] = 0x00; 							 // Chip Select Low 신호 출력

	digitalWrite(CS_MCP3208, 0); 				 // 설정한 채널로부터 데이터 획득
	wiringPiSPIDataRW(SPI_CHANNEL, buff, 3); 	 // 획득한 데이터를 버퍼에 설정한 길이로 임시 저장

	buff[1] = 0x0F & buff[1]; 					 // 하위 4자리 masking
	nAdcValue = (buff[1] << 8) | buff[2]; 		 // 12비트 변환 값 저장

	digitalWrite(CS_MCP3208, 1); 				 // Chip Select High 신호 출력

	return nAdcValue; 							 // 변환 값 반환
}

float getDistance(void) // 초음파 센서를 이용한 거리 측정 함수
{
	float fDistance; 		  // 거리 값 변수
	int nStartTime, nEndTime; // 측정 시간 변수
	
	digitalWrite(TP, LOW);  // TP에 LOW(0) 출력
	delayMicroseconds(2);   // pull the Trig pin to high level for more than 10us impulse
	digitalWrite(TP, HIGH); // TP에 HIGH(1) 출력
	delayMicroseconds(10);  // 10μs 딜레이
	digitalWrite(TP, LOW);  // TP에 LOW(0) 출력
	
	while(digitalRead(EP) == LOW) // EP를 읽어 LOW인 동안 대기
	{
		nStartTime = micros(); // EP가 HIGH일 때, 시간 측정
	}
	
	while(digitalRead(EP) == HIGH) // EP를 읽어 HIGH인 동안 대기
	{
		nEndTime = micros(); // EP가 LOW일 때, 시간 측정
	}
	
	fDistance = (nEndTime - nStartTime) * 0.034 / 2.; // EP가 HIGH인 시간을 측정하여 거리 산출 (cm)
	
	return fDistance; // 거리 값 반환
}

int dht11_dat[5] = {0, }; // DHT[0]

void* read_dht11_dat(void) // 온습도 센서를 이용한 DC Servo Motor 제어 함수 (void 포인터)
{
	while(1) // 반복문 while, 무한 루프
	{
		uint8_t laststate = HIGH; // 상태 확인을 위한 8비트 부호 변수 laststate, HIGH 상태로 초기화
		uint8_t counter = 0; 	  // 카운트를 위한 8비트 부호 변수 counter, 0으로 초기화
		uint8_t j = 0, i; 		  // 반복 및 조건을 위한 8비트 부호 변수 j, 0과 i로 초기화
		
		dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0; // DHT 배열의 모든 엘리멘트를 0으로 초기화
		
		pinMode(DHTPIN, OUTPUT); 	// DHT 핀 출력 설정
		digitalWrite(DHTPIN, LOW);  // DHT 핀에 LOW 신호 할당
		delay(18); 					// 18ms 딜레이
		digitalWrite(DHTPIN, HIGH); // DHT 핀에 HIGH 신호 할당
		delayMicroseconds(30); 		// 30μs 딜레이
		pinMode(DHTPIN, INPUT); 	// DHT 핀 입력 설정
		
		for(i=0; i<MAXTIMINGS; i++) // 반복문 for, 매개변수 i, 0부터 시작하여 DHT 데이터 비트에 맞게 +1, 아래 코드 반복
		{
			counter = 0; // 카운터 0으로 초기화

			while(digitalRead(DHTPIN) == laststate) // 반복문 while, DHT의 상태 확인
			{
				counter++; 			  // 카운터 1 증가

				delayMicroseconds(1); // 1μs 딜레이

				if(counter == 200) 	  // 조건문 if, 카운터가 200이 되었을 시, 
				{
					break; 			  // 반복문 탈출
				}
			}

			laststate = digitalRead(DHTPIN); // DHT 상태 갱신
			
			if(counter == 200) // 조건문 if, 카운터가 200이 되었을 시, 
			{
				break; 		   // 반복문 탈출
			}

			if((i >= 4) && ((i % 2) == 0)) // 조건문 if, 매개변수 i, 4 이상이고 2로 나누었을 때 나머지가 없으면 아래 코드 실행
			{
				dht11_dat[j / 8] <<= 1; // DHT 배열의 인덱스들을 8로 나누고 비트 이동

				if(counter > 20) // 조건문 if, 카운터가 20 초과일 시, 
				{
					dht11_dat[j / 8] |= 1; // DHT 배열의 인덱스들을 8로 나누고 OR 연산
				}

				j++; // 반복 및 조건 매개변수 1 증가
			}
		}
		
		if((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xff))) // 조건문 if, 매개변수와 DHT 배열 연산이 일치할 시, 
		{
			printf("humidity = %d.%d %% Temperature = %d.%d *C\n", dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]); // 온습도 메시지 출력
			
			if((dht11_dat[2] < 18) || (dht11_dat[2] > 21)) // 겨울철 적정 온도인 18 ~ 22도 사이가 아니면 서보모터 제어
			{	
				softPwmWrite(SERVO, 15); // 0 degree
				delay(300); 			 // 300ms 딜레이
				softPwmWrite(SERVO, 25); // 90 degree
				delay(300); 			 // 300ms 딜레이
				softPwmWrite(SERVO, 5);  // -90 degree
				delay(300);				 // 300ms 딜레이
			}
			else // 습도의 경우 온도와 마찬가지로 적정습도 범위를 벗어날 때 서보모터를 제어할 수 있지만 서보모터의 갯수 제한으로 온도로만 작동하도록 설정
			{
				printf("Data get failed\n"); // 습도가 높을 때 -> 난방기 또는 제습기 -> 차량의 에어컨으로 작동
			}
		}
	}
}

int nCdsChannel = 0; // ADC 채널 변수 선언
int nCdsValue = 0; 	 // ADC 값 변수 선언

void* cds_led(void) // 조도 센서를 이용한 LED 제어 함수 (void 포인터)
{
	while(1) // 반복문 while, 무한 루프
	{
		nCdsValue = ReadMcp3208ADC(nCdsChannel); // ADC 채널에 입력되는 값을 변환하여 ADC 값 변수에 저장

		printf("Cds Sensor Value = %u\n", nCdsValue); // 조도 메시지 출력

		if(nCdsValue <= 500) // 조건문 if, 조도가 500 이하일 시, 
		{
			digitalWrite(LED_PIN_1,HIGH); // LED 포트 RED1 핀에 HIGH 신호 할당
			digitalWrite(LED_PIN_2,LOW);  // LED 포트 RED2 핀에 LOW 신호 할당
		}
		else // 조건문 if-else, 조도가 500 이상일 시, 
		{
			digitalWrite(LED_PIN_1,LOW); // LED 포트 RED1 핀에 LOW 신호 할당
			digitalWrite(LED_PIN_2,LOW); // LED 포트 RED2 핀에 LOW 신호 할당
		}

		delay(1000); // 1000ms 딜레이
	}
}

void play_sound(int dis) // 거리 별 소리 출력 함수, 매개변수 dis
{
	if(dis == 1) // 거리가 70cm 초과 100cm 이하일 경우 
	{
		Change_FREQ(SevenScale(8)); // '솔 #' 출력 
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(400); 				// 400ms 딜레이
		Change_FREQ(SevenScale(8)); // '솔 #' 출력 
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(400); 				// 400ms 딜레이
	}
	
	if(dis == 2) // 거리가 50cm 초과 70cm 이하일 경우
	{
		Change_FREQ(SevenScale(8)); // '솔 #' 출력 
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(233); 				// 233ms 딜레이
		Change_FREQ(SevenScale(8)); // '솔 #' 출력 
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(233); 				// 233ms 딜레이
		Change_FREQ(SevenScale(8)); // '솔 #' 출력 
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(233); 				// 233ms 딜레이
	}
	
	if(dis == 3) // 거리가 30cm 초과 50cm 이하일 경우
	{
		Change_FREQ(SevenScale(8)); // '솔 #' 출력 
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(150); 				// 150ms 딜레이
		Change_FREQ(SevenScale(8)); // '솔 #' 출력
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(150); 				// 150ms 딜레이
		Change_FREQ(SevenScale(8)); // '솔 #' 출력
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(150); 				// 150ms 딜레이
		Change_FREQ(SevenScale(8)); // '솔 #' 출력
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(150); 				// 150ms 딜레이
	}
	
	if(dis == 4) // 거리가 30cm 이하일 경우 
	{
		Change_FREQ(SevenScale(8)); // '솔 #' 출력
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(100); 				// 100ms 딜레이
		Change_FREQ(SevenScale(8)); // '솔 #' 출력
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(100); 				// 100ms 딜레이
		Change_FREQ(SevenScale(8)); // '솔 #' 출력
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(100); 				// 100ms 딜레이
		Change_FREQ(SevenScale(8)); // '솔 #' 출력
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(100); 				// 100ms 딜레이
		Change_FREQ(SevenScale(8)); // '솔 #' 출력
		delay(100); 				// 100ms 동안 지속
		STOP_FREQ(); 				// 소리 출력 정지
		delay(100); 				// 100ms 딜레이
	}	
}

void* warning_sound(void) // 경고 소리 출력 함수 (void 포인터)
{
	while(1) // 반복문 while, 무한 루프
	{
		float fDistance = getDistance(); // 거리를 측정 후 실수형 변수 fDistance에 저장
		
		if(fDistance > 15) // 거리가 15(30cm) 초과일 시, 
		{
			MotorControl(80, RIGHT_ROTATE); // DC 모터 조작, PWM 80 및 시계방향 회전 설정
			digitalWrite(LED_PIN_2,LOW); 	// LED 포트 RED2 핀에 LOW 신호 할당
			
			if(fDistance <= 45 && fDistance > 35) // 거리가 35 초과 45 이하일 경우 
			{
				play_sound(1); 							 // 거리 70cm 초과 100cm 이하 소리 출력
				printf("Distance: %.2fcm\n", fDistance); // 거리 메시지 출력
			}
			else if(fDistance <= 35 && fDistance > 25) // 거리가 25 초과 35 이하일 경우 
			{
				play_sound(2); 							 // 거리 50cm 초과 70cm 이하 소리 출력
				printf("Distance: %.2fcm\n", fDistance); // 거리 메시지 출력
			}
			else if(fDistance <= 25 && fDistance > 15) // 거리가 15 초과 25 이하일 경우 
			{
				play_sound(3); 							 // 거리 30cm 초과 50cm 이하 소리 출력
				printf("Distance: %.2fcm\n", fDistance); // 거리 메시지 출력
			}
			else // 그 외 거리의 경우 LCD에만 거리 출력 및 소리 출력 없음
			{
				printf("Distance: %.2fcm\n", fDistance); // 거리 메시지 출력
				delay(500); 							 // 500ms 딜레이
			}
		}
		else if(fDistance <= 15) // 거리가 15(30cm) 이하일 시, 
		{	
			MotorStop(); 				  			 // DC 모터 정지
			digitalWrite(LED_PIN_2,HIGH); 			 // LED 포트 RED2 핀에 HIGH 신호 할당
			play_sound(4); 				  			 // 거리 30cm 이하 소리 출력
			printf("Distance: %.2fcm\n", fDistance); // 거리 메시지 출력
		}
	}
}

int main(void) // 메인 함수 시작
{
	pthread_t Motor_f, Cds_f, Dht11_f; // POSIX Thread 변수 선언 (모터, 조도센서, 온습도센서)
	
	if(wiringPiSetupGpio() == -1) // Wiring Pi의 GPIO를 사용하기 위한 설정
	{
		fprintf(stdout, "Not start wiringPi: %s\n", strerror(errno)); // Wiring Pi가 설정되지 않았다면 에러 출력
		return 1; 													  // 종료
	}
	if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) // Wiring Pi의 SPI를 사용하기 위한 설정
	{
		fprintf(stdout, "wiringPiSPISetup Failed: %s\n", strerror(errno)); // Wiring Pi가 설정되지 않았다면 에러 출력
		return 1; 														   // 종료
	}
	
	pinMode(LED_PIN_1, OUTPUT); // LED1 PORT SET
	pinMode(LED_PIN_2, OUTPUT); // LED2 PORT SET
	
	pinMode(MOTOR_MT_N_PIN, OUTPUT); // MT_N 핀 출력 설정
	pinMode(MOTOR_MT_P_PIN, OUTPUT); // MT_P 핀 출력 설정

	softPwmCreate(MOTOR_MT_N_PIN, 0, 100); // MT_N 핀 PWM 설정
	softPwmCreate(MOTOR_MT_P_PIN, 0, 100); // MT_P 핀 PWM 설정

	softPwmCreate(SERVO, 0, 200); // DC Servo Motor 핀 PWM 설정

	BUZZER_Init(); // Buzzer 초기화

	pinMode(CS_MCP3208, OUTPUT); // MCP3208 CS 핀 출력 설정
	
	pinMode(TP, OUTPUT); // Trigger 핀 출력 설정
	pinMode(EP, INPUT);  // Echo 핀 출력 설정
	
	pthread_create(&Motor_f, NULL, warning_sound, NULL);  // pthread 생성, 모터와 경고 소리
	pthread_create(&Cds_f, NULL, cds_led, NULL); 		  // pthread 생성, 조도센서와 LED
	pthread_create(&Dht11_f, NULL, read_dht11_dat, NULL); // pthread 생성, 온습도센서와 서보모터
	
	pthread_join(Motor_f, NULL); // 모터 pthread 종료 시 자원 해제
	pthread_join(Cds_f, NULL); 	 // 조도센서 pthread 종료 시 자원 해제
	pthread_join(Dht11_f, NULL); // 온습도센서 pthread 종료 시 자원 해제
	
	return 0; // 프로그램 종료
}
