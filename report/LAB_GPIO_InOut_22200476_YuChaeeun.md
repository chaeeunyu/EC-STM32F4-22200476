# LAB: GPIO Digital InOut

**Date:** 2026-09-02

**Author/Partner:** 유채은

**Github:** repository link

**Demo Video:** Youtube link

**PDF version:**

## Introduction

In this lab, you are required to create a simple program that toggles multiple LEDs using a push-button input.

You must create your own drivers for GPIO digital input and output control.

You must submit

- LAB Report (*.pdf)
- Zip source files(*main*.c, ecRCC.h, ecGPIO.h etc...).
  - Only the source files.
  - Do not submit project files

### Requirement

Write a list of HW/SW requirement

#### Hardware

- MCU
  - NUCLEO-F411RE
- Eval Board
- Actuator/Sensor/Others:

#### Software

- PlatformIO, CMSIS, EC_HAL library





## Problem 1: Create EC_HAL library

### Procedure

#### **Prepare Library Files**

Manage your library in the directory of `\repos\EC\include\`.

> DO NOT make duplicates of library files under each project folders

- **For VS.Code User:** Save your header library files in this directory. [See here for detail.](https://ykkim.gitbook.io/ec/ec-course/tutorial/tutorial-platformio-in-vscode)
- **For uVision User:** Save your header library files in this directory. [See here for detail.](https://ykkim.gitbook.io/ec/uvision/adding-my-api-header-in-uvision)

[Download library files.](https://github.com/ykkimhgu/EC-student/tree/main/include/lib-student)

- `ecRCC2.h, ecRCC2.c`
- `ecPinNames.h, ecPinNames.c`
- `ecGPIO2_student.h, ecGPIO2_student.c`

Create your own library for GPIO by renaming the downloaded files (`ecGPIO2_student.*)` as

- `ecGPIO2.c`, `ecGPIO2.h`
- Open `ecGPIO2.c` and change from `#include "ecGPIO2_student.h"` to `#include "ecGPIO2.h"`
- Write your name and modified date in the comment section

> You MUST write your name inside the comment section.



#### Function Definitions

Fill-in the blanks in the header files to complete the definitions for the provided functions



### Description with Code

- Lab source code: 깃허브 LINK 여기에 넣어 <-----------------------------------------------



#### GPIO_init()

Initialize a selected port and enable its clock.

```C
void GPIO_init(PinName_t pinName, uint32_t mode){     // clock enable
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);
	
	// mode  : Input(0), Output(1), AlterFunc(2), Analog(3)   
	if (Port == GPIOA)
		RCC_GPIOA_enable();
	if (Port == GPIOB)
		RCC_GPIOB_enable();
	if (Port == GPIOC)
		RCC_GPIOC_enable();
	if(Port == GPIOD)
		RCC_GPIOD_enable();
	if(Port == GPIOE)
		RCC_GPIOE_enable();

	GPIO_mode(pinName, mode);
}
```

##### parameter

- pinName : the pin name used in the STM board.
- mode : the mode of the selected port

##### example code 

```c
#define OUTPUT 0x01
```

```c
GPIO_init(A_13, OUTPUT);
```



#### GPIO_mode()

Select a mode of a port.

Run in the function 'GPIO_init.'

```c
void GPIO_mode(PinName_t pinName, uint32_t mode){
   GPIO_TypeDef * Port;
   unsigned int pin;
   ecPinmap(pinName,&Port,&pin);

   Port->MODER &= ~(3UL<<(2*pin));     
   Port->MODER |= mode<<(2*pin);    
}
```



#### GPIO_ospeed()

Selects output speed.

```c
// GPIO Speed          : Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
void GPIO_ospeed(PinName_t pinName, uint32_t speed){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	Port->OSPEEDR &= ~(3UL<<(2*pin));
	Port->OSPEEDR |= speed<<(2*pin);
}
```

##### parameter

- pinName 
- speed : the speed of output

##### example code

```c
#define FASTSPEED 0x02
```

```c
GPIO_ospeed(A_13, FASTSPEED);
```



#### GPIO_otype()

Selects output type.

```c
// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
void GPIO_otype(PinName_t pinName, uint32_t type){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

   	Port->OTYPER &= ~(1UL<<pin);
	Port->OTYPER |= type<<pin;
}
```

##### parameter

- pinName 
- type : the type of output

##### example code

```c
#define PUSH_PULL 0x00
```

```c
GPIO_ospeed(A_13, PUSH_PULL);
```



#### GPIO_write()

Writes 1 or 0 in a selected pin.

```c
void GPIO_write(PinName_t pinName, uint32_t Output){
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	Output == HIGH ? (Port->ODR |= (1UL<<pin)) : (Port->ODR &= ~(1UL<<pin));
}
```

##### parameter

- pinName 
- Output : 1 or 0, decides whether to set HIGH or LOW.

##### example code

```c
#define LED_PIN PB_12
#define PUSH_PULL 0x00
```

```c
GPIO_write(LED_PIN, LOW);
```



#### GPIO_read()

Reads the value of a selected pin.

```c
uint32_t GPIO_read(PinName_t pinName){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);
	unsigned int value;

   	value = (Port->IDR) & (1UL<<pin);
	// or, value = ((Port->IDR)>>pin)&(1UL);  ---> more desirable
	return value; 
}
```

##### parameter

- pinName 

##### example code

```c
#define BUTTON_PIN PA_0
```

```c
GPIO_write(BUTTON_PIN);
```







------

## Problem 2: Toggle a single LED with Digital Sensor(Photodetector)

### Problem

Toggle the LED by covering the photodetector sensor.

- Dark (LED ON), Bright (LED OFF) and repeat
- Connect the Photodetector sensor to PA_0 pin on Nucleo Board.

### Procedure

1. Connect the evaluation board [(JKIT-NUCLEO) ](https://ykkim.gitbook.io/ec/ec-course/hardware/stm32f-evaluation-board#reference-manual)to the MCU.
2. Make sure that your library **ecGPIO2.h, ecGPIO2.c** are in `EC\include\`.
3. Create a new project under the directory `EC\lab\`

- Environment: “**LAB_GPIO_DIO_LED_Photosensor”.**
- Source File: “**LAB_GPIO_DIO_LED_Photosensor.c”**

1. You must modify the `**platformio.ini**` **,** to add new environment.

### Configuration

Configure the MCU GPIO

| Function                  | Port - Pin | Configuration                                  |
| ------------------------- | ---------- | ---------------------------------------------- |
| **Photodetector on JKIT** | PA_0       | Digital IN, Pull-Up                            |
| **LED0**                  | PB_12      | Digital OUT, Pull-UP, Open-Drain, Medium Speed |



### Code

Your code goes here:

#### void setup()

Initializes clock and pins with the given configuration.

```c
// Initialiization 
void setup(void) {
	RCC_HSI_init();	// initialize clock

	// initialize the pushbutton pin as an input:
	GPIO_init(BUTTON_PIN, INPUT); 
	GPIO_pupd(BUTTON_PIN, PULL_UP);		// BUTTON_PIN: pull-up

	// initialize the LED pin as an output:
	GPIO_init(LED_PIN, OUTPUT);    
	GPIO_pupd(LED_PIN, PULL_UP);		// LED_PIN: pull-up
	GPIO_otype(LED_PIN, OPEN_DRAIN);	// open-drain
	GPIO_ospeed(LED_PIN, MEDSPEED); 	// medium speed
}
```





## Results and Analysis

### Results

Explain the final result with necessary images, graphs etc

사진 여기에

The first LED(pin PB_12) lights whenever covering the photosensor with a finger, and the light is turned off when the sensor is uncovered.



### Demo Video

Demo video link: https://youtube.com/shorts/3KT8f28M5Fo



### Analysis

Briefly analyze results





------

## Problem 3: Toggle multiple LEDs with a button

### Problem

Use the four LEDs (LED0 to LED3) on the JKIT evaluation board.

Each time Button PA4 is pressed, turn on one LED at a time in a sequential loop

- e.g., LED0 -> LED1 -> LED2 -> LED3 -> LED0)

### Procedure

1. Connect the evaluation board [(JKIT-NUCLEO) ](https://ykkim.gitbook.io/ec/ec-course/hardware/stm32f-evaluation-board#reference-manual)to the MCU.
2. Make sure that your library **ecGPIO2.h, ecGPIO2.c** are in `EC\include\`.
3. Create a new project under the directory `EC\lab\`

- Environment: “**LAB_GPIO_DIO_multiLED”**
- Source File: ““**LAB_GPIO_DIO_multiLED.c”**

1. You must modify the `**platformio.ini**` **,** to add new environment.

> You MUST write your name in the top of the source file, inside the comment section.

### Configuration

| Function        | port - pin                 | Configuration                                  |
| --------------- | -------------------------- | ---------------------------------------------- |
| **SW2 on JKIT** | PA_4                       | Digital IN, Pull-Up                            |
| **LED_0~LED_3** | PB_12, PB_13, PB_14, PB_15 | Digital OUT, Pull-UP, Open-Drain, Medium Speed |



### Code

main()

Toggle LED 1, 2, 3, 4 every time I press the button(PA4).

```c
while (1) {

        buttonState = GPIO_read(BUTTON_PIN);
        if(buttonState == LOW && prev_buttonState == HIGH){
            GPIO_write(selectFND[i], LOW);
            i = (i + 1) % 4;
            GPIO_write(selectFND[i], HIGH);
        }
        prev_buttonState = buttonState;
        delay_ms_HSI(30);             					// delay 100 ms

    }
```



### Results

Experiment images and results

> 

Demo video link: https://youtube.com/shorts/saDAE5PMI2I



### Discussion

1. Find out a typical solution for software debouncing and hardware debouncing.

> Hardware debouncing method - LPF(Low Pass Filter).
>
> This can be done by placing a resistor and a capacitor between the switch and the input pin of MCU. Even though debouncing happens, because of the constant time RC, the capacitor takes time to charge and discharge, leading the slower change of voltage value.
>
> There are several methods for software debouncing
>
> 1) Delay - not so desirable. It blocks MCU from doing other necessary works during the delay. Also, if there are faster inputs than the delay, the result may not the way it is intended.
>
> 2) Timer/Counter - Better than delay method. It does not block CPU from working.
>
> 3) Checking a sequence of input samples. - After checking several same input samples, software confirms that it is the input.

2. What method of debouncing did this NUCLEO board use for the push-button(B1)?

> According to PCB schematic file, button B1 is connected to the pin PC13, and the pin has  4.7[kohm] and 29[ohm] resistors, and a 100[mF] capacitor. Therefore, it seems to have a LPF debouncing method with the constant time 0.47[ms] of charging and 0.01[ms] of discharging.





