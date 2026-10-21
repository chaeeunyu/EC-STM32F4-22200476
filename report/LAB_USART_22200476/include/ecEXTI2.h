/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-01-2026
Language/ver     : C++ in VS Code

Description      : ecEXTI.h
/----------------------------------------------------------------*/

/**
 * @file    ecEXTI2.h
 * @brief   External interrupt (EXTI) driver for STM32F411.
 *
 * Embedded Controller by Young-Keun Kim - Handong Global University
 *
 * @author  Yu Chaeeun
 * @date    Created: 05-03-2021, Modified: Oct-01-2026
 * @note    Language: C (VS Code, PlatformIO)
 */

#ifndef __EC_EXTI2_H
#define __EC_EXTI2_H

#include "stm32f411xe.h"
#include "ecPinNames.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup EXTI EXTI Driver
 * @brief    Configure external interrupts on GPIO pins: trigger edge, NVIC priority,
 *           enable/disable (mask), and pending-flag handling.
 * @{
 */

#define FALL 0
#define RISE 1
#define BOTH 2

/**
 * @brief  Connect a GPIO pin to its EXTI line, set the trigger edge, and enable the interrupt in NVIC.
 * @param[in] pinName    Pin to use as interrupt source (e.g., PC_13).
 *                       The pin number (0~15) selects the EXTI line, the port selects the source.
 * @param[in] trig_type  FALL, RISE, or BOTH
 * @param[in] priority   NVIC priority, 0 (highest) ~ 15 (lowest)
 * @note   Initialize the pin as INPUT with GPIO_init() first. The SYSCFG clock is enabled inside.
 * @note   Each EXTI line is shared by all ports with the same pin number
 *         (e.g., PA_13 and PC_13 cannot both be used).
 * @note   Lines 5~9 share EXTI9_5_IRQHandler(), lines 10~15 share EXTI15_10_IRQHandler().
 */
void EXTI_init(PinName_t pinName, uint32_t trig_type,uint32_t priority);

/**
 * @brief  Enable (unmask) the EXTI interrupt of a pin.
 * @param[in] pinName  Pin whose EXTI line is enabled
 * @note   EXTI_init() must be called first.
 */
void EXTI_enable(PinName_t pinName);

/**
 * @brief  Disable (mask) the EXTI interrupt of a pin. The configuration is kept.
 * @param[in] pinName  Pin whose EXTI line is disabled
 */
void EXTI_disable(PinName_t pinName);

/**
 * @brief  Check whether the EXTI line of a pin has a pending interrupt.
 * @param[in] pinName  Pin to check
 * @return 1 if pending, 0 otherwise
 * @note   Use inside the IRQ handler to find which line triggered.
 */
uint32_t is_pending_EXTI(PinName_t pinName);

/**
 * @brief  Clear the pending flag of the EXTI line of a pin.
 * @param[in] pinName  Pin whose pending flag is cleared
 * @note   Call at the end of the IRQ handler. If not cleared, the handler is re-entered repeatedly.
 */
void clear_pending_EXTI(PinName_t pinName);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */
	 
#endif // __EC_EXTI2_H
