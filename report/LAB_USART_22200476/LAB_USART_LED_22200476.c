/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-19-2026
Language/ver     : C++ in VS Code

Description      : USART1, 2 IRQ Handler
                 : Toggling LED by pressing "L(off)" and "H(on)"
/----------------------------------------------------------------*/
#include <string.h>
#include "ecSTM32F4v2.h"

#define 	LED2  		PA_5
#define 	END_CHAR	'\r'  // CR: Carriage Return('\r') =/= LF: Line Feed('\n')
							 	// you should do both for the cursor to move
						 		// to "the beginning(\r)" of "the next line(\n)" 
#define 	MAX_BUF		15


// ---------------------------- Pin Configuration ----------------------------
// USART2: ST-Link Virtual COM Port (connected to PC via USB)
PinName_t pin_GPIO_TX2 = PA_2;
PinName_t pin_GPIO_RX2 = PA_3;
// USART1: connected to the other MCU (or Bluetooth module)
PinName_t pin_GPIO_TX1 = PA_9;
PinName_t pin_GPIO_RX1 = PA_10;

// ---------------------------- Global Variables ----------------------------
// [PC (USART2) side]
static volatile uint8_t PC_buffer[MAX_BUF]={0, };  // Temporary buffer for chars being received
static volatile uint8_t BT_buffer[MAX_BUF]={0, };  
static volatile uint8_t PC_string[MAX_BUF + 1]={0, };  // Completed string (+1 for NULL terminator '\0')
static volatile uint8_t PC_Data = 0;    // Most recently received byte
static volatile int idx = 0;        // Current write index of PC_buffer
static volatile int bReceive = 0;    // Flag: PC string complete -> print in main
static volatile int err_pc = 0;     // Flag: string exceeded MAX_BUF -> print error in main
static volatile int ovf_pc = 0;     // Overflow state of current string (kept until END_CHAR)

static uint8_t newline = '\n';

// [Other MCU (USART1) side] - same structure as PC side
static volatile uint8_t BT_Data = 0;
static volatile uint8_t BT_string[MAX_BUF + 1]={0, };
static volatile int BT_idx = 0;
static volatile int bBTReceive = 0;
static volatile int err_bt = 0;
static volatile int ovf_bt = 0;


// ---------------------------- Initialization ----------------------------
void setup(void){
	RCC_PLL_init();     // Set system clock to PLL (84 MHz)
	SysTick_init();     // Initialize SysTick timer (for delay)
	
	// USART2: USB serial init
	USART_init(USART2, pin_GPIO_TX2, pin_GPIO_RX2, BAUD_9600);  
	
	// USART1: MCU2 serial init 
	USART_init(USART1, pin_GPIO_TX1, pin_GPIO_RX1, BAUD_9600); 

    // initialize LED
    GPIO_init(LED2, OUTPUT);
}

// ---------------------------- Main ----------------------------
// ISR only sets flags; time-consuming printf is handled in the main loop
//  keep ISRs short!
int main(void){	
	setup();
	printf("MCU Initialized\r\n");	
	while(1){
		// USART Receive: Use Interrupt only
		// USART Transmit: Polling
		//USART2_write(PC_string, 7);    
        
        // Print when a string from PC is complete
		if (bReceive == 1){
            printf("PC_string: %s\r\n", PC_string);
            bReceive = 0;       // clear flag
		}
        // Print when a string from the other MCU is complete
		if (bBTReceive == 1){
            printf("BT_string: %s\r\n", BT_string);
            bBTReceive = 0;     // clear flag
		}

        // Handle string-too-long errors
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

// USART2 RX interrupt: called every time one char is typed on the PC keyboard
void USART2_IRQHandler(){       // USART2 Interrupt - when sending string
	if(is_USART2_RXNE()){   // RXNE (RX Not Empty): data available in receive data register
		PC_Data = USART2_read();		// RX from UART2 (my PC)
		
        // 1) Echo: send the char back so it appears on the PC terminal
        USART2_write(&PC_Data,1);		// TX to USART2	 (my PC)	 Echo of keyboard typing
        if(PC_Data == END_CHAR){
            USART2_write(&newline, 1);      // '\r' alone returns to line start and overwrites -> add '\n'
        }

         // 2) Forward: pass the received char to the other MCU as-is
        USART1_write(&PC_Data,1);	  // TX to Other MCU

		// 3) Assemble received chars into a string
        if(PC_Data == END_CHAR){        // [Case A] Enter pressed -> end of string
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
        else if (idx < MAX_BUF) {       // [Case B] Normal char & space left -> store in buffer
            PC_buffer[idx] = PC_Data;
            idx++;            
        }
        else{ ovf_pc = 1; }             // [Case C] Buffer full -> mark overflow, drop further chars
	}
}

// USART1 RX interrupt: called every time one char is received from the other MCU
void USART1_IRQHandler(){          	// USART1 Interrupt - when receiving string from the other
	if(is_USART1_RXNE()){
		BT_Data = USART1_read();		// RX from other MCU UART1
        
        // Ignore NULL and LF (not included in the string)
        if(BT_Data == '\0' || BT_Data == '\n') return;

        // 1) LED control command (reacts immediately, per char)
        if(BT_Data == 'L'){
            GPIO_write(LED2, LOW);      // 'L' -> LED OFF
        }
        else if(BT_Data == 'H'){
            GPIO_write(LED2, HIGH);     // 'H' -> LED ON
        }
		
		// 2) String assembly - same logic as USART2 handler
        if(BT_Data == END_CHAR){        // [Case A] End of string
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
        else if (BT_idx < MAX_BUF) {        // [Case B] Store in buffer
            BT_buffer[BT_idx] = BT_Data;
            BT_idx++;            
        }
        else{ ovf_bt = 1; }      // [Case C] Overflow
	}
}