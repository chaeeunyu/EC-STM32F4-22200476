/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : 유채은
Created          : 05-03-2021
Modified         : Oct-20-2026
Language/ver     : C++ in VS Code

Description      : USART1 - bluetooth
                   turning on&off LED by entering "H"&"L"
                   rotating stepper motor by pressing ("J":CW, "K":CCW, "P": stop)
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

#define 	LED2  		PA_5
#define 	END_CHAR	'\r'

static uint8_t   newline = '\n';
PinName_t pin_GPIO_TX2 = PA_2;
PinName_t pin_GPIO_RX2 = PA_3;
PinName_t pin_GPIO_TX1 = PA_9;
PinName_t pin_GPIO_RX1 = PA_10;
PinName_t pinStepper[4] = {PB_10, PB_4, PB_5, PB_3};  // ( A, B,  AN,  BN)

static uint8_t BT_Data = 0;
static volatile int cw_flag = 0;
static volatile int ccw_flag = 0;



void setup(void){	
    RCC_PLL_init();     // initialize clock
	SysTick_init();

    // USART2: USB serial init
	USART_init(USART2, pin_GPIO_TX2, pin_GPIO_RX2, BAUD_9600);  
	
	// USART1: MCU2 serial init 
	USART_init(USART1, pin_GPIO_TX1, pin_GPIO_RX1, BAUD_9600); 

    // initialize LED
    GPIO_init(LED2, OUTPUT);
    // initialize stepper motor
    stepper_init(FULL, pinStepper); 
    stepper_speed(10);   // motor speed = 10rpm
}


int main(void){	
	setup();
    //printf("MCU Initialized\r\n");

    uint32_t rev = 2;
	uint32_t steps = FULLSTEP_PER_REV*rev;	

	while(1){

        if(cw_flag == 1){
            stepper_step(steps, DIR_CW);
            cw_flag = 0;
        }
        else if(ccw_flag == 1){
            stepper_step(steps, DIR_CCW);
            ccw_flag = 0;
        }
	}
}

// ------------------------ USART Interrupt Handler ------------------------------

void USART1_IRQHandler(){          	// USART1 Interrupt - when receiving string from the other
	if(is_USART1_RXNE()){
		BT_Data = USART1_read();		// RX from other MCU UART1

        USART1_write(&BT_Data,1);		// TX to USART1	 (my PC)	 Echo of keyboard typing
        if(BT_Data == END_CHAR){
            USART1_write(&newline, 1);      // not for the prev line to be overwritten
        }

        // LED condition
        if(BT_Data == 'L'){
            GPIO_write(LED2, LOW);
        }
        else if(BT_Data == 'H'){
            GPIO_write(LED2, HIGH);
        }

        // motor condition
        if(BT_Data == 'J'){
            cw_flag = 1;
        }
        else if(BT_Data == 'K'){
            ccw_flag = 1;
        }
        else if(BT_Data == 'P'){
            stepper_stop();
            cw_flag = 0;
            ccw_flag = 0;
        }
	}
}
