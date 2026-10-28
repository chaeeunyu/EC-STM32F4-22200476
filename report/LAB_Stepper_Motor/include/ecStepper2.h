/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-12-2026 
Language/ver     : C++ in VS Code

Description      : Stepper Library. ecStepper2.h
/----------------------------------------------------------------*/

/**
 * @file    ecStepper2.h
 * @brief   Stepper motor library (28BYJ-48, unipolar) using an FSM for full-step and half-step drive.
 *
 * Embedded Controller by Young-Keun Kim - Handong Global University
 *
 * @author  Yu Chaeeun
 * @date    Created: 05-03-2021, Modified: Oct-12-2026
 * @note    Language: C (VS Code, PlatformIO)
 */


#include "stm32f411xe.h"
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"
			
#ifndef __EC_STEPPER2_H
#define __EC_STEPPER2_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


/**
 * @defgroup Stepper Stepper Motor
 * @brief    Drive a 4-phase stepper motor in full-step or half-step mode,
 *           with speed [rpm], step count, direction, and pause control.
 * @{
 */



// Stepper Mode
#define HALF 0     /**< Half-step mode (8 states) */
#define FULL 1	    /**< Full-step mode (4 states) */
#define DIR_CCW 0  /**< Counter-clockwise direction */
#define DIR_CW 1   /**< Clockwise direction */

// Stepper Motor Spec
// 28BYJ-48 : 64 steps/rev x 1/32 gear = 2048   
#define FULLSTEP_PER_REV  (64*32)		    /**< Full steps per output-shaft revolution (2048) */
#define HALFSTEP_PER_REV  (64*32*2)		 /**< Half steps per output-shaft revolution (4096) */

/** FSM state numbers. Full-step uses S0~S3, half-step uses S0~S7. */
typedef enum {
	S0, S1, S2, S3, S4, S5, S6, S7
} StateNum;

/** One FSM state: next state for each direction, and the coil output pattern. */
typedef struct {	
  uint32_t next[2];   /**< Next state. Index = direction (DIR_CCW or DIR_CW) */
  uint8_t out[4];    /**< Output levels for coils A, B, A', B' */
} StateStepper_t;



/**
 * @brief  Select the stepping mode and initialize the 4 motor pins as outputs.
 * @param[in] mode        FULL or HALF
 * @param[in] pinStepper  Array of 4 pins, in the order A, B, A', B'
 * @note   SysTick_init() must also be called before stepper_step(), since it uses delay_ms().
 * @par Example
 * @code
 * PinName_t pinStepper[4] = {PB_10, PB_4, PB_5, PB_3};
 * stepper_init(HALF, pinStepper);    // half-stepping mode
 * @endcode
 */
void stepper_init(uint32_t mode, PinName_t *pinStepper);

/**
 * @brief  Set the motor speed by converting [rpm] to the step period [ms].
 * @param[in] speedRPM      Motor speed [rpm]
 * @param[in] step_per_rev  # of steps per revolution
 * @par Example
 * @code
 * stepper_speed(5, FULLSTEP_PER_REV);    // 5 rpm -> _step_delay = 60000/(4096*5) = 2 ms (half-stepping)
 * @endcode
 */
void stepper_speed (uint32_t speedRPM, uint32_t step_per_rev);

/**
 * @brief  Rotate the motor for a given number of steps (blocking).
 * @param[in] steps      Number of steps
 * @param[in] direction  DIR_CW or DIR_CCW
 * @note   Returns immediately if stepper_init() was not called.
 *         While paused by stepper_pause(), the function waits.
 * @par Example
 * @code
 * stepper_step(HALFSTEP_PER_REV*5, DIR_CW);    // 5 revolutions, CW (half-stepping)
 * stepper_step(FULLSTEP_PER_REV*5, DIR_CCW);   // 5 revolutions, CCW (full-stepping)
 * @endcode
 */
void stepper_step(uint32_t steps, uint32_t direction);

/**
 * @brief  Write the coil output pattern of an FSM state to the motor pins.
 * @param[in] state  FSM state number (S0~S3 for FULL, S0~S7 for HALF)
 * @note   Called internally by stepper_step().
* @par Example
 * @code
 * stepper_out(S1);    // full-stepping: A=LOW, B=HIGH, A'=HIGH, B'=LOW
 * @endcode
 */
void stepper_out(uint32_t state);

/**
 * @brief  Toggle pause/resume of the running stepper_step().
 * @note   Despite the name, it does not stop the motor permanently.
 *         Each call toggles the pause state. Typically called from an EXTI handler (button).
  * @par Example
 * @code
 * void EXTI15_10_IRQHandler(void) {  
 *	if (is_pending_EXTI(BUTTON_PIN)) {
 *		stepper_pause();                     // pause or resume
 *		clear_pending_EXTI(BUTTON_PIN);
 *	}
 * }
 * @endcode
 */
void stepper_pause(void);

/**
 * @brief  stop the running stepper_step().
 * @note   It does stop the motor permanently.
  * @par Example
 * @code
 * void EXTI15_10_IRQHandler(void) {  
 *	if (is_pending_EXTI(BUTTON_PIN)) {
 *		stepper_stop();           
 *		clear_pending_EXTI(BUTTON_PIN);
 *	}
 * }
 * @endcode
 */
void stepper_stop(void);


/** @} */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_STEPPER2_H
