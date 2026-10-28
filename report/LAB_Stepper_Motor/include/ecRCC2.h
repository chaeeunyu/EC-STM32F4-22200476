/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Sep-14-2026
Language/ver     : C++ in VS Code

Description      : System Clock Library. Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/

/**
 * @file    ecRCC2.h
 * @brief   System clock (RCC) library for STM32F411: HSI/PLL clock setup and GPIO port clock enable.
 *
 * Embedded Controller by Young-Keun Kim - Handong Global University
 *
 * @author  Yu Chaeeun
 * @date    Created: 05-03-2021, Modified: Sep-14-2026
 * @note    Language: C (VS Code, PlatformIO)
 */

#ifndef __EC_RCC2_H
#define __EC_RCC2_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecPinNames.h"

/**
 * @defgroup RCC RCC (Clock) Driver
 * @brief    Select the system clock (HSI 16 MHz or PLL 84 MHz) and enable GPIO port clocks.
 * @{
 */


 /** Current system clock frequency [Hz]. Updated by RCC_HSI_init() and RCC_PLL_init(). */
extern volatile int EC_SYSCLK;		// current SYSCLK [Hz], set by RCC_HSI_init()/RCC_PLL_init()

/**
 * @brief  Approximate blocking delay using a busy-wait loop.
 * @param[in] ms  Delay time [ms]
 * @note   The loop count is estimated from EC_SYSCLK, so the timing is not exact.
 *         Use delay_ms() (SysTick) when accurate timing is needed.
 * @par Example
 * @code
 * RCC_HSI_init();
 * GPIO_init(PA_5, OUTPUT);
 * while (1) {
 *     GPIO_write(PA_5, HIGH);
 *     delay_ms_HSI(500);           // about 500 ms
 *     GPIO_write(PA_5, LOW);
 *     delay_ms_HSI(500);
 * }
 * @endcode
 */
void delay_ms_HSI(uint32_t ms); 

/**
 * @brief  Use the internal HSI oscillator (16 MHz) as the system clock.
 * @note   Sets EC_SYSCLK to 16000000.
 * @par Example
 * @code
 * RCC_HSI_init();      // SYSCLK = 16 MHz, EC_SYSCLK = 16000000
 * SysTick_init();      // 1 ms tick computed from 16 MHz
 * @endcode
 */
void RCC_HSI_init(void);

/**
 * @brief  Use the PLL (from HSI) as the system clock at 84 MHz.
 *
 * PLL: 16 MHz x 84 / 8 / 2 = 84 MHz. Flash latency = 2 wait states.
 * Bus clocks: AHB = 84 MHz, APB1 = 42 MHz, APB2 = 84 MHz.
 * @note   Sets EC_SYSCLK to 84000000. Call before SysTick_init() and USART setup,
 *         since they compute their timing from EC_SYSCLK.
 * @par Example
 * @code
 * RCC_PLL_init();      // SYSCLK = 84 MHz, EC_SYSCLK = 84000000
 * SysTick_init();      // 1 ms tick computed from 84 MHz
 * USART2_init();       // baud rate computed from APB1 = 42 MHz
 * @endcode
 */
void RCC_PLL_init(void);

/**
 * @brief Enable the AHB1 clock of GPIO port A.
 * @par Example
 * @code
 * RCC_GPIOA_enable();                   // required before accessing GPIOA registers
 * GPIOA->MODER &= ~(3UL << (2*5));
 * GPIOA->MODER |=  (1UL << (2*5));      // PA_5 = OUTPUT
 * // GPIO_init() calls this automatically for port A pins.
 * @endcode
 */
void RCC_GPIOA_enable(void);

/**
 * @brief Enable the AHB1 clock of GPIO port B.
 * @par Example
 * @code
 * RCC_GPIOB_enable();                   // required before accessing GPIOB registers
 * GPIOB->MODER &= ~(3UL << (2*10));     // PB_10 = INPUT
 * // GPIO_init() calls this automatically for port B pins.
 * @endcode
 */
void RCC_GPIOB_enable(void);

/**
 * @brief Enable the AHB1 clock of GPIO port C.
 * @par Example
 * @code
 * RCC_GPIOC_enable();                   // required before accessing GPIOC registers
 * GPIOC->MODER &= ~(3UL << (2*13));     // PC_13 (user button) = INPUT
 * // GPIO_init() calls this automatically for port C pins.
 * @endcode
 */
void RCC_GPIOC_enable(void);

/**
 * @brief Enable the AHB1 clock of GPIO port D.
 * @par Example
 * @code
 * RCC_GPIOD_enable();                   // required before accessing GPIOD registers
 * GPIOD->MODER &= ~(3UL << (2*2));
 * GPIOD->MODER |=  (1UL << (2*2));      // PD_2 = OUTPUT
 * // GPIO_init() calls this automatically for port D pins.
 * @endcode
 */
void RCC_GPIOD_enable(void);

/**
 * @brief Enable the AHB1 clock of GPIO port E.
 * @par Example
 * @code
 * RCC_GPIOE_enable();                   // required before accessing GPIOE registers
 * // Port E pins are not routed on the Nucleo-F411RE headers.
 * @endcode
 */
void RCC_GPIOE_enable(void);
// void RCC_GPIO_enable(GPIO_TypeDef * GPIOx);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_RCC2_H
