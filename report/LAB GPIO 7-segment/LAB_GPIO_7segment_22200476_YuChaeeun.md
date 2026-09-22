# LAB: GPIO 7-segment(eval board)

### LAB: GPIO 7-segment

**Date:** 2026-Sep-21

**Author/Partner:** 유채은

**PDF version:**

## Introduction

In this lab, you are required to create a simple program to control a 7-segment display to show a decimal number (0~9) that increases by pressing a push-button.

You must submit

- LAB Report (*.pdf)
- Zip source files(lab***.c, ecRCC2.h, ecGPIO2.h etc...).
  - Only the source files. Do not submit project files

**Requirement**

**Hardware**

- MCU
  - NUCLEO-F411RE
- Actuator/Sensor/Others:
  - eval board

**Software**

- PlatformIO, CMSIS, EC_HAL library

### Exercise

Fill in the table

| Port/Pin       | Description                  | Register setting             |
| -------------- | ---------------------------- | ---------------------------- |
| Port B Pin 5   | Clear Pin5 mode              | GPIOB->MODER &=~(3<<(5*2))   |
| Port B Pin 5   | Set Pin5 mode = Output       | GPIOB->MODER \|=____________ |
| Port B Pin 6   | Clear Pin6 mode              | GPIOB->MODER &=~___________  |
| Port B Pin 6   | Set Pin6 mode = Output       | GPIOB->MODER \|=____________ |
| Port B Pin Y   | Clear PinY mode              | GPIOB->MODER &=~___________  |
| Port B Pin Y   | Set PinY mode = Output       | GPIOB->MODER \|=____________ |
| Port B Pin 5~9 | Clear Pin5~9 mode            | GPIOB->MODER &=~___________  |
|                | Set Pin5~9 mode = Output     | GPIOB->MODER \|=____________ |
| Port X Pin Y   | Clear Pin Y mode             | GPIOX->MODER &=~___________  |
|                | Set Pin Y mode = Output      | GPIOX->MODER \|=____________ |
| Port B Pin5    | Set Pin5 otype=push-pull     | GPIOB->OTYPER =____________  |
| Port B PinY    | Set PinY otype=push-pull     | GPIOB-> OTYPER =____________ |
| Port B Pin5    | Set Pin5 ospeed=Fast         | GPIOB->OSPEEDR =____________ |
| Port B PinY    | Set PinY ospeed=Fast         | GPIOB-> OSPEEDR =___________ |
| Port B Pin 5   | Set Pin5 PUPD=no pullup/down | GPIOB->OTYPER =____________  |
| Port B Pin Y   | Set PinY PUPD=no pullup/down | GPIOB-> OTYPER =____________ |





## Problem 1. Library for 7-segment display on JKIT Board

#### Problem

There are four 7-Segment Displays on JKIT board.

- You need to select which one to use.
- There is no BCD decoder
- JKIT - Nucleo 64: [link](https://www.devicemart.co.kr/goods/view?no=14123215&srsltid=AfmBOooT3zgaxGXZ_q0fvy4uZxHesTbwpqNprE6P3YXMk9V0EGoGrhLM)



Complete the required functions that displays numbers on 7-segment FND.(JKIT - Nucleo 64)

These functions are defined and declared in `ecGPIO2.h,ecGPIO2.c`

<img src="./img/2026-09-22_163334.png">



Copy

```c
// Display a number 0 - 9 only
void FND_display_init(PinName_t *pinFND);
void FND_display(uint8_t  num)

// Select display: FND0 to FND3
void FND_select_init(PinName_t *selectFND);
void FND_select(uint8_t select);
```

Display a decimal number: 0~9 on each and all FNDs



If you want to display multiple 7-segment displays at the same time, you need to use a very short delay to select multiple numbers iteratively



**Check if the 7-segments display are Common Anode or Cathode**

**For Common Cathode: Giving 'High' to the pin -> LED ON**

#### Procedure

1. Connect the evaluation board [(JKIT-NUCLEO) ](https://ykkim.gitbook.io/ec/ec-course/hardware/stm32f-evaluation-board#reference-manual)to the MCU.
2. Make sure that your library **ecGPIO2.h, ecGPIO2.c** are in `EC\include\`.
3. Create a new project under the directory `EC\lab\`

- Environment: “**LAB_GPIO_7segment”**
- Source File: ““**LAB_GPIO_7segment.c”**

1. You must modify the `**platformio.ini**` **,** to add new environment.

> You MUST write your name in the top of the source file, inside the comment section.

**Configuration**

![img](https://ykkim.gitbook.io/ec/~gitbook/image?url=https%3A%2F%2Fraw.githubusercontent.com%2FLeeJunjae1%2FEC_22000573%2Fmain%2Fimg%2F7seg.png&width=768&dpr=3&quality=100&sign=07f2587375898a00d14d61ddf77a5b0a&sv=3)

config

![img](https://ykkim.gitbook.io/ec/~gitbook/image?url=https%3A%2F%2Fraw.githubusercontent.com%2FLeeJunjae1%2FEC_22000573%2Fmain%2Fimg%2FLED.png&width=768&dpr=3&quality=100&sign=0d3b3adc292ec12ef38ab4afcb982936&sv=3)

LED Choose



| Function                           | Port - Pin                                     | Configuration                 |
| ---------------------------------- | ---------------------------------------------- | ----------------------------- |
| **Selection of 7-Segment Display** | PA_10, PA_11, PC_4, PC_3 (FND_0~FND_3)         | DOUT, Push-Pull               |
| **7-Segment LEDs**                 | PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7 | DOUT, Push-Pull, Medium Speed |





## Problem 2: Counter with Button Press

#### Procedure

Create a code that increases the displayed number from 0 to 9 with each button press.

- After the number '9', it should start from '0' again.
- Modify from Problem 1 source code

#### Configuration

Configure the MCU GPIO

| Function                           | Port - Pin                                                   | Configuration                                 |
| ---------------------------------- | ------------------------------------------------------------ | --------------------------------------------- |
| **Button (SW2) on JKIT**           | PA_4                                                         | DIN, Pull-Up                                  |
| **7-Segment DOUT**                 | PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7<br />('a'~'g''dot', respectively) | Push-Pull, No Pull-up-Pull-down, Medium Speed |
| **Selection of 7-Segment Display** | PA_10 (FND_0)                                                | DOUT, Push-Pull                               |

*** Challenge : Extend the display number from 0 to 19**

**Code**

[**Sample Code**](https://ykkim.gitbook.io/ec/stm32-m4-programming/example-code#seven-segment).

Your code goes here:

> Explain your source code with necessary comments.



#### Connection Diagram

Circuit diagram (if needed)

> You need to include the circuit diagram

![img](https://ykkim.gitbook.io/ec/~gitbook/image?url=https%3A%2F%2Fuser-images.githubusercontent.com%2F38373000%2F192134563-72f68b29-4127-42ac-b064-2eda95a9a52a.png&width=768&dpr=3&quality=100&sign=24cfb4cbebca182e2f364217cc268c32&sv=3)

image

#### Results

Experiment images and results

> Show experiment images /results

Add [demo video link](https://github.com/ykkimhgu/course-doc/blob/master/course/lab/link/README.md)

#### 

#### Discussion

1. Analyze the result and explain any other necessary discussion.
2. What are the common cathode and common anode of 7-segment display?

> Answer discussion questions

1. Does the LED of a 7-segment display (common anode) pin turn ON when 'HIGH' is given to the LED pin from the MCU?

> Answer discussion questions

1. How can we display 4 digit-numbers on the 4 FNDS at the same time? How to remove ghosting of displaying a number?

> Answer discussion questions