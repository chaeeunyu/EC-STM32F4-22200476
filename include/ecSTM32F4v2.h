//ecSTM32F4v2.h

/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-01-2026
Language/ver     : C++ in VS Code

Description      : ecSTM32F4v2.h
/----------------------------------------------------------------*/

#ifndef __EC_STM_H
#define __EC_STM_H

// STM built-In Library
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "math.h"

// EC course Library
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
//#include "ecTIM2.h"
//#include "ecPWM2.h"
//#include "ecStepper2.h"
//#include "ecADC2.h"
//#include "ecUART2.h"


void MCU_init(void){

    RCC_PLL_init();    // CLOCK PLL 84MHz
    SysTick_init();    // SysTick 1msec
    
    // Button PC13
    // GPIO_init(PC_13, INPUT);
    // GPIO_pupd(PC_13, PULL_DOWN);    
    
    // // LED PA5
    // GPIO_init(PA_5, OUTPUT);    
    // jk init을 필요에 따라 만들어도됨

    // TIMx Default Initialization
    // ...
    // PWM Default Initialization
    // ...        
    // USART Default Initialization
    // ...
    // Others
}

#endif



 