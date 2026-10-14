/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : 유채은
Created          : 05-03-2021
Modified         : Sep-14-2026
Language/ver     : C++ in VS Code

Description      : Toggle the LED by covering the photodetector sensor.
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN PB_12
#define BUTTON_PIN PA_0

// Initialiization 
void setup(void) {
	RCC_HSI_init();	// initialize clock

	// initialize the pushbutton pin as an input:
	GPIO_init(BUTTON_PIN, INPUT); 
	GPIO_pupd(BUTTON_PIN, PULL_UP);		// BUTTON_PIN: pull-up

	// initialize the LED pin as an output:
	GPIO_init(LED_PIN, OUTPUT);    
	GPIO_pupd(LED_PIN, PULL_UP);		// LED_PIN: pull-up
	GPIO_otype(LED_PIN, OPEN_DRAIN);	// open-drain
	GPIO_ospeed(LED_PIN, MEDSPEED); 	// medium speed
}
	
int main(void) { 
 	setup();
	int buttonState=0;
	
	while(1){
		// check if the pushbutton is pressed. Turn LED on/off accordingly:
		buttonState = GPIO_read(BUTTON_PIN);
		if(buttonState)	GPIO_write(LED_PIN, LOW);
		else 		GPIO_write(LED_PIN, HIGH);
		delay_ms_HSI(100);             					// delay 100 ms
	}
}