/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-29-2026 
Language/ver     : C++ in VS Code

Description      : TU time interrupt test code
/----------------------------------------------------------------*/


#include "ecSTM32F4v2.h"
#include "stm32f411xe.h"


#define LD2_PIN PA_5
uint32_t _count = 0;
// led_toggle() function should have been defined in ecGPIO2.c


// Initialization
void setup(void){
    RCC_PLL_init();
    SysTick_init();
    GPIO_init(LD2_PIN, OUTPUT);

    // TIM2 Update-Event Interrupt every 1 msec
    TIM_UI_init_ms(TIM2, 1);           // UEV every 1 msec
    //TIM_UI_init_us(TIM2, 1000);   // UEV every 1000usec=1msec
}

int main(void) {
    // Initialization --------------------------------------------------
    setup();

    // Infinite Loop ---------------------------------------------------
    while(1){}
}


// TIM2 Update Interrupt Handler
void TIM2_IRQHandler(void){
    if(is_UIF(TIM2)){           // Check UIF(update interrupt flag)
        _count++;
        if (_count >= 1000) {
            led_toggle(LD2_PIN);    // Toggle every 1000 msec
            _count = 0;
        }
        clear_UIF(TIM2);        // Clear TIM_UI flag
    }
}