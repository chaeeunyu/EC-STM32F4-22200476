/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : 유채은
Created          : 05-03-2021
Modified         : Sep-21-2026
Language/ver     : C++ in VS Code

Description      : Problem1 - choose the displays to use
                   Problem2 - count number 0 to 9 (and then 0 again)
/----------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"

#define BUTTON_PIN PA_4         // problem2

PinName_t pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};


void setup(void){
    // Intialize System Clock
    RCC_HSI_init();    

    // // Problem 1 initialize
    // FND_init(pinFND);
    // FND_select_otype(pinFND, PUSH_PULL);  
    // FND_select_ospeed(pinFND, MEDSPEED);
    // FND_select_init(selectFND); 
    // FND_select_otype(selectFND, PUSH_PULL);
     
    
	// Problem 2 initialize
    FND_init(pinFND);
    FND_select_init(selectFND);

	GPIO_init(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, PULL_UP);  
	FND_select_otype(pinFND, PUSH_PULL);
    FND_select_pupd(pinFND, NPUPD);
    FND_select_ospeed(pinFND, MEDSPEED);
    FND_select_otype(selectFND, PUSH_PULL);

};

int main(void) {
    setup();

    // ------------- Problem 1 ---------------------
    // uint8_t numDisplay1  = 5;
    // uint8_t numDisplay2  = 2;
    // uint8_t selectFND1  = 0;
    // uint8_t selectFND2  = 1;

    // while (1) {
    //     FND_select(selectFND1);
    //     FND_display(numDisplay1);
    //     delay_ms_HSI(1);         

    //     FND_select(selectFND2);
    //     FND_display(numDisplay2);
    //     delay_ms_HSI(1);	
    // }
    // // ---------------------------------------------

    // -------------------- Problem 2 -----------------------
    int buttonState = HIGH; // coz of pull-up
    int prev_buttonState = HIGH;
    int i = 1;
    uint8_t selectFND3 = 0;

    FND_display(0);
	
	while(1){

        FND_select(selectFND3);
        buttonState = GPIO_read(BUTTON_PIN);
		// display 7-segment 0 to 9
		// increase number with button push
		if(buttonState == LOW && prev_buttonState == HIGH) {
            FND_display(i);
			i = (i + 1) % 10;
            delay_ms_HSI(1);
		}
        prev_buttonState = buttonState;
		delay_ms_HSI(50);
	}
    // ---------------------------------------------------------
}

