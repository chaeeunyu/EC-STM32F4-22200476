/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : 유채은
Created          : 05-03-2021
Modified         : Oct-19-2026
Language/ver     : C++ in VS Code

Description      : USART1, 2 IRQ Handler
                 : Toggling LED by pressing "L(off)" and "H(on)"
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

#define 	LED2  		PA_5
#define 	END_CHAR	'\r'  // CR: Carriage Return('\r') =/= LF: Line Feed('\n')
							 	// you should do both for the cursor to move
						 		// to "the beginning(\r)" of "the next line(\n)" 
#define 	MAX_BUF		15


PinName_t pin_GPIO_TX2 = PA_2;
PinName_t pin_GPIO_RX2 = PA_3;
PinName_t pin_GPIO_TX1 = PA_9;
PinName_t pin_GPIO_RX1 = PA_10;

static volatile uint8_t PC_buffer[MAX_BUF]={0, };
static volatile uint8_t BT_buffer[MAX_BUF]={0, };
static volatile uint8_t PC_string[MAX_BUF + 1]={0, };
static volatile uint8_t PC_Data = 0;
static uint8_t newline = '\n';
static volatile uint8_t BT_Data = 0;
static volatile uint8_t BT_string[MAX_BUF + 1]={0, };
static volatile int idx = 0;
static volatile int BT_idx = 0;
static volatile int bReceive = 0;
static volatile int bBTReceive = 0;
static volatile int err_pc = 0;
static volatile int err_bt = 0;
static volatile int ovf_pc = 0;
static volatile int ovf_bt = 0;


void setup(void){
	RCC_PLL_init();
	SysTick_init();
	
	// USART2: USB serial init
	USART_init(USART2, pin_GPIO_TX2, pin_GPIO_RX2, BAUD_9600);  
	
	// USART1: MCU2 serial init 
	USART_init(USART1, pin_GPIO_TX1, pin_GPIO_RX1, BAUD_9600); 

    // initialize LED
    GPIO_init(LED2, OUTPUT);
}

int main(void){	
	setup();
	printf("MCU Initialized\r\n");	
	while(1){
		// USART Receive: Use Interrupt only
		// USART Transmit: Polling
		//USART2_write(PC_string, 7);       
		if (bReceive == 1){
            printf("PC_string: %s\r\n", PC_string);
            bReceive = 0;
		}
		if (bBTReceive == 1){
            printf("BT_string: %s\r\n", BT_string);
            bBTReceive = 0;
		}

        if(err_pc == 1){
            printf("ERROR : Too long string\r\n");
            err_pc = 0;
        }
        if(err_bt == 1){
            printf("ERROR : Too long string\r\n");
            err_bt = 0;
        }
	}
}


// ------------------------ USART Interrupt Handler ------------------------------

void USART2_IRQHandler(){       // USART2 Interrupt - when sending string
	if(is_USART2_RXNE()){
		PC_Data = USART2_read();		// RX from UART2 (my PC)
		USART2_write(&PC_Data,1);		// TX to USART2	 (my PC)	 Echo of keyboard typing
        if(PC_Data == END_CHAR){
            USART2_write(&newline, 1);      // not for the prev line to be overwritten
        }
		USART1_write(&PC_Data,1);	  // TX to Other MCU

		// Creates a String from received serial character
        if(PC_Data == END_CHAR){
            if(ovf_pc){
                err_pc = 1;
            }
            else{
                bReceive = 1;
                // reset PC_string
                memset(PC_string, 0, sizeof(char) * MAX_BUF);
                // copy to PC_string
                memcpy(PC_string, PC_buffer, sizeof(char) * idx);
            }
            // reset PC_buffer
            memset(PC_buffer, 0, sizeof(char) * MAX_BUF);
            idx = 0;
            ovf_pc = 0;
        }
        else if (idx < MAX_BUF) {
            PC_buffer[idx] = PC_Data;
            idx++;            
        }
        else{ ovf_pc = 1; }     // if(idx >= MAX_BUF)
	}
}

void USART1_IRQHandler(){          	// USART1 Interrupt - when receiving string from the other
	if(is_USART1_RXNE()){
		BT_Data = USART1_read();		// RX from other MCU UART1
        if(BT_Data == '\0' || BT_Data == '\n') return;


        if(BT_Data == 'L'){
            GPIO_write(LED2, LOW);
        }
        else if(BT_Data == 'H'){
            GPIO_write(LED2, HIGH);
        }
		
		// Creates a String from received serial character
        if(BT_Data == END_CHAR){
            if(ovf_bt){
                err_bt = 1;
            }
            else{
                bBTReceive = 1;
                // reset BT_string
                memset(BT_string, 0, sizeof(char) * MAX_BUF);
                // copy to BT_string
                memcpy(BT_string, BT_buffer, sizeof(char) * BT_idx);
            }
            // reset BT_buffer
            memset(BT_buffer, 0, sizeof(char) * MAX_BUF);
            BT_idx = 0;
            ovf_bt = 0;
        }
        else if (BT_idx < MAX_BUF) {
            BT_buffer[BT_idx] = BT_Data;
            BT_idx++;            
        }
        else{ ovf_bt = 1; }     // if(idx >= MAX_BUF)
	}
}