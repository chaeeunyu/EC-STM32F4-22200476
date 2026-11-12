/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 10-08-2025
Modified         : 10-15-2026 
Language/ver     : C in VS Code

Description      : USART2  library  USART2.h
/----------------------------------------------------------------*/


/**
 * @file    ecUART2.h
 * @brief   USART library for STM32F411: initialization, baud rate, read/write, and printf retarget to USART2.
 *
 * Embedded Controller by Young-Keun Kim - Handong Global University
 *
 * Pin map:
 * - USART2 (USB virtual COM): PA_2 = TX, PA_3 = RX, APB1
 * - USART1: PA_9 = TX (or PB_6), PA_10 = RX (or PB_3), APB2
 * - USART6: PA_11 = TX (or PC_6), PA_12 = RX (or PC_7), APB2
 *
 * @author  Yu Chaeeun
 * @date    Created: 10-08-2026, Modified: 10-08-2026
 * @note    Language: C (VS Code, PlatformIO)
 */


#ifndef __EC_USART2_STUDENT_H
#define __EC_USART2_STUDENT_H

#include <stdio.h>
#include <math.h>
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"

/**
 * @defgroup USART USART Driver
 * @brief    Configure USART1/2/6 (8-N-1, oversampling x16) and send/receive bytes.
 *           printf() output is redirected to USART2.
 * @{
 */


#define POL 0	 /**< Polling mode */
#define INT 1	 /**< Interrupt mode */

// You can modify this
#define BAUD_9600	9600	/**< 9600 bps */
#define BAUD_19200	19200  	/**< 19200 bps */
#define BAUD_38400  38400   /**< 38400 bps */
#define BAUD_57600	57600   /**< 57600 bps */
#define BAUD_115200 115200   /**< 115200 bps */
#define BAUD_921600 921600   /**< 921600 bps */


// ********************** USART 2 (USB) ***************************
	// PA_2 = USART2_TX
	// PA_3 = USART2_RX
	// Alternate function(AF7), High Speed, Push pull, Pull up
	// APB1
// **********************************************************

// ********************** USART 1 ***************************
	// PA_9 = USART1_TX (default)	// PB_6  (option)
	// PA_10 = USART1_RX (default)	// PB_3 (option)
	// APB2
// **********************************************************

// ********************** USART 6 ***************************
	// PA_11 = USART6_TX (default)	// PC_6  (option)
	// PA_12 = USART6_RX (default)	// PC_7 (option)
	// APB2
// **********************************************************


/*================ EXERCISE ================*/
/*================ General Configuration ================*/
/**
 * @brief  Initialize a USART with given TX/RX pins and baud rate, and enable its RX interrupt.
 *
 * Frame format: 8 data bits, no parity, 1 stop bit, oversampling x16.
 * @param[in] USARTx       USART1, USART2, or USART6
 * @param[in] pin_GPIO_TX  TX pin (e.g., PA_2)
 * @param[in] pin_GPIO_RX  RX pin (e.g., PA_3)
 * @param[in] baud         Baud rate [bps] (e.g., BAUD_9600)
 * @note   Call after RCC_PLL_init() or RCC_HSI_init(), since the baud rate is computed from EC_SYSCLK.
 * @par Example
 * @code
 * RCC_PLL_init();
 * USART_init(USART2, PA_2, PA_3, BAUD_9600);     // PC (USB virtual COM)
 * USART_init(USART1, PA_9, PA_10, BAUD_9600);    // e.g., Bluetooth module
 * @endcode
 */
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud);	// copy of USART_setting

/**
 * @brief  Set the baud rate of a USART (BRR register).
 * @param[in] USARTx  USART1, USART2, or USART6
 * @param[in] baud    Baud rate [bps]
 * @note   Uses APB1 (EC_SYSCLK / 2) for USART2 and APB2 (EC_SYSCLK) for USART1/6.
 * @par Example
 * @code
 * USART_baud(USART2, BAUD_115200);
 * @endcode
 */
void USART_baud(USART_TypeDef* USARTx, uint32_t baud);

/**
 * @brief  Configure TX/RX pins for a USART: alternate function, push-pull, no pull-up/down, high speed.
 * @param[in] USARTx       USART1, USART2, or USART6 (AF7 for USART1/2, AF8 for USART6)
 * @param[in] pin_GPIO_TX  TX pin
 * @param[in] pin_GPIO_RX  RX pin
 * @par Example
 * @code
 * USART_pins(USART6, PA_11, PA_12);   // TX = PA_11, RX = PA_12, AF8
 * @endcode
 */
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX);


/**
 * @brief  Set the baud rate of USART1.
 * @param[in] baud  Baud rate [bps]
 * @par Example
 * @code
 * USART1_init();               // 9600 bps by default
 * USART1_baud(BAUD_38400);
 * @endcode
 */
void USART1_baud(uint32_t baud);

/**
 * @brief  Set the baud rate of USART2.
 * @param[in] baud  Baud rate [bps]
 * @par Example
 * @code
 * USART2_init();               // 9600 bps by default
 * USART2_baud(BAUD_115200);
 * @endcode
 */
void USART2_baud(uint32_t baud);


/*================ PROVIDED  ================*/
/*================ Configuration for each USART ================*/
// Configuration UART 1, 2 using default settings

/**
 * @brief  Initialize USART1 with default settings: PA_9 (TX), PA_10 (RX), 9600 bps, RX interrupt enabled.
 * @par Example
 * @code
 * RCC_PLL_init();
 * USART1_init();               // PA_9 = TX, PA_10 = RX, 9600 bps
 * @endcode
 */
void USART1_init(void);

/**
 * @brief  Initialize USART2 with default settings: PA_2 (TX), PA_3 (RX), 9600 bps, RX interrupt enabled.
 * @note   USART2 is connected to the PC through the Nucleo USB (virtual COM port).
 * @par Example
 * @code
 * RCC_PLL_init();
 * USART2_init();               // PA_2 = TX, PA_3 = RX, 9600 bps
 * printf("Hello Nucleo\r\n");  // printf() is sent to USART2
 * @endcode
 */
void USART2_init(void);


// USART write & read
/**
 * @brief  Send bytes through USART1 (blocking).
 * @param[in] buffer  Data to send
 * @param[in] nBytes  Number of bytes
 * @par Example
 * @code
 * uint8_t msg[] = "AT\r\n";
 * USART1_write(msg, sizeof(msg) - 1);
 * @endcode
 */
void USART1_write(uint8_t* buffer, uint32_t nBytes);

/**
 * @brief  Send bytes through USART2 (blocking).
 * @param[in] buffer  Data to send
 * @param[in] nBytes  Number of bytes
 * @par Example
 * @code
 * uint8_t msg[] = "Hello\r\n";
 * USART2_write(msg, sizeof(msg) - 1);
 * @endcode
 */
void USART2_write(uint8_t* buffer, uint32_t nBytes);

/**
 * @brief  Receive one byte from USART1 (blocking until data arrives).
 * @return Received byte
 * @par Example
 * @code
 * void USART1_IRQHandler(void) {
 *     if (is_USART1_RXNE()) {
 *         uint8_t c = USART1_read();
 *         USART2_write(&c, 1);         // forward to PC
 *     }
 * }
 * @endcode
 */
uint8_t USART1_read(void);			

/**
 * @brief  Receive one byte from USART2 (blocking until data arrives).
 * @return Received byte
 * @par Example
 * @code
 * void USART2_IRQHandler(void) {
 *     if (is_USART2_RXNE()) {
 *         uint8_t c = USART2_read();
 *         USART2_write(&c, 1);         // echo back to PC
 *     }
 * }
 * @endcode
 */
uint8_t USART2_read(void);	


// RX Inturrupt Flag USART1,2
/**
 * @brief  Check whether USART1 has received data (RXNE flag).
 * @return 1 if data is ready to read, 0 otherwise
 * @par Example
 * @code
 * void USART1_IRQHandler(void) {
 *     if (is_USART1_RXNE()) {          // data received?
 *         uint8_t c = USART1_read();
 *         USART2_write(&c, 1);
 *     }
 * }
 * @endcode
 */
uint32_t is_USART1_RXNE(void);

/**
 * @brief  Check whether USART2 has received data (RXNE flag).
 * @return 1 if data is ready to read, 0 otherwise
 * @par Example
 * @code
 * void USART2_IRQHandler(void) {
 *     if (is_USART2_RXNE()) {          // data received?
 *         uint8_t cmd = USART2_read();
 *         if (cmd == 'L') GPIO_write(PA_5, HIGH);
 *     }
 * }
 * @endcode
 */
uint32_t is_USART2_RXNE(void);


/*================ General Configuration ================*/
/**
 * @brief  Enable the peripheral clock of a USART.
 * @param[in] USARTx  USART1 / USART6 (APB2) or USART2 (APB1)
 * @par Example
 * @code
 * USART_clock_enable(USART6);      // APB2
 * @endcode
 */
void USART_clock_enable(USART_TypeDef* USARTx);

/**
 * @brief  Send bytes through a USART (blocking). Waits for TXE before each byte and TC at the end.
 * @param[in] USARTx  USART1, USART2, or USART6
 * @param[in] buffer  Data to send
 * @param[in] nBytes  Number of bytes
 * @par Example
 * @code
 * uint8_t msg[] = "Hello\r\n";
 * USART_write(USART6, msg, sizeof(msg) - 1);
 * @endcode
 */
void USART_write(USART_TypeDef* USARTx, uint8_t* buffer, uint32_t nBytes);

/**
 * @brief  Receive one byte from a USART (blocking until RXNE is set).
 * @param[in] USARTx  USART1, USART2, or USART6
 * @return Received byte
 * @par Example
 * @code
 * void USART6_IRQHandler(void) {
 *     if (is_USART_RXNE(USART6)) {
 *         uint8_t c = USART_read(USART6);
 *         USART_write(USART2, &c, 1);   // forward to PC
 *     }
 * }
 * @endcode
 */
uint8_t USART_read(USART_TypeDef * USARTx);	

/**
 * @brief  Check whether a USART has received data (RXNE flag).
 * @param[in] USARTx  USART1, USART2, or USART6
 * @return 1 if data is ready to read, 0 otherwise
 * @par Example
 * @code
 * void USART6_IRQHandler(void) {
 *     if (is_USART_RXNE(USART6)) {     // data received?
 *         uint8_t c = USART_read(USART6);
 *         USART_write(USART2, &c, 1);
 *     }
 * }
 * @endcode
 */
uint32_t is_USART_RXNE(USART_TypeDef * USARTx);

/**
 * @brief  Approximate busy-wait delay used between transmitted bytes.
 * @param[in] us  Delay time [us] (approximate, not calibrated)
 * @par Example
 * @code
 * USART_delay(300);                // about 300 us (not calibrated)
 * @endcode
 */
void USART_delay(uint32_t us);  


/** @} */
								

#endif // __EC_USART2_STUDENT_H



