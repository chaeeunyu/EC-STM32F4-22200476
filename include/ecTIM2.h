/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-29-2026
Language/ver     : C in VS Code

Description      : MCU timer interrupt .h
/----------------------------------------------------------------*/

/**
 * @file    ecTIM2.h
 * @brief   Timer counter and timer update interrupt driver for STM32F411.
 *
 * Embedded Controller by Young-Keun Kim - Handong Global University
 *
 * @author  Yu Chaeeun
 * @date    Created: 05-03-2021, Modified: Oct-29-2026
 * @note    Language: C (VS Code, PlatformIO)
 */

#ifndef __EC_TIM2_H
#define __EC_TIM2_H

#include "stm32f411xe.h"
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "math.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup TIM Timer Driver
 * @brief    Configure timer counters (PSC, ARR, direction), the update interrupt (UI),
 *           and the NVIC for TIM1~TIM5, TIM9~TIM11.
 * @{
 */


//////////////////////////////////////////////
/* Timer Update Interrupt Configuration     */
//////////////////////////////////////////////

/**
 * @brief  Initialize a timer with a period in msec and enable its update interrupt in NVIC.
 * @param[in] TIMx  Timer to use (TIM1~TIM5, TIM9~TIM11)
 * @param[in] msec  Update interrupt period [msec]. 16-bit timer: 1 ~ 6,553 msec
 * @note   Counter clock f_cnt = 10 kHz. NVIC priority is fixed at 2.
 * @note   Calls TIM_init_ms() → TIM_UI_enable() → NVIC enable.
 * @par Example
 * @code
 * volatile uint32_t count = 0;
 *
 * TIM_UI_init_ms(TIM3, 1);           // update interrupt every 1 msec
 *
 * void TIM3_IRQHandler(void) {
 *     if (is_UIF(TIM3)) {
 *         count++;
 *         clear_UIF(TIM3);
 *     }
 * }
 * @endcode
 */
void TIM_UI_init_ms(TIM_TypeDef* TIMx, uint32_t msec);     // 16-bit  msec <= 6,553 msec 

/**
 * @brief  Initialize a timer with a period in usec and enable its update interrupt in NVIC.
 * @param[in] TIMx  Timer to use (TIM1~TIM5, TIM9~TIM11)
 * @param[in] usec  Update interrupt period [usec]. 16-bit timer: 1 ~ 65,535 usec
 * @note   Counter clock f_cnt = 1 MHz. NVIC priority is fixed at 2.
 * @note   Calls TIM_init_us() → TIM_UI_enable() → NVIC enable.
 * @par Example
 * @code
 * TIM_UI_init_us(TIM2, 100);         // update interrupt every 100 usec (10 kHz)
 *
 * void TIM2_IRQHandler(void) {
 *     if (is_UIF(TIM2)) {
 *         // sampling task
 *         clear_UIF(TIM2);
 *     }
 * }
 * @endcode
 */
void TIM_UI_init_us(TIM_TypeDef* TIMx, uint32_t usec);  // 16-bit  usec < 65,530 usec 

/**
 * @brief  Enable the update interrupt (UIE) of a timer.
 * @param[in] TIMx  Timer to enable
 * @note   The UIF flag is cleared first, so a stale flag does not fire immediately.
 * @par Example
 * @code
 * TIM_UI_disable(TIM3);              // pause the periodic task
 * // ...
 * TIM_UI_enable(TIM3);               // resume
 * @endcode
 */
void TIM_UI_enable(TIM_TypeDef* TIMx);

/**
 * @brief  Disable the update interrupt (UIE) of a timer. The counter keeps running.
 * @param[in] TIMx  Timer to disable
 * @par Example
 * @code
 * TIM_UI_disable(TIM3);              // stop the periodic interrupt
 * @endcode
 */
void TIM_UI_disable(TIM_TypeDef* TIMx);

/**
 * @brief  Check the update interrupt flag (UIF) of a timer.
 * @param[in] TIMx  Timer to check
 * @return 1 if an update event (overflow/underflow) occurred, 0 otherwise
 * @note   Use inside the timer IRQ handler.
 * @par Example
 * @code
 * void TIM3_IRQHandler(void) {
 *     if (is_UIF(TIM3)) {            // update event?
 *         GPIO_write(PA_5, !GPIO_read(PA_5));
 *         clear_UIF(TIM3);
 *     }
 * }
 * @endcode
 */
uint32_t is_UIF(TIM_TypeDef *TIMx);

/**
 * @brief  Clear the update interrupt flag (UIF) of a timer.
 * @param[in] TIMx  Timer whose UIF is cleared
 * @note   Call at the end of the IRQ handler. If not cleared, the handler is re-entered repeatedly.
 * @par Example
 * @code
 * void TIM3_IRQHandler(void) {
 *     if (is_UIF(TIM3)) {
 *         // task
 *         clear_UIF(TIM3);           // without this, the handler is re-entered forever
 *     }
 * }
 * @endcode
 */
void clear_UIF(TIM_TypeDef *TIMx);


////////////////////////////////////////////////////////////////
/* Timer Counter Initialization                              */
////////////////////////////////////////////////////////////////

/**
 * @brief  Enable the timer clock, set the period in msec, set up-counting, and start the counter.
 * @param[in] TIMx  Timer to use (TIM1~TIM5, TIM9~TIM11)
 * @param[in] msec  Counter period [msec]. 16-bit timer: 1 ~ 6,553 msec
 * @note   f_cnt = 10 kHz (PSC = EC_SYSCLK / 10 kHz - 1). The update interrupt is NOT enabled.
 *         Use TIM_UI_init_ms() if an interrupt is needed.
 * @par Example
 * @code
 * TIM_init_ms(TIM3, 500);            // counter overflows every 500 msec
 * @endcode
 */
void TIM_init_ms(TIM_TypeDef *TIMx, uint32_t msec); 

/**
 * @brief  Enable the timer clock, set the period in usec, set up-counting, and start the counter.
 * @param[in] TIMx  Timer to use (TIM1~TIM5, TIM9~TIM11)
 * @param[in] usec  Counter period [usec]. 16-bit timer: 1 ~ 65,535 usec
 * @note   f_cnt = 1 MHz (PSC = EC_SYSCLK / 1 MHz - 1). The update interrupt is NOT enabled.
 *         Use TIM_UI_init_us() if an interrupt is needed.
 * @par Example
 * @code
 * TIM_init_us(TIM4, 1000);           // counter overflows every 1 msec, 1 tick = 1 usec
 * @endcode
 */
void TIM_init_us(TIM_TypeDef *TIMx, uint32_t usec); 

/**
 * @brief  Set the counter period in msec (PSC and ARR only).
 * @param[in] TIMx  Timer to configure
 * @param[in] msec  Counter period [msec]. 16-bit timer: 1 ~ 6,553 msec
 * @note   f_cnt = 10 kHz, ARR = 10 * msec - 1. Clock and counter enable are not touched.
 * @par Example
 * @code
 * TIM_period_ms(TIM3, 100);          // change the period to 100 msec at runtime
 * @endcode
 */
void TIM_period_ms(TIM_TypeDef* TIMx, uint32_t msec);	

/**
 * @brief  Set the counter period in usec (PSC and ARR only).
 * @param[in] TIMx  Timer to configure
 * @param[in] usec  Counter period [usec]. 16-bit timer: 1 ~ 65,535 usec
 * @note   f_cnt = 1 MHz, ARR = usec - 1. Clock and counter enable are not touched.
 * @par Example
 * @code
 * TIM_period_us(TIM3, 50);           // change the period to 50 usec (20 kHz)
 * @endcode
 */
void TIM_period_us(TIM_TypeDef* TIMx, uint32_t usec);  	

/**
 * @brief  Set both the tick size and the counter period in usec.
 * @param[in] TIMx       Timer to configure
 * @param[in] tick_us    One counter tick [usec]. PSC is 16-bit → 1 ~ 780 usec at 84 MHz
 * @param[in] period_us  Counter period [usec]. Must be a multiple of tick_us
 * @note   PSC = (EC_SYSCLK / 1 MHz) * tick_us - 1,  ARR = period_us / tick_us - 1.
 * @note   (period_us / tick_us) must not exceed 65,536 on a 16-bit timer.
 * @par Example
 * @code
 * TIM_period_tick(TIM3, 10, 100000); // 1 tick = 10 usec, period = 100 msec (ARR = 9999)
 * @endcode
 */
void TIM_period_tick(TIM_TypeDef* TIMx, uint32_t tick_us, uint32_t period_us);

/**
 * @brief  Get the NVIC IRQ number of a timer.
 * @param[in] TIMx  Timer (TIM1~TIM5, TIM9~TIM11)
 * @return IRQn value for NVIC_EnableIRQ() / NVIC_SetPriority()
 * @note   TIM1 update and TIM10 share TIM1_UP_TIM10_IRQn (TIM1_UP_TIM10_IRQHandler()).
 *         TIM9 → TIM1_BRK_TIM9_IRQn, TIM11 → TIM1_TRG_COM_TIM11_IRQn.
 * @par Example
 * @code
 * uint32_t irq = get_TIM_IRQn(TIM2); // TIM2_IRQn
 * NVIC_SetPriority(irq, 1);          // change priority after TIM_UI_init_xx()
 * @endcode
 */
uint32_t get_TIM_IRQn(TIM_TypeDef* TIMx);

/**
 * @brief  Enable the RCC clock of a timer.
 * @param[in] TIMx  Timer (TIM1, TIM9~TIM11 on APB2 / TIM2~TIM5 on APB1)
 * @note   Called inside TIM_init_ms() / TIM_init_us().
 * @par Example
 * @code
 * TIM_clock_enable(TIM2);            // RCC->APB1ENR |= TIM2EN
 * @endcode
 */
void TIM_clock_enable(TIM_TypeDef* TIMx);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_TIM2_H 
