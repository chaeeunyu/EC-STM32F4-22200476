/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-01-2026
Language/ver     : C++ in VS Code

Description      : [LAB] EXTI: counting numbers at a rate of 1s
                               Pause the counting when SW2 is pressed     
                               Resets to zero when SW1 is pressed
/----------------------------------------------------------------*/


// #include "ecRCC2.h"
// #include "ecGPIO2.h"
// #include "ecSysTick2.h"
// #include "ecEXTI2.h"
#include "ecSTM32F4v2.h"

#define SW1_PIN    PD_2         //EVAL board JKIT SW1
#define SW2_PIN    PA_4			//EVAL board JKIT SW2

PinName_t pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  // 7-segment pin
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3};

volatile int reset_flag = 0;
volatile int pause_flag = 0;

void reset_led(void){    
	// reset
	reset_flag = 1;
}

void pause_led(void){
    // pause(1) or resume(0)
    pause_flag ^= 1;
}

// Initialiization 
void setup(void)
{
    MCU_init();

    // Initialize GPIOA_4 for Input Button
    GPIO_init(SW1_PIN, INPUT);  // SW1: DIN, Pull-Up
    GPIO_pupd(SW1_PIN, PULL_UP);
    GPIO_init(SW2_PIN, INPUT);  // SW2: DIN, Pull-Up
    GPIO_pupd(SW2_PIN, PULL_UP); 

    FND_init(pinFND);           // 7-segment pins: Push-Pull, No Pull-up-Pull-down, Medium Speed
    FND_select_otype(pinFND, PUSH_PULL);
    FND_select_pupd(pinFND, NPUPD);
    FND_select_ospeed(pinFND, MEDSPEED);

    FND_select_init(selectFND);           // select display: DOUT, Push-Pull
    FND_select_otype(selectFND, PUSH_PULL);


    // Initialize EXTI PD_2 & PA_4
    EXTI_init(SW1_PIN, FALL, 10);
    EXTI_init(SW2_PIN, FALL, 10);		
}


// MAIN  ----------------------------------------
int main(void) {
	setup();

    int i = 0;
    int j = 0;
    int count = 0;
    uint8_t display0 = 0;
    uint8_t display1 = 1;
    uint32_t prevTicks = 0;

    FND_display(0);

	while (1){ // 7-segment: increase every 1 sec

        if(reset_flag == 1){    // let only main change 'count'
            count = 0;        
            reset_flag = 0;
        }
        else{
            i = count % 10;
            j = count / 10;  
        }
        
        if((msTicks - prevTicks) >= 1000){
            if(pause_flag == 0) count = (count + 1) % 60;    // pause(1), resume(0)
            prevTicks = msTicks;
        }

        FND_select(display0);
        FND_display(i);
        delay_ms(1);
        
        FND_select(display1);
        FND_display(j);
        delay_ms(1);
    }
}

void EXTI2_IRQHandler(void) {
    if(is_pending_EXTI(SW1_PIN)){ // EXTI_SW1 : reset to 0 when pressed

        reset_led();

        clear_pending_EXTI(SW1_PIN);
    }
}

void EXTI4_IRQHandler(void) {
	if (is_pending_EXTI(SW2_PIN)) {   // EXTI_SW2 : pause/resume counting

			pause_led();

            clear_pending_EXTI(SW2_PIN);
	}
}


