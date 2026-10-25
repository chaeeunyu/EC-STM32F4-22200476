/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
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

// ---------------------------- Pin Configuration ----------------------------
// USART2: ST-Link Virtual COM Port (connected to PC via USB, used by printf)
PinName_t pin_GPIO_TX2 = PA_2;
PinName_t pin_GPIO_RX2 = PA_3;
// USART1: connected to Bluetooth module (MCU TX -> BT RX, MCU RX -> BT TX)
PinName_t pin_GPIO_TX1 = PA_9;
PinName_t pin_GPIO_RX1 = PA_10;
// Stepper motor driver input pins, in coil-phase order
PinName_t pinStepper[4] = {PB_10, PB_4, PB_5, PB_3};  // ( A, B,  AN,  BN)

// ---------------------------- Global Variables ----------------------------
static uint8_t BT_Data = 0;             // Most recently received byte (used only inside ISR)
static volatile int cw_flag = 0;        // Flag: CW rotation requested  (set in ISR, handled in main)
static volatile int ccw_flag = 0;       // Flag: CCW rotation requested (set in ISR, handled in main)


// ---------------------------- Initialization ----------------------------
void setup(void){	
    RCC_PLL_init();     // Set system clock to PLL (84 MHz)
	SysTick_init();     // Initialize SysTick timer (used for step timing / delay)

    // USART2: USB serial init (for printf debugging to PC)
	USART_init(USART2, pin_GPIO_TX2, pin_GPIO_RX2, BAUD_9600);  
	
	// USART1: Bluetooth module serial init
	// Baud rate must match the BT module setting
	USART_init(USART1, pin_GPIO_TX1, pin_GPIO_RX1, BAUD_9600); 

    // initialize LED
    GPIO_init(LED2, OUTPUT);
    // initialize stepper motor in full-step mode with the 4 phase pins
    stepper_init(FULL, pinStepper); 
    stepper_speed(10);   // motor speed = 10rpm
}

// ---------------------------- Main ----------------------------
int main(void){	
	setup();
    //printf("MCU Initialized\r\n");

    uint32_t rev = 2;                         // Number of revolutions per command
	uint32_t steps = FULLSTEP_PER_REV*rev;    // Total steps = (steps per revolution) x (revolutions)

    // ISR only sets flags; the (long) motor motion is executed in the main loop
	while(1){

        if(cw_flag == 1){
            stepper_step(steps, DIR_CW);        // Rotate CW (blocking until done or stopped)
            cw_flag = 0;                        // clear flag after motion
        }
        else if(ccw_flag == 1){
            stepper_step(steps, DIR_CCW);       // Rotate CCW (blocking until done or stopped)
            ccw_flag = 0;                       // clear flag after motion
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

        // LED command: applied immediately inside ISR (fast operation)
        if(BT_Data == 'L'){
            GPIO_write(LED2, LOW);          // 'L' -> LED OFF
        }
        else if(BT_Data == 'H'){
            GPIO_write(LED2, HIGH);         // 'H' -> LED ON
        }

        // Motor command: only set flags here, actual motion is done in main
        if(BT_Data == 'J'){
            cw_flag = 1;                    // 'J' -> request CW rotation
        }
        else if(BT_Data == 'K'){
            ccw_flag = 1;                   // 'K' -> request CCW rotation
        }
        else if(BT_Data == 'P'){
            stepper_stop();                 // 'P' -> stop motor immediately
            cw_flag = 0;                    // cancel any pending requests
            ccw_flag = 0; 
        }
	}
}
