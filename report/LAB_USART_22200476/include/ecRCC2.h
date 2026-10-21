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
 */
void delay_ms_HSI(uint32_t ms); 

/**
 * @brief  Use the internal HSI oscillator (16 MHz) as the system clock.
 * @note   Sets EC_SYSCLK to 16000000.
 */
void RCC_HSI_init(void);

/**
 * @brief  Use the PLL (from HSI) as the system clock at 84 MHz.
 *
 * PLL: 16 MHz x 84 / 8 / 2 = 84 MHz. Flash latency = 2 wait states.
 * Bus clocks: AHB = 84 MHz, APB1 = 42 MHz, APB2 = 84 MHz.
 * @note   Sets EC_SYSCLK to 84000000. Call before SysTick_init() and USART setup,
 *         since they compute their timing from EC_SYSCLK.
 */
void RCC_PLL_init(void);

/** @brief Enable the AHB1 clock of GPIO port A. */
void RCC_GPIOA_enable(void);

/** @brief Enable the AHB1 clock of GPIO port B. */
void RCC_GPIOB_enable(void);

/** @brief Enable the AHB1 clock of GPIO port C. */
void RCC_GPIOC_enable(void);

/** @brief Enable the AHB1 clock of GPIO port D. */
void RCC_GPIOD_enable(void);

/** @brief Enable the AHB1 clock of GPIO port E. */
void RCC_GPIOE_enable(void);
// void RCC_GPIO_enable(GPIO_TypeDef * GPIOx);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_RCC2_H
