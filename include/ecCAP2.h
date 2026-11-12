/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Nov-5-2026 
Language/ver     : C++ in VS Code

Description      : Input Capture library - ecCAP2.h
/----------------------------------------------------------------*/

/**
 * @file    ecCAP2.h
 * @brief   Timer input capture (ICAP) driver for STM32F411.
 *
 * Embedded Controller by Young-Keun Kim - Handong Global University
 *
 * @author  Yu Chaeeun
 * @date    Created: 05-03-2021, Modified: Nov-05-2026
 * @note    Language: C (VS Code, PlatformIO)
 */

#ifndef __EC_ICAP2_H
#define __EC_ICAP2_H

#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecTIM2.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup ICAP Input Capture Driver
 * @brief    Capture the timer counter value at a pin edge to measure pulse width,
 *           period, or time interval (e.g., ultrasonic echo).
 *
 * Supported pins use the same TIMx_CHn map as the PWM driver
 * (PA0, PA1, PA5, PA6, PA8~PA10, PA15, PB3~PB10, PC6~PC9).
 * @{
 */


/** @name Edge Type */
/** @{ */
#define IC_RISE 0
#define IC_FALL 1
#define IC_BOTH 2
/** @} */

/** @name Input Capture Number */
/** @{ */
#define IC_1 1
#define IC_2 2
#define IC_3 3
#define IC_4 4
/** @} */

/** @name Input Capture Filter (ICxF) */
/** @{ */ 
#define IC_FILTER_0		0		// no filter, transition sampled at fDTS
#define IC_FILTER_1		1		// N = 2 samples at fCK_INT
#define IC_FILTER_2		2		// N = 4 samples at fCK_INT
#define IC_FILTER_3		3		// N = 8 samples at fCK_INT
/** @} */

////////////////////////////////////////////////////////////////
/* Timer Input Capture  Initialization                        */
////////////////////////////////////////////////////////////////

/**
 * @brief  Initialize input capture on a pin with its own channel (ICn = TIn) and a 1 usec tick.
 * @param[in] pinName    Capture input pin (e.g., PB_6 → TIM4_CH1 → IC1)
 * @param[in] edge_type  IC_RISE, IC_FALL, or IC_BOTH
 * @note   Enables the capture interrupt (CCnIE), the update interrupt (UIE), and NVIC (priority 2).
 *         Handle both in TIMx_IRQHandler(): UIF for overflow counting, CCnIF for captures.
 * @note   ARR is set to the maximum (0xFFFF, or 0xFFFFFFFF for TIM2/TIM5).
 * @par Example
 * @code
 * ICAP_init(PB_6, IC_RISE);          // TIM4_CH1 → IC1, capture rising edge
 * @endcode
 */
void ICAP_init(PinName_t pinName, uint32_t edge_type);

/**
 * @brief  Initialize input capture on a pin with a user-selected IC channel and a 1 usec tick.
 * @param[in] pinName    Capture input pin (TIn is decided by the pin)
 * @param[in] edge_type  IC_RISE, IC_FALL, or IC_BOTH
 * @param[in] ICn        IC_1 ~ IC_4. Either the same channel (ICn = TIn) or its pair
 *                       (IC1↔TI2, IC2↔TI1, IC3↔TI4, IC4↔TI3)
 * @note   Use two ICs on one pin to capture both edges into separate registers
 *         (e.g., rising → IC1, falling → IC2) for pulse-width measurement.
 * @note   Call order: ICAP_init() → ICAP_ICn_init() → ICAP_tick() (last).
 * @par Example
 * @code
 * ICAP_init(PB_6, IC_RISE);                 // TI1 → IC1 : rising edge
 * ICAP_ICn_init(PB_6, IC_FALL, IC_2);       // TI1 → IC2 : falling edge
 * ICAP_tick(PB_6, 10);                      // 1 tick = 10 usec  <- call last
 * @endcode
 */
void ICAP_ICn_init(PinName_t pinName, uint32_t edge_type, uint32_t ICn);

/**
 * @brief  Set the counter tick of the capture timer in usec. ARR is kept at maximum.
 * @param[in] pinName  Capture input pin (selects the timer)
 * @param[in] usec     One tick [usec]. PSC is 16-bit → 1 ~ 780 usec at 84 MHz (0 is changed to 1)
 * @note   Call this LAST: ICAP_ICn_init() resets the tick to 1 usec.
 * @note   Measured time [usec] = (captured ticks) * usec.
 * @par Example
 * @code
 * ICAP_init(PB_6, IC_RISE);
 * ICAP_ICn_init(PB_6, IC_FALL, IC_2);
 * ICAP_tick(PB_6, 10);               // 1 tick = 10 usec, 16-bit overflow every 655.36 msec
 * @endcode
 */
void ICAP_tick(PinName_t pinName, uint32_t usec);

/**
 * @brief  Read the captured counter value of an IC channel.
 * @param[in] pinName  Capture input pin (selects the timer)
 * @param[in] ICn      IC_1 ~ IC_4
 * @return Captured CCRn value [ticks]
 * @note   Reading CCRn also clears its CCnIF flag in hardware.
 * @par Example
 * @code
 * volatile uint32_t ovf_cnt = 0;
 * volatile float time1 = 0, time2 = 0, timeInterval = 0;   // [usec]
 *
 * void TIM4_IRQHandler(void) {
 *     if (is_UIF(TIM4)) {                        // counter overflow
 *         ovf_cnt++;
 *         clear_UIF(TIM4);
 *     }
 *     if (is_CCIF(TIM4, IC_1)) {                 // rising edge
 *         time1 = ICAP_read(PB_6, IC_1);
 *         clear_CCIF(TIM4, IC_1);
 *     }
 *     else if (is_CCIF(TIM4, IC_2)) {            // falling edge
 *         time2 = ICAP_read(PB_6, IC_2);
 *         timeInterval = ((time2 - time1) + ovf_cnt * (TIM4->ARR + 1)) * 10;  // 10 usec tick
 *         ovf_cnt = 0;
 *         clear_CCIF(TIM4, IC_2);
 *     }
 * }
 * @endcode
 */
uint32_t ICAP_read(PinName_t pinName, uint32_t ICn);


//////////////////////////////////////////////
/* Timer Input CC Interrupt Configuration         */
//////////////////////////////////////////////
/**
 * @brief  Check the capture/compare interrupt flag (CCnIF) of an IC channel.
 * @param[in] TIMx  Capture timer
 * @param[in] ICn   IC_1 ~ IC_4
 * @return 1 if a capture occurred, 0 otherwise
 * @note   Use inside TIMx_IRQHandler() to find which channel captured.
 * @par Example
 * @code
 * void TIM4_IRQHandler(void) {
 *     if (is_CCIF(TIM4, IC_1)) {
 *         time1 = ICAP_read(PB_6, IC_1);
 *         clear_CCIF(TIM4, IC_1);
 *     }
 * }
 * @endcode
 */
uint32_t is_CCIF(TIM_TypeDef *TIMx, uint32_t  ICn);  // CCnum= 1~4

/**
 * @brief  Clear the capture/compare interrupt flag (CCnIF) of an IC channel.
 * @param[in] TIMx  Capture timer
 * @param[in] ICn   IC_1 ~ IC_4
 * @note   Call at the end of each capture branch in the IRQ handler.
 * @par Example
 * @code
 * if (is_CCIF(TIM4, IC_2)) {
 *     time2 = ICAP_read(PB_6, IC_2);
 *     clear_CCIF(TIM4, IC_2);
 * }
 * @endcode
 */
void clear_CCIF(TIM_TypeDef *TIMx, uint32_t  ICn);


//////////////////////////////////////////////
/* Internal Functions                       */
//////////////////////////////////////////////
/**
 * @brief  Internal: find the timer and channel (TIn) connected to a pin.
 * @param[in]  pinName  Pin to look up
 * @param[out] TIMx     Timer of the pin
 * @param[out] chN      Timer input channel TIn, 1 ~ 4
 * @par Example
 * @code
 * TIM_TypeDef *TIMx;
 * int TIn;
 * ICAP_pinmap(PB_6, &TIMx, &TIn);    // TIMx = TIM4, TIn = 1
 * @endcode
 */
void ICAP_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN);

/**
 * @brief  Internal: set the digital input filter (ICnF) of one IC channel.
 * @param[in] TIMx       Capture timer
 * @param[in] ICn        IC_1 ~ IC_4
 * @param[in] filterOpt  IC_FILTER_0 ~ IC_FILTER_3
 * @note   Only the given channel is written, so output channels of the same timer are not affected
 *         (ICxF bits share the position of OCxM bits in CCMRx).
 * @par Example
 * @code
 * ICAP_filter(TIM4, IC_1, IC_FILTER_0);  // remove the default 8-sample filter on IC1
 * @endcode
 */
void ICAP_filter(TIM_TypeDef *TIMx, uint32_t ICn, uint16_t filterOpt);

/**
 * @brief  Internal: connect a timer input (TIn) to an input capture channel (ICn) via CCnS.
 * @param[in] TIMx  Capture timer
 * @param[in] TIn   Timer input channel of the pin, 1 ~ 4
 * @param[in] ICn   IC_1 ~ IC_4
 * @note   ICn = TIn → CCnS = 01 (direct). ICn ≠ TIn → CCnS = 10 (paired input:
 *         IC1↔TI2, IC2↔TI1, IC3↔TI4, IC4↔TI3).
 * @par Example
 * @code
 * ICAP_ICn_select(TIM4, 1, IC_2);    // TI1 → IC2 (CC2S = 10)
 * @endcode
 */
void ICAP_ICn_select(TIM_TypeDef *TIMx, uint32_t TIn, uint32_t ICn);

/** @} */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // __EC_ICAP2_H