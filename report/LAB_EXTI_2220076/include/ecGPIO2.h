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

/*---------------------------------------------------------------- 
                    [EXERCISE]
---------------------------------------------------------------- */
// ADD MORE  MACRO related to GPIO 
// PU, PD, NO_PUPD, PUSH_PULL, LOW, MEDIUM, etc
// GPIO_ospeed
#define LOWSPEED 0x00
#define MEDSPEED 0x01
#define FASTSPEED 0x02
#define HIGHSPEED 0x03

// GPIO_pupd
#define NPUPD 0x00
#define PULL_UP 0x01
#define PULL_DOWN 0x02
// #define REV 0x03  --> not used.

// GPIO_otype
#define PUSH_PULL 0x00
#define OPEN_DRAIN 0x01




#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
void GPIO_init(PinName_t pinName, uint32_t mode);     
void GPIO_mode(PinName_t pinName, uint32_t mode);
void GPIO_ospeed(PinName_t pinName, uint32_t speed);
void GPIO_otype(PinName_t pinName, uint32_t type);
void GPIO_pupd(PinName_t pinName, uint32_t pupd);
void GPIO_write(PinName_t pinName, uint32_t Output);
uint32_t  GPIO_read(PinName_t pinName);

// ARDUINO Syntax  (Optional)
//void pinMode(PinName_t pin, uint32_t mode);
//void digitalWrite(PinName_t pin, uint32_t value);
//uint32_t  digitalRead(PinName_t pin);


/*---------------------------------------------------------------- 
						[GPIO_7segment.h]
---------------------------------------------------------------- */

// FND for JKIT board 
extern int numberFND[12][8];
void FND_init(PinName_t *pinFND);
void FND_select_init(PinName_t *selectFND);
void FND_display(uint8_t num);
void FND_select(uint8_t digit);

void FND_select_pupd(PinName_t *selectFND, uint32_t pupd);
void FND_select_otype(PinName_t *selectFND, uint32_t otype);
void FND_select_ospeed(PinName_t *selectFND, uint32_t ospeed);


 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __ECGPIO2_H
