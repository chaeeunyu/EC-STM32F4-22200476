# Intro.md

**[github repository: EC-STM32F4-22200476](https://github.com/chaeeunyu/EC-STM32F4-22200476)**

Embedded Controller, HGU (26-Fall)

Firmware for STM32F4



folder structure

EC/
?쒋?? intro.md   	
?쒋?? lab
?쒋?? tutorial
?쒋?? docs
??
?쒋?? include
?쒋?? api-*.md      ??moxygen creates it
?쒋?? README.md     ??doxygen + moxygen cooperation
?붴?? Doxyfile
<a id="pinnames"></a>

# Pin Names

Pin name types used by all EC library functions, and conversion from a pin name to its GPIO port and pin number.

## Enumerations

| Name | Description |
|------|-------------|
| [`PortName_t`](#portname_t)  | GPIO port index. Used as the upper nibble of [PinName_t](#pinname_t). |
| [`PinName_t`](#pinname_t)  | Pin name. Encoded as 0xXY: X = port index ([PortName_t](#portname_t)), Y = pin number (0~15). For example, PC_13 = 0x2D (port C = 2, pin 13 = 0xD). Arduino connector names (A0~A5, D0~D15) are aliases of the matching pins. NC means "not connected". |

---

<a id="portname_t"></a>

### PortName_t

```cpp
enum PortName_t
```

Defined in include/ecPinNames.h:40

GPIO port index. Used as the upper nibble of [PinName_t](#pinname_t).

`PortA` · `PortB` · `PortC` · `PortD` · `PortE` · `PortF` · `PortG` · `PortH` · `PortI` · `PortJ` · `PortK`


---

<a id="pinname_t"></a>

### PinName_t

```cpp
enum PinName_t
```

Defined in include/ecPinNames.h:61

Pin name. Encoded as 0xXY: X = port index ([PortName_t](#portname_t)), Y = pin number (0~15). For example, PC_13 = 0x2D (port C = 2, pin 13 = 0xD). Arduino connector names (A0~A5, D0~D15) are aliases of the matching pins. NC means "not connected".

`PA_0` · `PA_1` · `PA_2` · `PA_3` · `PA_4` · `PA_5` · `PA_6` · `PA_7` · `PA_8` · `PA_9` · `PA_10` · `PA_11` · `PA_12` · `PA_13` · `PA_14` · `PA_15` · `PB_0` · `PB_1` · `PB_2` · `PB_3` · `PB_4` · `PB_5` · `PB_6` · `PB_7` · `PB_8` · `PB_9` · `PB_10` · `PB_12` · `PB_13` · `PB_14` · `PB_15` · `PC_0` · `PC_1` · `PC_2` · `PC_3` · `PC_4` · `PC_5` · `PC_6` · `PC_7` · `PC_8` · `PC_9` · `PC_10` · `PC_11` · `PC_12` · `PC_13` · `PC_14` · `PC_15` · `PD_2` · `PH_0` · `PH_1` · `A0` · `A1` · `A2` · `A3` · `A4` · `A5` · `D0` · `D1` · `D2` · `D3` · `D4` · `D5` · `D6` · `D7` · `D8` · `D9` · `D10` · `D11` · `D12` · `D13` · `D14` · `D15` · `NC`

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`ecPinmap`](#ecpinmap)  | Convert a pin name to its GPIO port and pin number. |

---

<a id="ecpinmap"></a>

### ecPinmap

```c
void ecPinmap(PinName_t pinName, GPIO_TypeDef **GPIOx, unsigned int *pin)
{
	// pinName -> port + pin   						e.g. pinName == O O O O   O O O O
	//															     portNum    pinNum 
	
	unsigned int pinNum= pinName & (0x000F);  // pinNum 
	*pin=pinNum;

	unsigned int portNum=(pinName>>4);    //   portNum 

	
	if (portNum==0)
		*GPIOx=GPIOA;
	else if (portNum==1)
		*GPIOx=GPIOB;
	else if (portNum==2)
		*GPIOx=GPIOC;
	else if (portNum==3)
		*GPIOx=GPIOD;
	else if (portNum==7)
		*GPIOx=GPIOH;
	else 
		*GPIOx=GPIOA;
}
```

Defined in include/ecPinNames.h:155

Convert a pin name to its GPIO port and pin number.

> [!NOTE]
> Only ports A, B, C, D, H are mapped. Any other port falls back to GPIOA.


#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](#pinname_t) | Pin name (e.g., PA_5) |
| `GPIOx` | `GPIO_TypeDef **` | GPIO port pointer (e.g., GPIOA) |
| `pin` | `unsigned int *` | Pin number, 0~15 |

<a id="rccclockdriver"></a>

# RCC (Clock) Driver

Select the system clock (HSI 16 MHz or PLL 84 MHz) and enable GPIO port clocks.

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`delay_ms_HSI`](#delay_ms_hsi)  | Approximate blocking delay using a busy-wait loop. |
| `void` | [`RCC_HSI_init`](#rcc_hsi_init)  | Use the internal HSI oscillator (16 MHz) as the system clock. |
| `void` | [`RCC_PLL_init`](#rcc_pll_init)  | Use the PLL (from HSI) as the system clock at 84 MHz. |
| `void` | [`RCC_GPIOA_enable`](#rcc_gpioa_enable)  | Enable the AHB1 clock of GPIO port A. |
| `void` | [`RCC_GPIOB_enable`](#rcc_gpiob_enable)  | Enable the AHB1 clock of GPIO port B. |
| `void` | [`RCC_GPIOC_enable`](#rcc_gpioc_enable)  | Enable the AHB1 clock of GPIO port C. |
| `void` | [`RCC_GPIOD_enable`](#rcc_gpiod_enable)  | Enable the AHB1 clock of GPIO port D. |
| `void` | [`RCC_GPIOE_enable`](#rcc_gpioe_enable)  | Enable the AHB1 clock of GPIO port E. |

---

<a id="delay_ms_hsi"></a>

### delay_ms_HSI

```c
void delay_ms_HSI(uint32_t ms) {
	//uint32_t EC_SYSCLK=16000000;
	volatile uint32_t cnt = ((uint32_t)EC_SYSCLK / (1000UL * 6)) * ms;
	while (cnt > 0) cnt--;
}
```

Defined in include/ecRCC2.h:49

Approximate blocking delay using a busy-wait loop.

> [!NOTE]
> The loop count is estimated from EC_SYSCLK, so the timing is not exact. Use [delay_ms()](api-SysTick.md#delay_ms) (SysTick) when accurate timing is needed.


#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `ms` | `uint32_t` | Delay time [ms] |

---

<a id="rcc_hsi_init"></a>

### RCC_HSI_init

```c
void RCC_HSI_init() {
// Enable High Speed Internal Clock (HSI = 16 MHz)
  //RCC->CR |= ((uint32_t)RCC_CR_HSION);
	RCC->CR |= 0x00000001U;
	
  // wait until HSI is ready
  //while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	while ( (RCC->CR & 0x00000002U) == 0 ) {;}
	
  // Select HSI as system clock source 
  	RCC->CFGR &= (uint32_t)(~RCC_CFGR_SW); 								// not essential
  	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI; 								//00: HSI16 oscillator used as system clock

	// Wait till HSI is used as system clock source
  	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != 0 ) {;}  

	EC_SYSCLK=16000000;	
}
```

Defined in include/ecRCC2.h:55

Use the internal HSI oscillator (16 MHz) as the system clock.

> [!NOTE]
> Sets EC_SYSCLK to 16000000.


---

<a id="rcc_pll_init"></a>

### RCC_PLL_init

```c
void RCC_PLL_init() {	
	// To correctly read data from FLASH memory, the number of wait states (LATENCY)
  // must be correctly programmed according to the frequency of the CPU clock
  // (HCLK) and the supply voltage of the device.		
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |=  FLASH_ACR_LATENCY_2WS;
		
	// Enable the Internal High Speed oscillator (HSI)
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	// Disable PLL for configuration
	RCC->CR    &= ~RCC_CR_PLLON;
	
	// Select clock source to PLL
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC; 		// Set source for PLL: clear bits
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI; // Set source for PLL: 0 =HSI, 1 = HSE
	
	// Make PLL as 84 MHz
	// f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) = 16MHz * 84/8 = 168 MHz
	// f(PLL_R) = f(VCO clock) / PLLP = 168MHz/2 = 84MHz
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | 84U << 6;
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | 8U ; 
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;  // 00: PLLP = 2, 01: PLLP = 4, 10: PLLP = 6, 11: PLLP = 8	
	
	
	// Enable PLL after configuration
	RCC->CR   |= RCC_CR_PLLON; 
	while((RCC->CR & RCC_CR_PLLRDY)>>25 == 0);
	
	// Select PLL as system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	
	// Wait until System Clock has been selected
	while ((RCC->CFGR & RCC_CFGR_SWS) != 8UL);
	
	// The maximum frequency of the AHB and APB2 is 100MHz,
	// The maximum frequency of the APB1 is 50 MHz.
	RCC->CFGR &= ~RCC_CFGR_HPRE;  		// AHB prescaler = 1; SYSCLK not divided (84MHz)
	RCC->CFGR &= ~RCC_CFGR_PPRE1; 		// APB high-speed prescaler (APB1) = 2, HCLK divided by 2 (42MHz)
	RCC->CFGR |=  RCC_CFGR_PPRE1_2;
	RCC->CFGR &= ~RCC_CFGR_PPRE2; 		// APB high-speed prescaler (APB2) = 1, HCLK not divided	(84MHz)
	
	EC_SYSCLK=84000000;
}
```

Defined in include/ecRCC2.h:65

Use the PLL (from HSI) as the system clock at 84 MHz.

PLL: 16 MHz x 84 / 8 / 2 = 84 MHz. Flash latency = 2 wait states. Bus clocks: AHB = 84 MHz, APB1 = 42 MHz, APB2 = 84 MHz. 
> [!NOTE]
> Sets EC_SYSCLK to 84000000. Call before [SysTick_init()](api-SysTick.md#systick_init) and USART setup, since they compute their timing from EC_SYSCLK.


---

<a id="rcc_gpioa_enable"></a>

### RCC_GPIOA_enable

```c
void RCC_GPIOA_enable()
{
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}
```

Defined in include/ecRCC2.h:68

Enable the AHB1 clock of GPIO port A.

---

<a id="rcc_gpiob_enable"></a>

### RCC_GPIOB_enable

```c
void RCC_GPIOB_enable()
{
	// RCC Peripheral Clock Enable Register 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
}
```

Defined in include/ecRCC2.h:71

Enable the AHB1 clock of GPIO port B.

---

<a id="rcc_gpioc_enable"></a>

### RCC_GPIOC_enable

```c
void RCC_GPIOC_enable()
{
	// RCC Peripheral Clock Enable Register
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
}
```

Defined in include/ecRCC2.h:74

Enable the AHB1 clock of GPIO port C.

---

<a id="rcc_gpiod_enable"></a>

### RCC_GPIOD_enable

```c
void RCC_GPIOD_enable()
{
	// RCC Peripheral Clock Enable Register
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
}
```

Defined in include/ecRCC2.h:77

Enable the AHB1 clock of GPIO port D.

---

<a id="rcc_gpioe_enable"></a>

### RCC_GPIOE_enable

```c
void RCC_GPIOE_enable()
{
	// RCC Peripheral Clock Enable Register
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
}
```

Defined in include/ecRCC2.h:80

Enable the AHB1 clock of GPIO port E.

## Variables

| Return | Name | Description |
|--------|------|-------------|
| `volatile int` | [`EC_SYSCLK`](#ec_sysclk)  | Current system clock frequency [Hz]. Updated by [RCC_HSI_init()](#rcc_hsi_init) and [RCC_PLL_init()](#rcc_pll_init). |

---

<a id="ec_sysclk"></a>

### EC_SYSCLK

```cpp
volatile int EC_SYSCLK
```

Defined in include/ecRCC2.h:41

Current system clock frequency [Hz]. Updated by [RCC_HSI_init()](#rcc_hsi_init) and [RCC_PLL_init()](#rcc_pll_init).

<a id="gpiodriver"></a>

# GPIO Driver

Initialize GPIO pins and configure mode, speed, output type, pull-up/down, and read/write pin levels.

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`GPIO_init`](#gpio_init)  | Initialize a GPIO pin: enable the port clock and set the pin mode. |
| `void` | [`GPIO_mode`](#gpio_mode)  | Set the mode of a GPIO pin (MODER register). |
| `void` | [`GPIO_ospeed`](#gpio_ospeed)  | Set the output speed of a GPIO pin (OSPEEDR register). |
| `void` | [`GPIO_otype`](#gpio_otype)  | Set the output type of a GPIO pin (OTYPER register). |
| `void` | [`GPIO_pupd`](#gpio_pupd)  | Set the internal pull-up/pull-down resistor of a GPIO pin (PUPDR register). |
| `void` | [`GPIO_write`](#gpio_write)  | Write a logic level to a GPIO output pin. |
| `uint32_t` | [`GPIO_read`](#gpio_read)  | Read the logic level of a GPIO input pin. |
| `void` | [`FND_init`](#fnd_init)  | Initialize the 8 segment pins (a~g, dp) as outputs. |
| `void` | [`FND_select_init`](#fnd_select_init)  | Initialize the digit-select pins as outputs. |
| `void` | [`FND_display`](#fnd_display)  | Display a number on the currently selected digit. |
| `void` | [`FND_select`](#fnd_select)  | Select which digit of the display is active. |
| `void` | [`FND_select_pupd`](#fnd_select_pupd)  | Set pull-up/pull-down for all digit-select pins. |
| `void` | [`FND_select_otype`](#fnd_select_otype)  | Set output type for all digit-select pins. |
| `void` | [`FND_select_ospeed`](#fnd_select_ospeed)  | Set output speed for all digit-select pins. |

---

<a id="gpio_init"></a>

### GPIO_init

```c
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
	
	
	// You can also make and use a more general function of
	// void RCC_GPIO_enable(GPIO_TypeDef *Port); --> ?쒕룄?대낫湲?

	GPIO_mode(pinName, mode);
}
```

Defined in include/ecGPIO2.h:75

Initialize a GPIO pin: enable the port clock and set the pin mode.

> [!NOTE]
> Call this before any other GPIO_* function on the same pin.


#### Example

```cpp
#define OUTPUT 0x01
GPIO_init(A_13, OUTPUT);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to initialize (e.g., PA_5). See [ecPinNames.h](#ecpinnamesh). |
| `mode` | `uint32_t` | INPUT, OUTPUT, AF, or ANALOG |

---

<a id="gpio_mode"></a>

### GPIO_mode

```c
void GPIO_mode(PinName_t pinName, uint32_t mode){
   GPIO_TypeDef * Port;
   unsigned int pin;
   ecPinmap(pinName,&Port,&pin);

   Port->MODER &= ~(3UL<<(2*pin));     
   Port->MODER |= mode<<(2*pin);    
}
```

Defined in include/ecGPIO2.h:82

Set the mode of a GPIO pin (MODER register).

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin (e.g., PA_5) |
| `mode` | `uint32_t` | INPUT, OUTPUT, AF, or ANALOG |

---

<a id="gpio_ospeed"></a>

### GPIO_ospeed

```c
void GPIO_ospeed(PinName_t pinName, uint32_t speed){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	Port->OSPEEDR &= ~(3UL<<(2*pin));
	Port->OSPEEDR |= speed<<(2*pin);
}
```

Defined in include/ecGPIO2.h:95

Set the output speed of a GPIO pin (OSPEEDR register).

> [!NOTE]
> Effective only in OUTPUT or AF mode.


#### Example

```cpp
#define FASTSPEED 0x02
GPIO_ospeed(A_13, FASTSPEED);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `speed` | `uint32_t` | LOWSPEED, MEDSPEED, FASTSPEED, or HIGHSPEED |

---

<a id="gpio_otype"></a>

### GPIO_otype

```c
void GPIO_otype(PinName_t pinName, uint32_t type){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

   	Port->OTYPER &= ~(1UL<<pin);
	Port->OTYPER |= type<<pin;
}
```

Defined in include/ecGPIO2.h:108

Set the output type of a GPIO pin (OTYPER register).

> [!NOTE]
> Effective only in OUTPUT or AF mode.


#### Example

```cpp
#define PUSH_PULL 0x00
GPIO_ospeed(A_13, PUSH_PULL);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `type` | `uint32_t` | PUSH_PULL or OPEN_DRAIN |

---

<a id="gpio_pupd"></a>

### GPIO_pupd

```c
void GPIO_pupd(PinName_t pinName, uint32_t pupd){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

   	Port->PUPDR &= ~(3UL<<(2*pin));
	Port->PUPDR |= pupd<<(2*pin);
}
```

Defined in include/ecGPIO2.h:115

Set the internal pull-up/pull-down resistor of a GPIO pin (PUPDR register).

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `pupd` | `uint32_t` | NPUPD, PULL_UP, or PULL_DOWN |

---

<a id="gpio_write"></a>

### GPIO_write

```c
void GPIO_write(PinName_t pinName, uint32_t Output){
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	Output == HIGH ? (Port->ODR |= (1UL<<pin)) : (Port->ODR &= ~(1UL<<pin));
}
```

Defined in include/ecGPIO2.h:128

Write a logic level to a GPIO output pin.

> [!NOTE]
> The pin must be initialized in OUTPUT mode.


#### Example

```cpp
GPIO_init(PA_5, OUTPUT);
GPIO_write(PA_5, HIGH);   // LED on
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `Output` | `uint32_t` | HIGH (1) or LOW (0) |

---

<a id="gpio_read"></a>

### GPIO_read

```c
uint32_t GPIO_read(PinName_t pinName){
	GPIO_TypeDef * Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);
	unsigned int value;

   	// value = (Port->IDR) & (1UL<<pin);
	value = ((Port->IDR)>>pin)&(1UL); // ---> more desirable
	return value; 
}
```

Defined in include/ecGPIO2.h:140

Read the logic level of a GPIO input pin.

#### Returns
HIGH (1) or LOW (0) 

> [!NOTE]
> The pin must be initialized in INPUT mode.


#### Example

```cpp
buttonState = GPIO_read(BUTTON_PIN);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |

---

<a id="fnd_init"></a>

### FND_init

```c
void FND_init(PinName_t *pinFND){
	for (int i=0; i<8; i++){
		_fndPins[i] = pinFND[i];
		GPIO_init(_fndPins[i], OUTPUT);
	}
}
```

Defined in include/ecGPIO2.h:150

Initialize the 8 segment pins (a~g, dp) as outputs.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of 8 pins, in the order a, b, c, d, e, f, g, dp |

---

<a id="fnd_select_init"></a>

### FND_select_init

```c
void FND_select_init(PinName_t *selectFND){	
	for(int i=0; i<4; i++){
		_fndSel[i] = selectFND[i];
		GPIO_init(_fndSel[i], OUTPUT);
	}
}
```

Defined in include/ecGPIO2.h:156

Initialize the digit-select pins as outputs.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `selectFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of digit-select pins |

---

<a id="fnd_display"></a>

### FND_display

```c
void FND_display(uint8_t num){
	if(num > 9){
		num = 10;   // error: print dot
	}
	for(int i=0; i<8; i++){
		GPIO_write(_fndPins[i], numberFND[num][i]);
	}
}
```

Defined in include/ecGPIO2.h:168

Display a number on the currently selected digit.

> [!NOTE]
> [FND_init()](#fnd_init) must be called first.


#### Example

```cpp
uint8_t numDisplay1  = 5;
FND_display(numDisplay1);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `num` | `uint8_t` | Number to display, 0~11 (index of numberFND) |

---

<a id="fnd_select"></a>

### FND_select

```c
void FND_select(uint8_t digit){
	for(int i=0; i<4; i++){
		GPIO_write(_fndSel[i], (digit == i) ? HIGH : LOW);
	}	
}
```

Defined in include/ecGPIO2.h:180

Select which digit of the display is active.

> [!NOTE]
> [FND_select_init()](#fnd_select_init) must be called first.


#### Example

```cpp
uint8_t selectFND2  = 1;
FND_select(selectFND2);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `digit` | `uint8_t` | Digit index |

---

<a id="fnd_select_pupd"></a>

### FND_select_pupd

```c
void FND_select_pupd(PinName_t *selectFND, uint32_t pupd){
	for(int i=0; i<4; i++){
		GPIO_pupd(_fndSel[i], pupd);
	}
}
```

Defined in include/ecGPIO2.h:187

Set pull-up/pull-down for all digit-select pins.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `selectFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of digit-select pins |
| `pupd` | `uint32_t` | NPUPD, PULL_UP, or PULL_DOWN |

---

<a id="fnd_select_otype"></a>

### FND_select_otype

```c
void FND_select_otype(PinName_t *selectFND, uint32_t otype){
	for(int i=0; i<4; i++){
		GPIO_otype(_fndSel[i], otype);
	}
}
```

Defined in include/ecGPIO2.h:194

Set output type for all digit-select pins.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `selectFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of digit-select pins |
| `otype` | `uint32_t` | PUSH_PULL or OPEN_DRAIN |

---

<a id="fnd_select_ospeed"></a>

### FND_select_ospeed

```c
void FND_select_ospeed(PinName_t *selectFND, uint32_t ospeed){
	for(int i=0; i<4; i++){
		GPIO_ospeed(_fndSel[i], ospeed);
	}
}
```

Defined in include/ecGPIO2.h:201

Set output speed for all digit-select pins.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `selectFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of digit-select pins |
| `ospeed` | `uint32_t` | LOWSPEED, MEDSPEED, FASTSPEED, or HIGHSPEED |

## Variables

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`numberFND`](#numberfnd)  | Segment patterns for each displayable number. [number][segment a~g, dp] |

---

<a id="numberfnd"></a>

### numberFND

```cpp
int numberFND[12][8]
```

Defined in include/ecGPIO2.h:144

Segment patterns for each displayable number. [number][segment a~g, dp]

<a id="systicktimer"></a>

# SysTick Timer

Generate a 1 ms tick with the SysTick timer and provide millisecond delays.

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`SysTick_init`](#systick_init)  | Configure SysTick to interrupt every 1 ms and enable it (NVIC priority 15). |
| `void` | [`SysTick_Handler`](#systick_handler)  | SysTick interrupt handler. Called by hardware every 1 ms; increments msTicks. |
| `void` | [`SysTick_counter`](#systick_counter)  | Increment msTicks by 1. |
| `void` | [`delay_ms`](#delay_ms)  | Blocking delay in milliseconds, based on msTicks. |
| `void` | [`SysTick_reset`](#systick_reset)  | Reset the SysTick current value register (VAL) to 0. |
| `uint32_t` | [`SysTick_val`](#systick_val)  | Read the SysTick current value register (VAL). |

---

<a id="systick_init"></a>

### SysTick_init

```c
void SysTick_init(void){
	//  SysTick Control and Status Register
	// Disable SysTick IRQ and SysTick Counter
	SysTick->CTRL = 0;											
	
	// Select processor clock
	// 1 = processor clock;  0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

	// SysTick Reload Value Register : (SYSCLK / 1000) - 1  ->  1 ms
	//   HSI 16 MHz :  15999      PLL 84 MHz : 83999 
	SysTick->LOAD = (uint32_t)EC_SYSCLK *1UL/ 1000UL - 1UL;   // 1[ms] = 1/1000[s] 

	// SysTick Current Value Register : Reset counter value to 0
	SysTick->VAL = 0;

	// Enables SysTick exception request
	// 1 = counting down to zero --> SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
		
	// Enable interrupt in NVIC
	NVIC_SetPriority(SysTick_IRQn, 15);		// Set Priority to 1
	NVIC_EnableIRQ(SysTick_IRQn);			// Enable interrupt in NVIC
}
```

Defined in include/ecSysTick2.h:51

Configure SysTick to interrupt every 1 ms and enable it (NVIC priority 15).

> [!NOTE]
> Call after [RCC_HSI_init()](api-RCC.md#rcc_hsi_init) or [RCC_PLL_init()](api-RCC.md#rcc_pll_init), because the reload value is computed from EC_SYSCLK.


---

<a id="systick_handler"></a>

### SysTick_Handler

```c
void SysTick_Handler(void){
	SysTick_counter();	
}
```

Defined in include/ecSysTick2.h:57

SysTick interrupt handler. Called by hardware every 1 ms; increments msTicks.

> [!NOTE]
> Do not call this function directly.


---

<a id="systick_counter"></a>

### SysTick_counter

```c
void SysTick_counter(void){
	msTicks++;
}
```

Defined in include/ecSysTick2.h:63

Increment msTicks by 1.

> [!NOTE]
> Called from [SysTick_Handler()](#systick_handler). Not intended for direct use.


---

<a id="delay_ms"></a>

### delay_ms

```c
void delay_ms (uint32_t msec){
  	uint32_t curTicks;
  	curTicks = msTicks;
	while ((msTicks - curTicks) < msec){;}		// unsigned math: safe across wrap-around
}
```

Defined in include/ecSysTick2.h:72

Blocking delay in milliseconds, based on msTicks.

> [!NOTE]
> [SysTick_init()](#systick_init) must be called first.


> [!WARNING]
> Do not call inside an ISR whose priority is equal to or higher than SysTick (priority 15). msTicks will not increase and the program will hang.


#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `msec` | `uint32_t` | Delay time [ms] |

---

<a id="systick_reset"></a>

### SysTick_reset

```c
void SysTick_reset(void)
{
	SysTick->VAL = 0;
}
```

Defined in include/ecSysTick2.h:78

Reset the SysTick current value register (VAL) to 0.

> [!NOTE]
> This does not reset msTicks.


---

<a id="systick_val"></a>

### SysTick_val

```c
uint32_t SysTick_val(void) {
	return SysTick->VAL;
}
```

Defined in include/ecSysTick2.h:84

Read the SysTick current value register (VAL).

#### Returns
Current counter value. It counts down from the reload value to 0 every 1 ms.

## Variables

| Return | Name | Description |
|--------|------|-------------|
| `volatile uint32_t` | [`msTicks`](#msticks)  | Millisecond counter. Incremented by 1 every 1 ms in [SysTick_Handler()](#systick_handler). |

---

<a id="msticks"></a>

### msTicks

```cpp
volatile uint32_t msTicks
```

Defined in include/ecSysTick2.h:43

Millisecond counter. Incremented by 1 every 1 ms in [SysTick_Handler()](#systick_handler).

<a id="extidriver"></a>

# EXTI Driver

Configure external interrupts on GPIO pins: trigger edge, NVIC priority, enable/disable (mask), and pending-flag handling.

## Macros

[`FALL`](#fall) · [`RISE`](#rise) · [`BOTH`](#both)


---

<a id="fall"></a>

### FALL

```cpp
#define FALL 0
```

Defined in include/ecEXTI2.h:39

---

<a id="rise"></a>

### RISE

```cpp
#define RISE 1
```

Defined in include/ecEXTI2.h:40

---

<a id="both"></a>

### BOTH

```cpp
#define BOTH 2
```

Defined in include/ecEXTI2.h:41

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`EXTI_init`](#exti_init)  | Connect a GPIO pin to its EXTI line, set the trigger edge, and enable the interrupt in NVIC. |
| `void` | [`EXTI_enable`](#exti_enable)  | Enable (unmask) the EXTI interrupt of a pin. |
| `void` | [`EXTI_disable`](#exti_disable)  | Disable (mask) the EXTI interrupt of a pin. The configuration is kept. |
| `uint32_t` | [`is_pending_EXTI`](#is_pending_exti)  | Check whether the EXTI line of a pin has a pending interrupt. |
| `void` | [`clear_pending_EXTI`](#clear_pending_exti)  | Clear the pending flag of the EXTI line of a pin. |

---

<a id="exti_init"></a>

### EXTI_init

```c
void EXTI_init(PinName_t pinName, uint32_t trig_type, uint32_t priority){

	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);
	
	// Enable EXTI Clock with SYSCFG controller clock 	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;		
	

	// Connect EXTI to the GPIO Port_y Pin_x
	// Pin 0~3: EXTICR1 // EXTICR[0]
	// Pin 4~7: EXTICR2 // EXTICR[1]
	// Pin 8~11: EXTICR3 // EXTICR[2]
	// Pin 12~15: EXTICR4 // EXTICR[3]	
	uint32_t EXTICR_port=0;
	if		(Port == GPIOA) EXTICR_port = 0;
	else if	(Port == GPIOB) EXTICR_port = 1;
	else if	(Port == GPIOC) EXTICR_port = 2;
	else if	(Port == GPIOD) EXTICR_port = 3;
	else					EXTICR_port = 4;
	
	SYSCFG->EXTICR[pin/4] &= ~(0xFUL<<((pin%4)*4));			// clear 4 bits
	SYSCFG->EXTICR[pin/4] |= (EXTICR_port<<((pin%4)*4));			// Set 4 bits
	

	// Select  Trigger edge (RISE, FALL, BOTH)
	if (trig_type == FALL) 		EXTI->FTSR |= 1UL << pin;  // Falling trigger enable 
	else if	(trig_type == RISE) EXTI->RTSR |= 1UL << pin;   // Rising trigger enable 
	else if	(trig_type == BOTH) {			// Both falling/rising trigger enable
		EXTI->RTSR |= (1UL << pin); 
		EXTI->FTSR |= (1UL << pin);
	} 
	
	// Enable EXTI, with Not-Masked Interrupt Request 
	// Not-mask (==Enable) EXTIx
 	EXTI->IMR |= 1UL << pin;
	
	
	// NVIC(IRQ) Setting
	uint32_t EXTI_IRQn = 0;
	if (pin < 5) 		EXTI_IRQn = pin + EXTI0_IRQn;
	else if	(pin < 10) 	EXTI_IRQn = EXTI9_5_IRQn;
	else 				EXTI_IRQn = EXTI15_10_IRQn;
		
	NVIC_SetPriority(EXTI_IRQn, priority);	
	NVIC_EnableIRQ(EXTI_IRQn); 				
}
```

Defined in include/ecEXTI2.h:54

Connect a GPIO pin to its EXTI line, set the trigger edge, and enable the interrupt in NVIC.

> [!NOTE]
> Initialize the pin as INPUT with [GPIO_init()](api-GPIO.md#gpio_init) first. The SYSCFG clock is enabled inside.


> [!NOTE]
> Each EXTI line is shared by all ports with the same pin number (e.g., PA_13 and PC_13 cannot both be used).


> [!NOTE]
> Lines 5~9 share EXTI9_5_IRQHandler(), lines 10~15 share EXTI15_10_IRQHandler().


#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to use as interrupt source (e.g., PC_13). The pin number (0~15) selects the EXTI line, the port selects the source. |
| `trig_type` | `uint32_t` | FALL, RISE, or BOTH |
| `priority` | `uint32_t` | NVIC priority, 0 (highest) ~ 15 (lowest) |

---

<a id="exti_enable"></a>

### EXTI_enable

```c
void EXTI_enable(PinName_t pinName) {
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);
	
	EXTI->IMR |= (1UL<<pin);     // not masked (i.e., Interrupt enabled)
}
```

Defined in include/ecEXTI2.h:61

Enable (unmask) the EXTI interrupt of a pin.

> [!NOTE]
> [EXTI_init()](#exti_init) must be called first.


#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin whose EXTI line is enabled |

---

<a id="exti_disable"></a>

### EXTI_disable

```c
void EXTI_disable(PinName_t pinName) {
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	EXTI->IMR &= ~(1UL<<pin);     // masked (i.e., Interrupt disabled)
}
```

Defined in include/ecEXTI2.h:67

Disable (mask) the EXTI interrupt of a pin. The configuration is kept.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin whose EXTI line is disabled |

---

<a id="is_pending_exti"></a>

### is_pending_EXTI

```c
uint32_t is_pending_EXTI(PinName_t pinName) {
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	uint32_t EXTI_PRx = ((EXTI->PR)>>pin) & (1UL);     // read PRx, EXTI pending bit.  Use  (REG>>k & 1) to read.
	return EXTI_PRx; 	// Return only 0 or 1
}
```

Defined in include/ecEXTI2.h:75

Check whether the EXTI line of a pin has a pending interrupt.

#### Returns
1 if pending, 0 otherwise 

> [!NOTE]
> Use inside the IRQ handler to find which line triggered.


#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to check |

---

<a id="clear_pending_exti"></a>

### clear_pending_EXTI

```c
void clear_pending_EXTI(PinName_t pinName){
	// Port, Pin Configuration
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	// clear by writing 1 to the pending bit
	EXTI->PR  = (1 << pin);     
}
```

Defined in include/ecEXTI2.h:82

Clear the pending flag of the EXTI line of a pin.

> [!NOTE]
> Call at the end of the IRQ handler. If not cleared, the handler is re-entered repeatedly.


#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin whose pending flag is cleared |

<a id="steppermotor"></a>

# Stepper Motor

Drive a 4-phase stepper motor in full-step or half-step mode, with speed [rpm], step count, direction, and pause control.

## Classes

| Name | Description |
|------|-------------|
| [`StateStepper_t`](#statestepper_t) | One FSM state: next state for each direction, and the coil output pattern. |

## Macros

| Name | Description |
|------|-------------|
| [`HALF`](#half)  | Half-step mode (8 states) |
| [`FULL`](#full)  | Full-step mode (4 states) |
| [`DIR_CCW`](#dir_ccw)  | Counter-clockwise direction |
| [`DIR_CW`](#dir_cw)  | Clockwise direction |
| [`FULLSTEP_PER_REV`](#fullstep_per_rev)  | Full steps per output-shaft revolution (2048) |
| [`HALFSTEP_PER_REV`](#halfstep_per_rev)  | Half steps per output-shaft revolution (4096) |

---

<a id="half"></a>

### HALF

```cpp
#define HALF 0
```

Defined in include/ecStepper2.h:47

Half-step mode (8 states)

---

<a id="full"></a>

### FULL

```cpp
#define FULL 1
```

Defined in include/ecStepper2.h:48

Full-step mode (4 states)

---

<a id="dir_ccw"></a>

### DIR_CCW

```cpp
#define DIR_CCW 0
```

Defined in include/ecStepper2.h:49

Counter-clockwise direction

---

<a id="dir_cw"></a>

### DIR_CW

```cpp
#define DIR_CW 1
```

Defined in include/ecStepper2.h:50

Clockwise direction

---

<a id="fullstep_per_rev"></a>

### FULLSTEP_PER_REV

```cpp
#define FULLSTEP_PER_REV (64*32)
```

Defined in include/ecStepper2.h:54

Full steps per output-shaft revolution (2048)

---

<a id="halfstep_per_rev"></a>

### HALFSTEP_PER_REV

```cpp
#define HALFSTEP_PER_REV (64*32*2)
```

Defined in include/ecStepper2.h:55

Half steps per output-shaft revolution (4096)

## Enumerations

| Name | Description |
|------|-------------|
| [`StateNum`](#statenum)  | FSM state numbers. Full-step uses S0~S3, half-step uses S0~S7. |

---

<a id="statenum"></a>

### StateNum

```cpp
enum StateNum
```

Defined in include/ecStepper2.h:58

FSM state numbers. Full-step uses S0~S3, half-step uses S0~S7.

`S0` · `S1` · `S2` · `S3` · `S4` · `S5` · `S6` · `S7`

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`stepper_init`](#stepper_init)  | Select the stepping mode and initialize the 4 motor pins as outputs. |
| `void` | [`stepper_speed`](#stepper_speed)  | Set the motor speed by converting [rpm] to the step period [ms]. |
| `void` | [`stepper_step`](#stepper_step)  | Rotate the motor for a given number of steps (blocking). |
| `void` | [`stepper_out`](#stepper_out)  | Write the coil output pattern of an FSM state to the motor pins. |
| `void` | [`stepper_pause`](#stepper_pause)  | Toggle pause/resume of the running [stepper_step()](#stepper_step). |
| `void` | [`stepper_stop`](#stepper_stop)  | stop the running [stepper_step()](#stepper_step). |

---

<a id="stepper_init"></a>

### stepper_init

```c
void stepper_init(uint32_t mode, PinName_t *pinStepper){
	if (mode == HALF) 
		fsm = fsm_Half;
	else
		fsm = fsm_Full;

	//  GPIO Digital Out Initiation
	// For A, B, AN, BN

	for (int i=0; i<4; i++){
		stepperPins[i] = pinStepper[i];
		GPIO_init(stepperPins[i], OUTPUT);
	}
	FND_select_ospeed(stepperPins, FASTSPEED);
}
```

Defined in include/ecStepper2.h:81

Select the stepping mode and initialize the 4 motor pins as outputs.

> [!NOTE]
> [SysTick_init()](api-SysTick.md#systick_init) must also be called before [stepper_step()](#stepper_step), since it uses [delay_ms()](api-SysTick.md#delay_ms).


#### Example

```cpp
PinName_t pinStepper[4] = {PB_10, PB_4, PB_5, PB_3};
stepper_init(HALF, pinStepper);    // half-stepping mode
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `mode` | `uint32_t` | FULL or HALF |
| `pinStepper` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of 4 pins, in the order A, B, A', B' |

---

<a id="stepper_speed"></a>

### stepper_speed

```c
void stepper_speed (uint32_t speedRPM){      // rpm
		_step_delay = 60*1000/(FULLSTEP_PER_REV*speedRPM); 		//in [msec] ---------> MODIFY!!(HALF/FULL)	
		// [usec] can be used for a higher accuracy
		// But needs delay_us() 
}
```

Defined in include/ecStepper2.h:91

Set the motor speed by converting [rpm] to the step period [ms].

#### Example

```cpp
stepper_speed(5);    // 5 rpm -> _step_delay = 60000/(4096*5) = 2 ms (half-stepping)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `speedRPM` | `uint32_t` | Motor speed [rpm] |

---

<a id="stepper_step"></a>

### stepper_step

```c
void stepper_step(uint32_t steps, uint32_t direction){
	// Exit if stepper_init() not called 
	if (fsm == 0) return;
	_steps = steps;

	// run for step size: 
	for (uint32_t i = 0; i < _steps; i++){
		while(pause_flag == 1) {;}
		// Update Present State
		state = fsm[state].next[direction];
		
		// Output of Present State
		stepper_out(state);
	
		// Delay for steppermotor speed
		delay_ms(_step_delay);
	}
	
}
```

Defined in include/ecStepper2.h:105

Rotate the motor for a given number of steps (blocking).

> [!NOTE]
> Returns immediately if [stepper_init()](#stepper_init) was not called. While paused by [stepper_pause()](#stepper_pause), the function waits.


#### Example

```cpp
stepper_step(HALFSTEP_PER_REV*5, DIR_CW);    // 5 revolutions, CW (half-stepping)
stepper_step(STEP_PER_REV*5, DIR_CCW);       // 5 revolutions, CCW (full-stepping)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `steps` | `uint32_t` | Number of steps |
| `direction` | `uint32_t` | DIR_CW or DIR_CCW |

---

<a id="stepper_out"></a>

### stepper_out

```c
void stepper_out(uint32_t state){
	
	// stepperPins[0]=A, stepperPins[1]=B, ...
	// out[0]=A, out[1]=B, ...

	// i want to give '1' to a certain pins(stepperPins[], A B A' B')
	for(uint32_t i=0; i<4; i++){
		GPIO_write(stepperPins[i], fsm[state].out[i]);
	}
}
```

Defined in include/ecStepper2.h:116

Write the coil output pattern of an FSM state to the motor pins.

> [!NOTE]
> Called internally by [stepper_step()](#stepper_step).


#### Example

```cpp
stepper_out(S1);    // full-stepping: A=LOW, B=HIGH, A'=HIGH, B'=LOW
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `state` | `uint32_t` | FSM state number (S0~S3 for FULL, S0~S7 for HALF) |

---

<a id="stepper_pause"></a>

### stepper_pause

```c
void stepper_pause(void) {
    
	pause_flag ^= 1;
}
```

Defined in include/ecStepper2.h:132

Toggle pause/resume of the running [stepper_step()](#stepper_step).

> [!NOTE]
> Despite the name, it does not stop the motor permanently. Each call toggles the pause state. Typically called from an EXTI handler (button).


#### Example

```cpp
void EXTI15_10_IRQHandler(void) {  
 if (is_pending_EXTI(BUTTON_PIN)) {
    stepper_pause();                     // pause or resume
    clear_pending_EXTI(BUTTON_PIN);
 }
}
```

---

<a id="stepper_stop"></a>

### stepper_stop

```c
void stepper_stop(void){
	_steps = 0;    
    
	// All pins DigitalOut '0'
	for(uint32_t i=0; i<4; i++){
		GPIO_write(stepperPins[i], LOW);
	}
}
```

Defined in include/ecStepper2.h:147

stop the running [stepper_step()](#stepper_step).

> [!NOTE]
> It does stop the motor permanently.


#### Example

```cpp
void EXTI15_10_IRQHandler(void) {  
 if (is_pending_EXTI(BUTTON_PIN)) {
    stepper_stop();           
    clear_pending_EXTI(BUTTON_PIN);
 }
}
```


## Class Definitions

<a id="statestepper_t"></a>

### StateStepper_t

```cpp
#include <ecStepper2.h>
```

```cpp
struct StateStepper_t
```

Defined in include/ecStepper2.h:63

One FSM state: next state for each direction, and the coil output pattern.

#### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint32_t` | [`next`](#next)  | Next state. Index = direction (DIR_CCW or DIR_CW) |
| `uint8_t` | [`out`](#out)  | Output levels for coils A, B, A', B' |

---

<a id="next"></a>

##### next

```cpp
uint32_t next[2]
```

Defined in include/ecStepper2.h:64

Next state. Index = direction (DIR_CCW or DIR_CW)

---

<a id="out"></a>

##### out

```cpp
uint8_t out[4]
```

Defined in include/ecStepper2.h:65

Output levels for coils A, B, A', B'

<a id="usartdriver"></a>

# USART Driver

Configure USART1/2/6 (8-N-1, oversampling x16) and send/receive bytes. printf() output is redirected to USART2.

## Macros

| Name | Description |
|------|-------------|
| [`POL`](#pol)  | Polling mode |
| [`INT`](#int)  | Interrupt mode |
| [`BAUD_9600`](#baud_9600)  | 9600 bps |
| [`BAUD_19200`](#baud_19200)  | 19200 bps |
| [`BAUD_38400`](#baud_38400)  | 38400 bps |
| [`BAUD_57600`](#baud_57600)  | 57600 bps |
| [`BAUD_115200`](#baud_115200)  | 115200 bps |
| [`BAUD_921600`](#baud_921600)  | 921600 bps |

---

<a id="pol"></a>

### POL

```cpp
#define POL 0
```

Defined in include/ecUART2.h:46

Polling mode

---

<a id="int"></a>

### INT

```cpp
#define INT 1
```

Defined in include/ecUART2.h:47

Interrupt mode

---

<a id="baud_9600"></a>

### BAUD_9600

```cpp
#define BAUD_9600 9600
```

Defined in include/ecUART2.h:50

9600 bps

---

<a id="baud_19200"></a>

### BAUD_19200

```cpp
#define BAUD_19200 19200
```

Defined in include/ecUART2.h:51

19200 bps

---

<a id="baud_38400"></a>

### BAUD_38400

```cpp
#define BAUD_38400 38400
```

Defined in include/ecUART2.h:52

38400 bps

---

<a id="baud_57600"></a>

### BAUD_57600

```cpp
#define BAUD_57600 57600
```

Defined in include/ecUART2.h:53

57600 bps

---

<a id="baud_115200"></a>

### BAUD_115200

```cpp
#define BAUD_115200 115200
```

Defined in include/ecUART2.h:54

115200 bps

---

<a id="baud_921600"></a>

### BAUD_921600

```cpp
#define BAUD_921600 921600
```

Defined in include/ecUART2.h:55

921600 bps

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`USART_init`](#usart_init)  | Initialize a USART with given TX/RX pins and baud rate, and enable its RX interrupt. |
| `void` | [`USART_baud`](#usart_baud)  | Set the baud rate of a USART (BRR register). |
| `void` | [`USART_pins`](#usart_pins)  | Configure TX/RX pins for a USART: alternate function, push-pull, no pull-up/down, high speed. |
| `void` | [`USART1_baud`](#usart1_baud)  | Set the baud rate of USART1. |
| `void` | [`USART2_baud`](#usart2_baud)  | Set the baud rate of USART2. |
| `void` | [`USART1_init`](#usart1_init)  | Initialize USART1 with default settings: PA_9 (TX), PA_10 (RX), 9600 bps, RX interrupt enabled. |
| `void` | [`USART2_init`](#usart2_init)  | Initialize USART2 with default settings: PA_2 (TX), PA_3 (RX), 9600 bps, RX interrupt enabled. |
| `void` | [`USART1_write`](#usart1_write)  | Send bytes through USART1 (blocking). |
| `void` | [`USART2_write`](#usart2_write)  | Send bytes through USART2 (blocking). |
| `uint8_t` | [`USART1_read`](#usart1_read)  | Receive one byte from USART1 (blocking until data arrives). |
| `uint8_t` | [`USART2_read`](#usart2_read)  | Receive one byte from USART2 (blocking until data arrives). |
| `uint32_t` | [`is_USART1_RXNE`](#is_usart1_rxne)  | Check whether USART1 has received data (RXNE flag). |
| `uint32_t` | [`is_USART2_RXNE`](#is_usart2_rxne)  | Check whether USART2 has received data (RXNE flag). |
| `void` | [`USART_clock_enable`](#usart_clock_enable)  | Enable the peripheral clock of a USART. |
| `void` | [`USART_write`](#usart_write)  | Send bytes through a USART (blocking). Waits for TXE before each byte and TC at the end. |
| `uint8_t` | [`USART_read`](#usart_read)  | Receive one byte from a USART (blocking until RXNE is set). |
| `uint32_t` | [`is_USART_RXNE`](#is_usart_rxne)  | Check whether a USART has received data (RXNE flag). |
| `void` | [`USART_delay`](#usart_delay)  | Approximate busy-wait delay used between transmitted bytes. |

---

<a id="usart_init"></a>

### USART_init

```c
void USART_init(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud){
//1. GPIO Pin for TX and RX
	USART_pins(USARTx, pin_GPIO_TX, pin_GPIO_RX);

//2. USARTx (x=2,1,6) configuration
	USART_clock_enable(USARTx);

	// Disable USARTx.
	USARTx->CR1 &= ~USART_CR1_UE; 	// USART disable

	// No Parity / 8-bit word length / Oversampling x16 / Stop bit =1
	USARTx->CR1 &= ~USART_CR1_PCE;
	USARTx->CR1 &= ~USART_CR1_M;
	USARTx->CR1 &= ~USART_CR1_OVER8;
	USARTx->CR2 &= ~USART_CR2_STOP;

	// Set Baudrate using APB frequency
	// If oversampling by 16, Tx/Rx baud = f_CK / (16*USARTDIV),
	// If oversampling by 8,  Tx/Rx baud = f_CK / (8*USARTDIV)
	USART_baud(USARTx, baud);

	// Enable TX, RX, and USARTx
	USARTx->CR1 |= (USART_CR1_RE | USART_CR1_TE);   	// Transmitter and Receiver enable
	USARTx->CR1 |= USART_CR1_UE; 						          // USART enable

// 3. RX interrupt (data ready to be read)
  // Enable RXNE interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;
  // USART_IRQn
	uint32_t USART_IRQ = USART2_IRQn;	// USART2
	if(USARTx == USART1){
		USART_IRQ = USART1_IRQn;		// USART1
	}
	if(USARTx == USART6){
		USART_IRQ = USART6_IRQn;		// USART6
	}
 
	NVIC_SetPriority(USART_IRQ, 1);
	NVIC_EnableIRQ(USART_IRQ);

}
```

Defined in include/ecUART2.h:90

Initialize a USART with given TX/RX pins and baud rate, and enable its RX interrupt.

Frame format: 8 data bits, no parity, 1 stop bit, oversampling x16. 
> [!NOTE]
> Call after [RCC_PLL_init()](api-RCC.md#rcc_pll_init) or [RCC_HSI_init()](api-RCC.md#rcc_hsi_init), since the baud rate is computed from EC_SYSCLK.


#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |
| `pin_GPIO_TX` | [`PinName_t`](api-PinNames.md#pinname_t) | TX pin (e.g., PA_2) |
| `pin_GPIO_RX` | [`PinName_t`](api-PinNames.md#pinname_t) | RX pin (e.g., PA_3) |
| `baud` | `uint32_t` | Baud rate [bps] (e.g., BAUD_9600) |

---

<a id="usart_baud"></a>

### USART_baud

```c
void USART_baud(USART_TypeDef* USARTx, uint32_t baud){
	// Disable USARTx.
	USARTx->CR1  &= ~USART_CR1_UE; 						// USART disable
	USARTx->BRR = 0;

	// Configure Baud-rate
	float fCK = EC_SYSCLK;                                   // if(USARTx==USART1 || USARTx==USART6), APB2
	                                                         // (84MHz after RCC_PLL_init(), 16MHz after RCC_HSI_init())
	if(USARTx == USART2) fCK =fCK/2;      // APB1
  float USARTDIV = (float) fCK/16.0/baud;

	// Method 1 : build the two BRR fields explicitly
	//   BRR[15:4] = DIV_Mantissa (integer part of USARTDIV)
	//   BRR[3:0]  = DIV_Fraction (counts 1/16 steps -> multiply the fraction by 16)
	uint32_t mantissa = (uint32_t)USARTDIV;
	uint32_t fraction = round((USARTDIV - mantissa) * 16);
	if (fraction > 15) { mantissa += 1; fraction = 0; }
	USARTx->BRR = ((mantissa<<4) | fraction); 

	// Enable TX, RX, and USARTx
	USARTx->CR1 |= USART_CR1_UE;
}
```

Defined in include/ecUART2.h:98

Set the baud rate of a USART (BRR register).

> [!NOTE]
> Uses APB1 (EC_SYSCLK / 2) for USART2 and APB2 (EC_SYSCLK) for USART1/6.


#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |
| `baud` | `uint32_t` | Baud rate [bps] |

---

<a id="usart_pins"></a>

### USART_pins

```c
void USART_pins(USART_TypeDef* USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX){
	GPIO_TypeDef *GPIO_TX;
	unsigned int pinTX;
	ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);

	GPIO_TypeDef *GPIO_RX;
	unsigned int pinRX;
	ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);

	// Alternate Function, Push-Pull, No Pull-up/down, High speed
	// TX settings
	GPIO_init(pin_GPIO_TX, AF);
	GPIO_otype(pin_GPIO_TX, PUSH_PULL);
	GPIO_pupd(pin_GPIO_TX, NPUPD);
	GPIO_ospeed(pin_GPIO_TX, HIGHSPEED);
	
	// RX settings
	GPIO_init(pin_GPIO_RX, AF);
	GPIO_otype(pin_GPIO_RX, PUSH_PULL);
	GPIO_pupd(pin_GPIO_RX, NPUPD);
	GPIO_ospeed(pin_GPIO_RX, HIGHSPEED);

	// Set Alternate Function Register for USARTx
	//   AF7 - USART1, USART2
	//   AF8 - USART6
	uint32_t AF_num = (USARTx == USART6) ? 8 : 7;
	if (pinTX < 8) { GPIO_TX->AFR[0] &= ~(0xFUL << (4*pinTX));     GPIO_TX->AFR[0] |= AF_num << (4*pinTX); }
	else           { GPIO_TX->AFR[1] &= ~(0xFUL << (4*(pinTX-8))); GPIO_TX->AFR[1] |= AF_num << (4*(pinTX-8)); }
	
  	if (pinRX < 8) {
    	GPIO_RX->AFR[0] &= ~(0xFUL << (4*pinRX));  // clear the 4-bit AF field
		GPIO_RX->AFR[0] |= AF_num << (4*pinRX);  // set AF7 (USART1/2) or AF8 (USART6)
  	}
  	else{
    	GPIO_RX->AFR[1] &= ~(0xFUL << (4*(pinRX-8)));  // clear (offset by 8 in AFRH)
		GPIO_RX->AFR[1] |= AF_num << (4*(pinRX-8));  // set AF number
  	}

}
```

Defined in include/ecUART2.h:106

Configure TX/RX pins for a USART: alternate function, push-pull, no pull-up/down, high speed.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 (AF7 for USART1/2, AF8 for USART6) |
| `pin_GPIO_TX` | [`PinName_t`](api-PinNames.md#pinname_t) | TX pin |
| `pin_GPIO_RX` | [`PinName_t`](api-PinNames.md#pinname_t) | RX pin |

---

<a id="usart1_baud"></a>

### USART1_baud

```c
void USART1_baud(uint32_t baud){
	USART_baud(USART1, baud);
}
```

Defined in include/ecUART2.h:113

Set the baud rate of USART1.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `baud` | `uint32_t` | Baud rate [bps] |

---

<a id="usart2_baud"></a>

### USART2_baud

```c
void USART2_baud(uint32_t baud){
	USART_baud(USART2, baud);
}
```

Defined in include/ecUART2.h:119

Set the baud rate of USART2.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `baud` | `uint32_t` | Baud rate [bps] |

---

<a id="usart1_init"></a>

### USART1_init

```c
void USART1_init(void){
	// ********************** USART 1 ***************************
	// PA_9 = USART1_TX (default)	// PB_6  (option)
	// PA_10 = USART1_RX (default)	// PB_3 (option)
	// APB2
	// **********************************************************
	//USART_init(USART1, PA_9, PA_10, 9600);

  ////////////////////////////////////////////
  //  for USART1  ONLY
  ////////////////////////////////////////////
  USART_TypeDef *USARTx= USART1;

//1. GPIO Pin for TX and RX
// Default : PA_9 = USART1_TX, PA_10 = USART1_RX
	PinName_t pin_GPIO_TX  =PA_9;
	PinName_t pin_GPIO_RX  =PA_10;

	GPIO_TypeDef *GPIO_TX;
	unsigned int pinTX;
	ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);
	GPIO_TypeDef *GPIO_RX;
	unsigned int pinRX;
	ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);

	// Alternate Function, Push-Pull, No Pull-up/down, High speed
	GPIO_init(pin_GPIO_TX, AF);
	GPIO_otype(pin_GPIO_TX, PUSH_PULL);
	GPIO_pupd(pin_GPIO_TX, NPUPD);
	GPIO_ospeed(pin_GPIO_TX, HIGHSPEED);

	GPIO_init(pin_GPIO_RX, AF);
	GPIO_otype(pin_GPIO_RX, PUSH_PULL);
	GPIO_pupd(pin_GPIO_RX, NPUPD);
	GPIO_ospeed(pin_GPIO_RX, HIGHSPEED);

 	// Set Alternate Function Register for USARTx
	//   AF7 - USART1, USART2
	//   AF8 - USART6
	uint32_t AF_num = (USARTx == USART6) ? 8 : 7;
	if (pinTX < 8) { GPIO_TX->AFR[0] &= ~(0xFUL << (4*pinTX));     GPIO_TX->AFR[0] |= AF_num << (4*pinTX); }
	else           { GPIO_TX->AFR[1] &= ~(0xFUL << (4*(pinTX-8))); GPIO_TX->AFR[1] |= AF_num << (4*(pinTX-8)); }
	if (pinRX < 8) { GPIO_RX->AFR[0] &= ~(0xFUL << (4*pinRX));     GPIO_RX->AFR[0] |= AF_num << (4*pinRX); }
	else           { GPIO_RX->AFR[1] &= ~(0xFUL << (4*(pinRX-8))); GPIO_RX->AFR[1] |= AF_num << (4*(pinRX-8)); }


//2. USARTx (x=2,1,6) configuration
  // Enable USART peripheral clock
  	//      USART2   = APB1
  	//      USART1,6 = APB2
	USART_clock_enable(USARTx);

  // Disable USARTx.
	USARTx->CR1 &= ~USART_CR1_UE;			// disable while configuring

  // No Parity / 8-bit word length / Oversampling x16
	USARTx->CR1 &= ~USART_CR1_M;			// 8 data bits
	USARTx->CR1 &= ~USART_CR1_PCE;			// no parity
	USARTx->CR1 &= ~USART_CR1_OVER8;		// oversampling by 16
	USARTx->CR2 &= ~USART_CR2_STOP;			// 1 stop bit

  // BAUD RATE (Default: 9600)
  //    Baud rate : BRR[15:4]=mantissa, BRR[3:0]=fraction (1/16 steps)
  //    USARTDIV = f_APB1 / (16 x baud)
  //    USART_baud(USARTx, 9600);
  uint32_t baud=  9600;				// Default baud rate for USART2
  float fCK      = (float)EC_SYSCLK / 2;		// APB1 : 84MHz PLL -> 42MHz
  float USARTDIV = fCK / 16 / baud;
  uint32_t MNT   = (uint32_t)USARTDIV;
  uint32_t FRC   = round((USARTDIV - MNT) * 16);
	if (FRC > 15) { MNT += 1; FRC = 0; }		// carry into the mantissa
	USARTx->BRR = (MNT << 4) | FRC;

  // Enable TX, RX
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);	// transmitter + receiver
  // Enable USART
  	USARTx->CR1 |= USART_CR1_UE;

// 3. RX interrupt (data ready to be read)
  // Enable RXNE interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;
  // USART2 IRQ
	uint32_t USART_IRQ = USART2_IRQn;
	if (USARTx == USART1){
		USART_IRQ = USART1_IRQn;
	}
	NVIC_SetPriority(USART_IRQ, 1);
	NVIC_EnableIRQ(USART_IRQ);
}
```

Defined in include/ecUART2.h:129

Initialize USART1 with default settings: PA_9 (TX), PA_10 (RX), 9600 bps, RX interrupt enabled.

---

<a id="usart2_init"></a>

### USART2_init

```c
void USART2_init(void){
// ********************** USART 2 ***************************
// PA2 = USART2_TX
// PA3 = USART2_RX
// Alternate function(AF7), High Speed, Push pull, Pull up
// **********************************************************
// USART_init(USART2, PA_2, PA_3, 9600);

////////////////////////////////////////////
//  for USART2  ONLY
////////////////////////////////////////////
  USART_TypeDef *USARTx= USART2;

//1. GPIO Pin for TX and RX
// Default : PA_2 = USART2_TX, PA_3 = USART2_RX
	PinName_t pin_GPIO_TX  =PA_2;
	PinName_t pin_GPIO_RX  =PA_3;

	GPIO_TypeDef *GPIO_TX;
	unsigned int pinTX;
	ecPinmap(pin_GPIO_TX, &GPIO_TX, &pinTX);
	GPIO_TypeDef *GPIO_RX;
	unsigned int pinRX;
	ecPinmap(pin_GPIO_RX, &GPIO_RX, &pinRX);

	// Alternate Function, Push-Pull, No Pull-up/down, High speed
	GPIO_init(pin_GPIO_TX, AF);
	GPIO_otype(pin_GPIO_TX, PUSH_PULL);
	GPIO_pupd(pin_GPIO_TX, NPUPD);
	GPIO_ospeed(pin_GPIO_TX, HIGHSPEED);

	GPIO_init(pin_GPIO_RX, AF);
	GPIO_otype(pin_GPIO_RX, PUSH_PULL);
	GPIO_pupd(pin_GPIO_RX, NPUPD);
	GPIO_ospeed(pin_GPIO_RX, HIGHSPEED);

 	// Set Alternate Function Register for USARTx
	//   AF7 - USART1, USART2
	//   AF8 - USART6
	uint32_t AF_num = (USARTx == USART6) ? 8 : 7;
	if (pinTX < 8) { GPIO_TX->AFR[0] &= ~(0xFUL << (4*pinTX));     GPIO_TX->AFR[0] |= AF_num << (4*pinTX); }
	else           { GPIO_TX->AFR[1] &= ~(0xFUL << (4*(pinTX-8))); GPIO_TX->AFR[1] |= AF_num << (4*(pinTX-8)); }
	if (pinRX < 8) { GPIO_RX->AFR[0] &= ~(0xFUL << (4*pinRX));     GPIO_RX->AFR[0] |= AF_num << (4*pinRX); }
	else           { GPIO_RX->AFR[1] &= ~(0xFUL << (4*(pinRX-8))); GPIO_RX->AFR[1] |= AF_num << (4*(pinRX-8)); }


//2. USARTx (x=2,1,6) configuration
  // Enable USART peripheral clock
  	//      USART2   = APB1
  	//      USART1,6 = APB2
	USART_clock_enable(USARTx);

  // Disable USARTx.
	USARTx->CR1 &= ~USART_CR1_UE;			// disable while configuring

  // No Parity / 8-bit word length / Oversampling x16
	USARTx->CR1 &= ~USART_CR1_M;			// 8 data bits
	USARTx->CR1 &= ~USART_CR1_PCE;			// no parity
	USARTx->CR1 &= ~USART_CR1_OVER8;		// oversampling by 16
	USARTx->CR2 &= ~USART_CR2_STOP;			// 1 stop bit

  // BAUD RATE (Default: 9600)
  //    Baud rate : BRR[15:4]=mantissa, BRR[3:0]=fraction (1/16 steps)
  //    USARTDIV = f_APB1 / (16 x baud)
  //    USART_baud(USARTx, 9600);
  uint32_t baud=  9600;				// Default baud rate for USART2
  float fCK      = (float)EC_SYSCLK / 2;		// APB1 : 84MHz PLL -> 42MHz
  float USARTDIV = fCK / 16 / baud;
  uint32_t MNT   = (uint32_t)USARTDIV;
  uint32_t FRC   = round((USARTDIV - MNT) * 16);
	if (FRC > 15) { MNT += 1; FRC = 0; }		// carry into the mantissa
	USARTx->BRR = (MNT << 4) | FRC;

  // Enable TX, RX
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);	// transmitter + receiver
  // Enable USART
  	USARTx->CR1 |= USART_CR1_UE;

// 3. RX interrupt (data ready to be read)
  // Enable RXNE interrupt
	USARTx->CR1 |= USART_CR1_RXNEIE;
  // USART2 IRQ
	uint32_t USART_IRQ = USART2_IRQn;
	if (USARTx == USART1){
		USART_IRQ = USART1_IRQn;
	}
	NVIC_SetPriority(USART_IRQ, 1);
	NVIC_EnableIRQ(USART_IRQ);
}
```

Defined in include/ecUART2.h:135

Initialize USART2 with default settings: PA_2 (TX), PA_3 (RX), 9600 bps, RX interrupt enabled.

> [!NOTE]
> USART2 is connected to the PC through the Nucleo USB (virtual COM port).


---

<a id="usart1_write"></a>

### USART1_write

```c
void USART1_write(uint8_t* buffer, uint32_t nBytes){
	USART_write(USART1, buffer, nBytes);
}
```

Defined in include/ecUART2.h:144

Send bytes through USART1 (blocking).

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `buffer` | `uint8_t *` | Data to send |
| `nBytes` | `uint32_t` | Number of bytes |

---

<a id="usart2_write"></a>

### USART2_write

```c
void USART2_write(uint8_t* buffer, uint32_t nBytes){
	USART_write(USART2, buffer, nBytes);
}
```

Defined in include/ecUART2.h:151

Send bytes through USART2 (blocking).

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `buffer` | `uint8_t *` | Data to send |
| `nBytes` | `uint32_t` | Number of bytes |

---

<a id="usart1_read"></a>

### USART1_read

```c
uint8_t USART1_read(void){
	return USART_read(USART1);
}
```

Defined in include/ecUART2.h:157

Receive one byte from USART1 (blocking until data arrives).

#### Returns
Received byte

---

<a id="usart2_read"></a>

### USART2_read

```c
uint8_t USART2_read(void){
	return USART_read(USART2);
}
```

Defined in include/ecUART2.h:163

Receive one byte from USART2 (blocking until data arrives).

#### Returns
Received byte

---

<a id="is_usart1_rxne"></a>

### is_USART1_RXNE

```c
uint32_t is_USART1_RXNE(void){
	return is_USART_RXNE(USART1);
}
```

Defined in include/ecUART2.h:171

Check whether USART1 has received data (RXNE flag).

#### Returns
1 if data is ready to read, 0 otherwise

---

<a id="is_usart2_rxne"></a>

### is_USART2_RXNE

```c
uint32_t is_USART2_RXNE(void){
	return is_USART_RXNE(USART2);
}
```

Defined in include/ecUART2.h:177

Check whether USART2 has received data (RXNE flag).

#### Returns
1 if data is ready to read, 0 otherwise

---

<a id="usart_clock_enable"></a>

### USART_clock_enable

```c
void USART_clock_enable(USART_TypeDef* USARTx){
	if (USARTx == USART1)
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 	// Enabl lock: AHB clock = 84MHz)
	else if(USARTx == USART2)
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  	// Enable USART 2 clock (APB1 clock: AHB clock/2 = 42MHz)
	else
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;  	// Enable USART 6 clock (APB2 clock: AHB clock = 84MHz)
}
```

Defined in include/ecUART2.h:185

Enable the peripheral clock of a USART.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1 / USART6 (APB2) or USART2 (APB1) |

---

<a id="usart_write"></a>

### USART_write

```c
void USART_write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	// TXE is set by hardware when the content of the TDR
	// register has been transferred into the shift register.
	int i;
	for (i = 0; i < nBytes; i++) {
		// wait until TXE (TX empty) bit is set
		while (!(USARTx->SR & USART_SR_TXE));
		// Writing USART_DR automatically clears the TXE flag
		USARTx->DR = buffer[i] & 0xFF;
		USART_delay(300);
	}
	// wait until TC bit is set
	while (!(USARTx->SR & USART_SR_TC));
	// TC bit clear
	USARTx->SR &= ~USART_SR_TC;
}
```

Defined in include/ecUART2.h:193

Send bytes through a USART (blocking). Waits for TXE before each byte and TC at the end.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |
| `buffer` | `uint8_t *` | Data to send |
| `nBytes` | `uint32_t` | Number of bytes |

---

<a id="usart_read"></a>

### USART_read

```c
uint8_t USART_read(USART_TypeDef * USARTx){
	// Wait until RXNE (RX not empty) bit is set by HW -->Read to read
	while (is_USART_RXNE(USARTx) == 0);
	// Reading USART_DR automatically clears the RXNE flag
	return ((uint8_t)(USARTx->DR & 0xFF));
}
```

Defined in include/ecUART2.h:200

Receive one byte from a USART (blocking until RXNE is set).

#### Returns
Received byte

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |

---

<a id="is_usart_rxne"></a>

### is_USART_RXNE

```c
uint32_t is_USART_RXNE(USART_TypeDef * USARTx){
	return (USARTx->SR & USART_SR_RXNE) != 0;
}
```

Defined in include/ecUART2.h:207

Check whether a USART has received data (RXNE flag).

#### Returns
1 if data is ready to read, 0 otherwise

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |

---

<a id="usart_delay"></a>

### USART_delay

```c
void USART_delay(uint32_t us) {
   uint32_t time = 100*us/7;
   while(--time);
}
```

Defined in include/ecUART2.h:213

Approximate busy-wait delay used between transmitted bytes.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `us` | `uint32_t` | Delay time [us] (approximate, not calibrated) |

<a id="mcuinitialization"></a>

# MCU Initialization

One-call default setup of the MCU.

