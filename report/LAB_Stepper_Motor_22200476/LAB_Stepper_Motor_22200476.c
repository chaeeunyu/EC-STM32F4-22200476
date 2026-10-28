/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-15-2026
Language/ver     : C++ in VS Code

Description      : Stepper Motor
				   Rotating the stepper motor (DIR: CW / CCW)
				   Pausing & resuming the motor by pressing the button(PC_13)
/----------------------------------------------------------------*/


#include "stm32f411xe.h"
#include "ecSTM32F4v2.h"

// ( A, B,  AN,  BN)
PinName_t pinStepper[4]    = {PB_10,PB_4,PB_5,PB_3};
#define BUTTON_PIN  PC_13


// Initialiization 
void setup(void){	
	RCC_PLL_init();                                 // System Clock = 84MHz
	SysTick_init();                                 // Systick init
	
	GPIO_init(BUTTON_PIN, INPUT);           		// GPIOC pin13 initialization
	GPIO_pupd(BUTTON_PIN, PULL_UP);
    EXTI_init(BUTTON_PIN, FALL, 10);           		// External Interrupt Setting

	stepper_init(FULL, pinStepper); 				// Stepper GPIO pin initialization  <------------------- MODIFY(FULL/HALF)
	stepper_speed(5, FULLSTEP_PER_REV);             //  set stepper motor speed, 5 rpm  <------------------- MODIFY(FULL/HALF)
}


int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	uint32_t rev=1;
	uint32_t steps=FULLSTEP_PER_REV*rev;                       //     <------------------------------------ MODIFY(FULL/HALF)
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		stepper_step(steps, DIR_CW);  // (Step : 2048, Direction : 0 or 1, Mode : FULL or HALF)
		// delay_ms(2000);
		// stepper_step(steps, DIR_CCW);  // (Step : 2048, Direction : 0 or 1, Mode : FULL or HALF)
		// delay_ms(2000);
	}
}


void EXTI15_10_IRQHandler(void) {  
	if (is_pending_EXTI(BUTTON_PIN)) {
		stepper_pause();
		clear_pending_EXTI(BUTTON_PIN); // cleared by writing '1'
	}
}