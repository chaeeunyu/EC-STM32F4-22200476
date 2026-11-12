/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-31-2026 
Language/ver     : C in VS Code

Description      : PWM (Pulse Width Modulation) library of STM32F411 MCU
				  ecPWM.h
/----------------------------------------------------------------*/

/**
 * @file    ecPWM2.h
 * @brief   PWM (Pulse Width Modulation) output driver for STM32F411.
 *
 * Embedded Controller by Young-Keun Kim - Handong Global University
 *
 * @author  Yu Chaeeun
 * @date    Created: 05-03-2021, Modified: Oct-31-2026
 * @note    Language: C (VS Code, PlatformIO)
 */

#ifndef __EC_PWM2_STUDENT_H
#define __EC_PWM2_STUDENT_H

#include "math.h"
#include "stm32f411xe.h"
#include "ecGPIO2.h"
#include "ecTIM2.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup PWM PWM Driver
 * @brief    Generate PWM on a GPIO pin using a timer output-compare channel:
 *           period, duty ratio, and pulse width.
 *
 * Supported pins (TIMx_CHn):
 * | Port | Pin → Timer / Channel |
 * |------|-----------------------|
 * | GPIOA | PA0 TIM2_CH1, PA1 TIM2_CH2, PA5 TIM2_CH1, PA6 TIM3_CH1, PA8 TIM1_CH1, PA9 TIM1_CH2, PA10 TIM1_CH3, PA15 TIM2_CH1 |
 * | GPIOB | PB3 TIM2_CH2, PB4 TIM3_CH1, PB5 TIM3_CH2, PB6 TIM4_CH1, PB7 TIM4_CH2, PB8 TIM4_CH3, PB9 TIM4_CH4, PB10 TIM2_CH3 |
 * | GPIOC | PC6 TIM3_CH1, PC7 TIM3_CH2, PC8 TIM3_CH3, PC9 TIM3_CH4 |
 * @{
 */



/** @name PWM Output Mode (OCxM field in CCMRx) */
/** @{ */
#define EC_PWM_MODE1	0	// output active while CNT <  CCR  (default)
#define EC_PWM_MODE2	1	// output active while CNT >= CCR  (inverted)
/** @} */

///////////////////////////////////////////////////
/* 		        EXERCISE					     */
///////////////////////////////////////////////////

/**
 * @brief  Internal helper: configure the pin as AF, set up the timer, and start PWM.
 * @param[in] pinName  PWM output pin (see the pin table above)
 * @param[in] usec     PWM period [usec]
 * @note   Used by PWM_init() and PWM_init_us(). Call those instead.
 * @note   Sets AF1 (TIM1, TIM2) or AF2 (TIM3, TIM4), PWM mode 1, active high, 50% duty.
 * @note   TIM2/TIM5 or period <= 65,536 usec → TIM_init_us() (1 usec tick).
 *         Otherwise (16-bit timer, long period) → TIM_init_ms() (0.1 msec tick).
 * @note   Returns without doing anything if the pin has no PWM channel.
 */
void PWM_init_config(PinName_t pinName, uint32_t usec);

/**
 * @brief  Set the duty ratio of a PWM pin.
 * @param[in] pinName  PWM output pin
 * @param[in] duty     Duty ratio, 0.0 ~ 1.0 (clamped to this range)
 * @note   CCR = (ARR + 1) * duty. Assumes PWM mode 1.
 * @par Example
 * @code
 * PWM_init(PA_5, 1);                 // 1 msec period (1 kHz)
 * PWM_duty(PA_5, 0.25f);             // 25% duty → LED brightness 25%
 * @endcode
 */
void PWM_duty(PinName_t pinName, float duty);



///////////////////////////////////////////////////
/* 		GIVEN:  PWM Initialization                */
///////////////////////////////////////////////////

/**
 * @brief  Initialize PWM with the period in msec (low frequency, 1 kHz or below).
 * @param[in] pinName  PWM output pin (see the pin table above)
 * @param[in] msec     PWM period [msec]. 0 is changed to 1.
 *                     TIM2/TIM5 (32-bit): 1 ~ 4,294,967 msec,
 *                     TIM1/TIM3/TIM4 (16-bit): 1 ~ 6,553 msec
 * @note   Default: PWM mode 1, active high, 50% duty.
 * @par Example
 * @code
 * PWM_init(PA_1, 20);                // RC servo: 20 msec period (50 Hz)
 * PWM_pulsewidth_us(PA_1, 1500);     // 1.5 msec pulse → servo center
 * @endcode
 */
void PWM_init(PinName_t pinName, uint32_t msec);	// 1 to 6,553 msec (16-bit)  - 1ms이상일때 

/**
 * @brief  Initialize PWM with the period in usec (high frequency, 1 kHz ~ 100 kHz).
 * @param[in] pinName  PWM output pin (see the pin table above)
 * @param[in] usec     PWM period [usec]. Minimum 10 usec (100 kHz); smaller values are changed to 10.
 * @note   Default: PWM mode 1, active high, 50% duty. 1 tick = 1 usec,
 *         so the duty resolution is 1/usec (e.g., 10 usec period → 10 steps).
 * @par Example
 * @code
 * PWM_init_us(PB_6, 50);             // DC motor: 50 usec period (20 kHz)
 * PWM_duty(PB_6, 0.6f);              // 60% speed
 * @endcode
 */
void PWM_init_us(PinName_t pinName, uint32_t usec);	// 10 usec and up            - 1ms이하일때


///////////////////////////////////////////////////
/* 		GIVEN: DUTY RATIO SETUP                 */
///////////////////////////////////////////////////

/**
 * @brief  Set the pulse width (high time) of a PWM pin in msec.
 * @param[in] pinName         PWM output pin
 * @param[in] pulse_width_ms  Pulse width [msec], must be <= PWM period
 * @note   Converted to a duty ratio using the current PSC and ARR, then PWM_duty() is called.
 * @par Example
 * @code
 * PWM_init(PA_0, 100);               // 100 msec period
 * PWM_pulsewidth(PA_0, 30);          // 30 msec high → 30% duty
 * @endcode
 */
void PWM_pulsewidth(PinName_t pinName, uint32_t pulse_width_ms);

/**
 * @brief  Set the pulse width (high time) of a PWM pin in usec.
 * @param[in] pinName         PWM output pin
 * @param[in] pulse_width_us  Pulse width [usec], must be <= PWM period
 * @note   Converted to a duty ratio using the current PSC and ARR, then PWM_duty() is called.
 * @par Example
 * @code
 * PWM_init(PA_1, 20);                // RC servo: 20 msec period
 * PWM_pulsewidth_us(PA_1, 500);      // 0.5 msec → 0 deg
 * PWM_pulsewidth_us(PA_1, 2500);     // 2.5 msec → 180 deg
 * @endcode
 */
void PWM_pulsewidth_us(PinName_t pinName, uint32_t pulse_width_us);


////////////////////////////////////////////////////////////////
/* Internal Helper Functions - GIVEN. DO NOT MODIFY           */
////////////////////////////////////////////////////////////////

/**
 * @brief  Internal: set the output-compare mode of a timer channel to PWM.
 * @param[in] TIMx  Timer
 * @param[in] chN   Channel number, 1 ~ 4
 * @param[in] mode  EC_PWM_MODE1 (OCxM = 110) or EC_PWM_MODE2 (OCxM = 111)
 * @note   Also enables CCR preload, sets active-high polarity, CCR = ARR/2 (50% duty),
 *         and enables the channel output (CCxE).
 * @par Example
 * @code
 * PWM_init(PA_5, 1);
 * PWM_mode_config(TIM2, 1, EC_PWM_MODE2);   // invert the output on TIM2_CH1
 * @endcode
 */
void PWM_mode_config(TIM_TypeDef *TIMx, int chN, uint32_t mode);

/**
 * @brief  Internal: find the timer and channel connected to a pin.
 * @param[in]  pinName  Pin to look up
 * @param[out] TIMx     Timer of the pin (unchanged if the pin has no PWM channel)
 * @param[out] chN      Channel number 1 ~ 4
 * @note   Only TIM1~TIM4 are mapped. See the pin table above.
 * @par Example
 * @code
 * TIM_TypeDef *TIMx = NULL;
 * int ch = 0;
 * PWM_pinmap(PB_6, &TIMx, &ch);      // TIMx = TIM4, ch = 1
 * @endcode
 */
void PWM_pinmap(PinName_t pinName, TIM_TypeDef **TIMx, int *chN);

/** @} */



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_PWM2_STUDENT_H
