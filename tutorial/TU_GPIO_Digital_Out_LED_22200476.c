/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ 유채은 ]
Created          : 05-03-2021
Modified         : Sep-10-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : Tutorial Digital Out
/----------------------------------------------------------------*/



// GPIO Mode			 : Input(00), Output(01), AlterFunc(10), Analog(11, reset)
// GPIO Speed			 : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
// GPIO Output Type		: Output push-pull (0, reset), Output open drain (1)
// GPIO Push-Pull	 	: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)


#include "stm32f411xe.h"
#include "ecRCC2.h"

#define LED_PIN    PA_5		//LD2


int main(void) {	
		/* Part 1. RCC GPIOA Register Setting */
		RCC_HSI_init();
		RCC_GPIOA_enable();

/*---------------------------------------------------------------- 
                    [EXERCISE]
---------------------------------------------------------------- */
		/* Part 2. GPIO Register Setting */			
		// GPIO Mode Register : MODE=OUTPUT
		GPIOA->MODER &= ~(3UL<<(2*LED_PIN)); 	   		    	// Clear '00' for Pin 5
		GPIOA->MODER |= (1UL<<(2*LED_PIN));						// Set '01' for Pin 5
		// UL: Unsigned Long
		// GPIO Output Type Register  
		GPIOA->OTYPER &= ~(1UL<<(LED_PIN));						// Clear '00'   
                        										// 0:Push-Pull
			
		// GPIO Pull-Up/Pull-Down Register 
		GPIOA->PUPDR  &= ~(3<<(2*LED_PIN));							// 00: none
		
		// GPIO Output Speed Register 
		GPIOA->OSPEEDR &= ~(3<<(2*LED_PIN));
		GPIOA->OSPEEDR |= (2<<(2*LED_PIN));							//10:Fast Speed
	
		// Dead loop & program hangs here
		while(1){
			GPIOA->ODR |= (1UL << LED_PIN);	 		// Change only LED_PIN = H  
		}
}