/**
 * @file    ecGPIO2.h
 * @brief   GPIO and 7-segment (FND) driver for STM32F411.
 *
 * Embedded Controller by Young-Keun Kim - Handong Global University
 *
 * @author  Yu Chaeeun
 * @date    Created: 05-03-2021, Modified: Sep-14-2026
 * @note    Language: C (VS Code, PlatformIO)
 */

 /*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Sep-14-2026
Language/ver     : C++ in VS Code

Description      : GPIO Library. Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/

#ifndef __ECGPIO2_H
#define __ECGPIO2_H

#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecPinNames.h"

// GPIO_mode
#define INPUT  0x00
#define OUTPUT 0x01
#define AF     0x02
#define ANALOG 0x03

#define HIGH 1
#define LOW  0

// GPIO_ospeed
#define LOWSPEED  0x00
#define MEDSPEED  0x01
#define FASTSPEED 0x02
#define HIGHSPEED 0x03

// GPIO_pupd
#define NPUPD     0x00
#define PULL_UP   0x01
#define PULL_DOWN 0x02

// GPIO_otype
#define PUSH_PULL  0x00
#define OPEN_DRAIN 0x01

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup GPIO GPIO Driver
 * @brief    Initialize GPIO pins and configure mode, speed, output type,
 *           pull-up/down, and read/write pin levels.
 * @{
 */

/**
 * @brief  Initialize a GPIO pin: enable the port clock and set the pin mode.
 * @param[in] pinName  Pin to initialize (e.g., PA_5). See ecPinNames.h.
 * @param[in] mode     INPUT, OUTPUT, AF, or ANALOG
 * @note   Call this before any other GPIO_* function on the same pin.
  * @par Example
 * @code
 * GPIO_init(PA_5, OUTPUT);     // LED (LD2)
 * GPIO_init(PC_13, INPUT);     // user button (B1)
 * @endcode
 */
void GPIO_init(PinName_t pinName, uint32_t mode);

/**
 * @brief  Set the mode of a GPIO pin (MODER register).
 * @param[in] pinName  Target pin (e.g., PA_5)
 * @param[in] mode     INPUT, OUTPUT, AF, or ANALOG
 * @par Example
 * @code
 * GPIO_mode(PA_5, OUTPUT);     // LED
 * GPIO_mode(PA_2, AF);         // USART2 TX
 * @endcode
 */
void GPIO_mode(PinName_t pinName, uint32_t mode);

/**
 * @brief  Set the output speed of a GPIO pin (OSPEEDR register).
 * @param[in] pinName  Target pin
 * @param[in] speed    LOWSPEED, MEDSPEED, FASTSPEED, or HIGHSPEED
 * @note   Effective only in OUTPUT or AF mode.
  * @par Example
 * @code
 * GPIO_init(PA_5, OUTPUT);
 * GPIO_ospeed(PA_5, FASTSPEED);
 * @endcode
 */
void GPIO_ospeed(PinName_t pinName, uint32_t speed);

/**
 * @brief  Set the output type of a GPIO pin (OTYPER register).
 * @param[in] pinName  Target pin
 * @param[in] type     PUSH_PULL or OPEN_DRAIN
 * @note   Effective only in OUTPUT or AF mode.
  * @par Example
 * @code
 * GPIO_init(PA_5, OUTPUT);
 * GPIO_otype(PA_5, PUSH_PULL);
 * @endcode
 */
void GPIO_otype(PinName_t pinName, uint32_t type);

/**
 * @brief  Set the internal pull-up/pull-down resistor of a GPIO pin (PUPDR register).
 * @param[in] pinName  Target pin
 * @param[in] pupd     NPUPD, PULL_UP, or PULL_DOWN
 * @par Example
 * @code
 * GPIO_init(PA_0, INPUT);
 * GPIO_pupd(PA_0, PULL_UP);    // switch connected between PA_0 and GND
 * @endcode
 */
void GPIO_pupd(PinName_t pinName, uint32_t pupd);

/**
 * @brief  Write a logic level to a GPIO output pin.
 * @param[in] pinName  Target pin
 * @param[in] Output   HIGH (1) or LOW (0)
 * @note   The pin must be initialized in OUTPUT mode.
  * @par Example
 * @code
 * GPIO_init(PA_5, OUTPUT);
 * GPIO_write(PA_5, HIGH);   // LED on
 * @endcode
 */
void GPIO_write(PinName_t pinName, uint32_t Output);

/**
 * @brief  Read the logic level of a GPIO input pin.
 * @param[in] pinName  Target pin
 * @return HIGH (1) or LOW (0)
 * @note   The pin must be initialized in INPUT mode.
  * @par Example
 * @code
 * buttonState = GPIO_read(BUTTON_PIN);
 * @endcode
 */
uint32_t GPIO_read(PinName_t pinName);


/** Segment patterns for each displayable number. [number][segment a~g, dp] */
extern int numberFND[12][8];

/**
 * @brief  Initialize the 8 segment pins (a~g, dp) as outputs.
 * @param[in] pinFND  Array of 8 pins, in the order a, b, c, d, e, f, g, dp
 * @par Example
 * @code
 * PinName_t pinsFND[8] = {PA_7, PB_6, PC_7, PA_9, PA_8, PB_10, PA_6, PA_10};  // a, b, c, d, e, f, g, dp
 * FND_init(pinsFND);
 * @endcode
 */
void FND_init(PinName_t *pinFND);

/**
 * @brief  Initialize the digit-select pins as outputs.
 * @param[in] selectFND  Array of digit-select pins
 * @par Example
 * @code
 * PinName_t pinsSel[4] = {PC_0, PC_1, PC_2, PC_3};    // digit 0, 1, 2, 3
 * FND_select_init(pinsSel);
 * @endcode
 */
void FND_select_init(PinName_t *selectFND);

/**
 * @brief  Display a number on the currently selected digit.
 * @param[in] num  Number to display, 0~11 (index of numberFND)
 * @note   FND_init() must be called first.
  * @par Example
 * @code
 * uint8_t numDisplay1  = 5;
 * FND_display(numDisplay1);
 * @endcode
 */
void FND_display(uint8_t num);

/**
 * @brief  Select which digit of the display is active.
 * @param[in] digit  Digit index
 * @note   FND_select_init() must be called first.
  * @par Example
 * @code
 * uint8_t selectFND2  = 1;
 * FND_select(selectFND2);
 * @endcode
 */
void FND_select(uint8_t digit);

/**
 * @brief  Set pull-up/pull-down for array pins.
 * @param[in] pinArr  Array of pins (as a pointer)
 * @param[in] n       The size of the array (# of elements)
 * @param[in] pupd    NPUPD, PULL_UP, or PULL_DOWN
 * @note  GPIO_*_init() must be called first.
 * @par Example
 * @code
 * PinName_t pinsSel[4] = {PC_0, PC_1, PC_2, PC_3};
 * FND_select_init(pinsSel);
 * GPIO_pupd_arr(pinsSel, 4, NPUPD);
 * @endcode
 */
void GPIO_pupd_arr(PinName_t *pinArr, int n, uint32_t pupd);

/**
 * @brief  Set output type for array pins.
 * @param[in] pinArr  Array of pins (as a pointer)
 * @param[in] n       The size of the array (# of elements)
 * @param[in] otype      PUSH_PULL or OPEN_DRAIN
 * @note  GPIO_*_init() must be called first.
 * @par Example
 * @code
 * PinName_t pinsSel[4] = {PC_0, PC_1, PC_2, PC_3};
 * FND_select_init(pinsSel);
 * GPIO_otype_arr(pinsSel, 4, PUSH_PULL);
 * @endcode
 */
void GPIO_otype_arr(PinName_t *pinArr, int n, uint32_t otype);

/**
 * @brief  Set output speed for array pins.
 * @param[in] pinArr  Array of pins (as a pointer)
 * @param[in] n       The size of the array (# of elements)
 * @param[in] ospeed  LOWSPEED, MEDSPEED, FASTSPEED, or HIGHSPEED
 * @note  GPIO_*_init() must be called first.
 * @par Example
 * @code
 * PinName_t pinsSel[4] = {PC_0, PC_1, PC_2, PC_3};
 * FND_select_init(pinsSel);
 * GPIO_ospeed_arr(pinsSel, 4, MEDSPEED);
 * @endcode
 */
void GPIO_ospeed_arr(PinName_t *pinArr, int n, uint32_t ospeed);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // __ECGPIO2_H