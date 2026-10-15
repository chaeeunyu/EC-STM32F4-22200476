/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-15-2026
Language/ver     : C++ in VS Code

Description      : SysTick Library. ecSysTick.h
/----------------------------------------------------------------*/

/**
 * @file    ecSysTick2.h
 * @brief   SysTick timer library: 1 ms system tick and blocking millisecond delay.
 *
 * Embedded Controller by Young-Keun Kim - Handong Global University
 *
 * @author  Yu Chaeeun
 * @date    Created: 05-03-2021, Modified: Sep-28-2026
 * @note    Language: C (VS Code, PlatformIO)
 */


#ifndef __EC_SYSTICK2_H
#define __EC_SYSTICK2_H

#include "stm32f4xx.h"
#include "ecRCC2.h"
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


/**
 * @defgroup SysTick SysTick Timer
 * @brief    Generate a 1 ms tick with the SysTick timer and provide millisecond delays.
 * @{
 */


 /** Millisecond counter. Incremented by 1 every 1 ms in SysTick_Handler(). */
extern volatile uint32_t msTicks;	


/**
 * @brief  Configure SysTick to interrupt every 1 ms and enable it (NVIC priority 15).
 * @note   Call after RCC_HSI_init() or RCC_PLL_init(), because the reload value
 *         is computed from EC_SYSCLK.
 */
void SysTick_init(void);

/**
 * @brief  SysTick interrupt handler. Called by hardware every 1 ms; increments msTicks.
 * @note   Do not call this function directly.
 */
void SysTick_Handler(void);

/**
 * @brief  Increment msTicks by 1.
 * @note   Called from SysTick_Handler(). Not intended for direct use.
 */
void SysTick_counter(void);

/**
 * @brief  Blocking delay in milliseconds, based on msTicks.
 * @param[in] msec  Delay time [ms]
 * @note   SysTick_init() must be called first.
 * @warning Do not call inside an ISR whose priority is equal to or higher than SysTick
 *          (priority 15). msTicks will not increase and the program will hang.
 */
void delay_ms(uint32_t msec);

/**
 * @brief  Reset the SysTick current value register (VAL) to 0.
 * @note   This does not reset msTicks.
 */
void SysTick_reset(void);

/**
 * @brief  Read the SysTick current value register (VAL).
 * @return Current counter value. It counts down from the reload value to 0 every 1 ms.
 */
uint32_t SysTick_val(void);


/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_SYSTICK2_H
