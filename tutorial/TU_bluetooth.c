#include "ecSTM32F4v2.h"
// this should include #include "ecUART2.h" 
#include "ecUART2.h" 

static volatile uint8_t PC_Data = 0;
static volatile uint8_t BT_Data = 0;
uint8_t PC_string[]="Hi_BT\r\n";

void setup(void){
	RCC_PLL_init();
	SysTick_init();
	
	// USART1: BT serial init 
	USART1_init();
	USART1_baud(BAUD_9600);

    GPIO_init(PA_5, OUTPUT);   // Nucleo user LED (LD2)
}

int main(void){	
	setup();
    uint32_t led = 0;
	//printf("MCU Initialized\r\n");	
	while(1){
		// USART Receive: Use Interrupt only
		// USART Transmit:  Interrupt or Polling
		USART1_write(PC_string, 7);
        
        led ^= 1;                  // 0 ↔ 1 toggle
		GPIO_write(PA_5, led);

		delay_ms(2000);        
	}
}
