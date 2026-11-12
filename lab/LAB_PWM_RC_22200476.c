/*----------------------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Nov-02-2026
Language/ver     : C in VS Code

Description      : LAB_PWM_RC
                   1) Increase the angle of RC servo motor from 0° to 180° 
                     by 10° every 500msec.
                   2) After reaching 180°, decrease the angle back to 0°
                   3) user button (PC13) -> reset to the angle 0°
/----------------------------------------------------------------------------*/

#include "stm32f411xe.h"
#include "ecSTM32F4v2.h"

#define     PWM_RC       PA_1
#define     BUTTON_PIN   PC_13
#define     MAX_US       2500      // 180[deg]
#define     MIN_US       500       // 0[deg]
#define     ANGLE_MAX    180
#define     MOVE_BY      10     // [deg]  <----------------- MODIFY
#define     MAX_STEP     (ANGLE_MAX / MOVE_BY)

volatile int32_t step = 0;
volatile uint8_t dir = 1;     // dir=1:up, dir=0:down

uint32_t pw_direction(int32_t step){
    uint32_t pw = 0;    // us
    
    pw = MIN_US + ((MAX_US - MIN_US) * step) / MAX_STEP ;

    return pw;
}


// Initialization
void setup(void){
    RCC_PLL_init();     // CLOCK PLL 84MHz
    SysTick_init();     // SysTick 1msec

    // button-pin
    GPIO_init(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, PULL_UP);

    // PWM of 20 msec:  TIM2_CH2 (PA_1) 
    PWM_init(PWM_RC, 20);          // 20 msec period -> 50 Hz 
    PWM_pulsewidth_us(PWM_RC, 500);    // 0.5 msec

    // TIM3 Update-Event Interrupt every 500 msec
    TIM_UI_init_ms(TIM3, 500);           // UEV every 500 msec

    // Initialize EXTI PC_13
    EXTI_init(BUTTON_PIN, FALL, 10);	

}


int main(void) {  // ----------------- main --------------------
    // Initialization 
    setup();

    while(1){}

} // -------------- end main ------------------


// TIM3 Update Interrupt Handler - increase by 10°
void TIM3_IRQHandler(void){

    if(is_UIF(TIM3)){           // Check UIF(update interrupt flag)

        if(dir == 1){       // dir: up
            PWM_pulsewidth_us(PWM_RC, pw_direction(step));
            step++;
            if(step > MAX_STEP){        // update dir, step if it goes out of boundary
                dir = 0;
                step = MAX_STEP - 1;
            }
        }
        else if (dir == 0){     // dir: down
            PWM_pulsewidth_us(PWM_RC, pw_direction(step));
            step--;
            if(step < 0){
                dir = 1;
                step = 1;
            }
        }
        clear_UIF(TIM3);        // Clear TIM_UI flag
    }
}

/* -------------------------------------------------
cf)
500us = 0° (min), 2500us = 180° (max) ---- datasheet
move by 10° -> 180[°] / (10[°/step]) = 18 step
pw : how long should you move every time for 10°?
pw = 500 + (2000*step)/18
----------------------------------------------------*/

void EXTI15_10_IRQHandler(void) {

	if (is_pending_EXTI(BUTTON_PIN)) {   // EXTI_BUTTON_PIN : reset the servo motor to 0°
			step = 0 ;

            PWM_pulsewidth_us(PWM_RC, pw_direction(step));
            dir = 1;
            clear_pending_EXTI(BUTTON_PIN);
	}
}