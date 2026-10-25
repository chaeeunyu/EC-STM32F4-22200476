/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Student ID       : 22200476
Created          : Oct-22-2026 
Language/ver     : C in VS Code

Description      : EC Test 1 (2026) Programming Test
                   
                   Rename this file to  EC_Test1_yourID.c
/----------------------------------------------------------------*/


#include "stm32f411xe.h"

//==================================================================
//  EC Library Include
//==================================================================
#include "ecGPIO2.h"
#include "ecRCC2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
#include "ecStepper2.h"

//==================================================================
// User GPIO Pin definitions
//==================================================================
#define SW1_PIN    PD_2         // increase the motor speed
#define SW2_PIN    PA_4			// decreases the motor speed
#define PHOTO_SENSOR PA_0
#define LED_PIN PB_12 // LED OUT

#define LOW_SPEED  2
#define MED_SPEED  8
#define HIGH_SPEED 20

// Stepper ( A, B, AN, BN )
PinName_t pinStepper[4]    = {PC_5,PC_6,PC_7,PC_8};

// FND segments
PinName_t pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};

// FND digit select : FND_0 ~ FND_3
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

// Global Variables
// use   volatile uint32_t   instead of uint32_t if used in Interrupt ISR
volatile uint32_t motor_mode   = 0;	
volatile uint32_t motor_dir    = 0;	
volatile uint32_t motor_speed  = 0;	
// YOUR CODE GOES HERE
// YOUR CODE GOES HERE
// YOUR CODE GOES HERE

volatile int speed_flag = 0;
volatile int photo_state = 0;

void increase_speed(void){
	if(speed_flag == 0) speed_flag++;    // mode 1
	else if(speed_flag == 1) speed_flag++;   // mode 2
	else if(speed_flag == 2 || speed_flag == 3) speed_flag = 3;   // mode 3
}

void decrease_speed(void){
	if(speed_flag == 0 || speed_flag == 1) speed_flag = 0;    // mode 0
	else if(speed_flag == 2) speed_flag = 1;   // mode 1
	else if(speed_flag == 3) speed_flag = 2;   // mode 2
}


//==================================================================
//  Setup 
//==================================================================
void setup(void){
	RCC_PLL_init();					// System Clock = 84MHz
	SysTick_init();					// SysTick, 1 msec
	
	GPIO_init(SW1_PIN, INPUT);           		// GPIOC pin13 initialization
	GPIO_pupd(SW1_PIN, PULL_UP);
    EXTI_init(SW1_PIN, FALL, 10);           		// External Interrupt Setting

	GPIO_init(SW2_PIN, INPUT);           		// GPIOC pin13 initialization
	GPIO_pupd(SW2_PIN, PULL_UP);
    EXTI_init(SW2_PIN, FALL, 10);           		// External Interrupt Setting

	stepper_init(FULL, pinStepper); 				// Stepper GPIO pin initialization
	//stepper_speed(5);   

	GPIO_init(PHOTO_SENSOR, INPUT); 
	GPIO_pupd(PHOTO_SENSOR, PULL_UP);		// BUTTON_PIN: pull-up

	// initialize the LED pin as an output:
	GPIO_init(LED_PIN, OUTPUT);    
	
	// USART2, 9600 baud
	// YOUR CODE GOES HERE			
	// YOUR CODE GOES HERE			

	// GPIO Inputs
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE

	// EXTI 
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	
	// GPIO Outputs	
	FND_init(pinFND);
	FND_select_otype(pinFND, PUSH_PULL);

	FND_select_init(selectFND);
	FND_select_otype(selectFND, PUSH_PULL);

	// Other Setup
	// YOUR CODE GOES HERE
}


//==================================================================
//  Main
//==================================================================
int main(void){
	setup();

	uint32_t steps= 1 ;

	// initial state (Mode 0)
	stepper_stop();
	motor_dir = DIR_CW;

	uint8_t display0 = 0;
    uint8_t display1 = 1;

	FND_display(0);


	while(1){

		// Check PAUSE/RESUME 
		// YOUR CODE GOES HERE
		// YOUR CODE GOES HERE

		// Related to controlling motor speed/dir etc
		// HINT:  use  stepper_speed(vel);	only when vel>0
		
		if(speed_flag == 0) stepper_stop();  // 0으로 되돌아오면 그 다음부터 버튼이 작동을 안함
											 // 분명 초기상태인데 미세하게 모터가 회전함 
		else if(speed_flag == 1) stepper_speed(LOW_SPEED);
		else if(speed_flag == 2) stepper_speed(MED_SPEED);
		else if(speed_flag == 3) stepper_speed(HIGH_SPEED);

		stepper_step(steps, motor_dir);	 // Keep step=1 or a low value to stop blocking other code


		// Print to the serial monitor every 2 sec
		// PAUSE, blink at 2 sec
		//     e.g.  printf("Mode=%lu Speed=%lu DIR=%lu\r\n", ... );
		// HINT:  msTicks in ecSysTick2.h  is increasing at a 1 msec rate accurately.
		// You can use msTicks  to count every 2000 msec 
		photo_state = GPIO_read(PHOTO_SENSOR);
		if(photo_state){
			GPIO_write(LED_PIN, LOW);
			//stepper_pause();
		}	
		else{
			GPIO_write(LED_PIN, HIGH);
			//stepper_pause();
		} 		

		// FND_select(display0);
        // FND_display(i);
        // delay_ms(1);
        
        // FND_select(display1);
        // FND_display(j);
        // delay_ms(1);

			
	delay_ms(1);  // use this delay
	}
}


//==================================================================
//  EXTI Interrupt Service Routines
//==================================================================

void EXTI2_IRQHandler(void) {
	if(is_pending_EXTI(SW1_PIN)){ // EXTI_SW1 : increases motor speed
		// mode up: 0->1->2->3->3->3...
		increase_speed();
        clear_pending_EXTI(SW1_PIN);
    }
}

void EXTI4_IRQHandler(void) {
	if (is_pending_EXTI(SW2_PIN)) {   // EXTI_SW2 : decreases motor speed
		// mode down: 3>2>1->1->1->1...
		decrease_speed();
        clear_pending_EXTI(SW2_PIN);
	}
}

//==================================================================
//  UART2 Interrupt Service Routines
//==================================================================
void USART2_IRQHandler(void){
	if(is_USART2_RXNE()){
	
	// HINT: 	comparing a character:  
	//  		uint8_t ch = USART2_read();		
	//			if(ch == 'C' || ch == 'c')	 etc..
	//
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	}
}
