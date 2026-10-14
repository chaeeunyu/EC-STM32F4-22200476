/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : 유채은
Created          : 05-03-2021
Modified         : Sep-19-2026
Language/ver     : C++ in VS Code

Description      : Each time Button PA4 is pressed, turn on one LED at a time in a sequential loop
                    (e.g., LED0 -> LED1 -> LED2 -> LED3 -> LED0)
/----------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"

#define BUTTON_PIN PA_4

PinName_t pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
PinName_t selectFND[4] = {PB_12, PB_13, PB_14, PB_15};


void setup(void){

    // Intialize System Clock
    RCC_HSI_init();    
    // Initialize button pin
    GPIO_init(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, PULL_UP);
    // Intialize FND pins and Others
    FND_init(pinFND);
    FND_select_init(selectFND); 
    FND_select_pupd(selectFND, PULL_UP);
    FND_select_otype(selectFND, OPEN_DRAIN);
    FND_select_ospeed(selectFND, MEDSPEED);      

}

int main(void) {
    setup();
    int buttonState = HIGH; // coz of pull-up
    int prev_buttonState = HIGH;
    int i = 0;

    GPIO_write(selectFND[0], HIGH);

    while (1) {

        buttonState = GPIO_read(BUTTON_PIN);
        if(buttonState == LOW && prev_buttonState == HIGH){
            GPIO_write(selectFND[i], LOW);
            i = (i + 1) % 4;
            GPIO_write(selectFND[i], HIGH);
        }
        prev_buttonState = buttonState;
        delay_ms_HSI(30);             					// delay 100 ms

    }
}

