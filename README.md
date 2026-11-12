# Intro.md

**[github repository: EC-STM32F4-22200476](https://github.com/chaeeunyu/EC-STM32F4-22200476)**

Embedded Controller, HGU (26-Fall)

Firmware for STM32F4



folder structure

```text
EC/
├── intro.md   	
├── lab
├── tutorial
├── docs
⋮
├── include
├── api-*.md      ← moxygen creates it
├── README.md     ← doxygen + moxygen cooperation
└── Doxyfile
```

<a id="pinnames"></a>

# Pin Names

Pin name types used by all EC library functions, and conversion from a pin name to its GPIO port and pin number.

## Enumerations

| Name | Description |
|------|-------------|
| [`PortName_t`](#portname_t)  | GPIO port index. Used as the upper nibble of [PinName_t](#pinname_t). |
| [`PinName_t`](#pinname_t)  | Pin name. Encoded as 0xXY: X = port index ([PortName_t](#portname_t)), Y = pin number (0\~15). For example, PC_13 = 0x2D (port C = 2, pin 13 = 0xD). Arduino connector names (A0\~A5, D0\~D15) are aliases of the matching pins. NC means "not connected". |

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

Pin name. Encoded as 0xXY: X = port index ([PortName_t](#portname_t)), Y = pin number (0\~15). For example, PC_13 = 0x2D (port C = 2, pin 13 = 0xD). Arduino connector names (A0\~A5, D0\~D15) are aliases of the matching pins. NC means "not connected".

`PA_0` · `PA_1` · `PA_2` · `PA_3` · `PA_4` · `PA_5` · `PA_6` · `PA_7` · `PA_8` · `PA_9` · `PA_10` · `PA_11` · `PA_12` · `PA_13` · `PA_14` · `PA_15` · `PB_0` · `PB_1` · `PB_2` · `PB_3` · `PB_4` · `PB_5` · `PB_6` · `PB_7` · `PB_8` · `PB_9` · `PB_10` · `PB_12` · `PB_13` · `PB_14` · `PB_15` · `PC_0` · `PC_1` · `PC_2` · `PC_3` · `PC_4` · `PC_5` · `PC_6` · `PC_7` · `PC_8` · `PC_9` · `PC_10` · `PC_11` · `PC_12` · `PC_13` · `PC_14` · `PC_15` · `PD_2` · `PH_0` · `PH_1` · `A0` · `A1` · `A2` · `A3` · `A4` · `A5` · `D0` · `D1` · `D2` · `D3` · `D4` · `D5` · `D6` · `D7` · `D8` · `D9` · `D10` · `D11` · `D12` · `D13` · `D14` · `D15` · `NC`

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`ecPinmap`](#ecpinmap)  | Convert a pin name to its GPIO port and pin number. |

---

<a id="ecpinmap"></a>

### ecPinmap

```cpp
void ecPinmap(PinName_t pinName, GPIO_TypeDef ** GPIOx, unsigned int * pin)
```

Defined in include/ecPinNames.h:162

Convert a pin name to its GPIO port and pin number.

> [!NOTE]
> Only ports A, B, C, D, H are mapped. Any other port falls back to GPIOA.


#### Example

```cpp
GPIO_TypeDef *port;
unsigned int pin;
ecPinmap(PA_5, &port, &pin);     // port = GPIOA, pin = 5
port->ODR |= (1UL << pin);       // PA_5 = HIGH
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](#pinname_t) | Pin name (e.g., PA_5) |
| `GPIOx` | `GPIO_TypeDef **` | GPIO port pointer (e.g., GPIOA) |
| `pin` | `unsigned int *` | Pin number, 0\~15 |

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

```cpp
void delay_ms_HSI(uint32_t ms)
```

Defined in include/ecRCC2.h:60

Approximate blocking delay using a busy-wait loop.

> [!NOTE]
> The loop count is estimated from EC_SYSCLK, so the timing is not exact. Use [delay_ms()](api-SysTick.md#delay_ms) (SysTick) when accurate timing is needed.


#### Example

```cpp
RCC_HSI_init();
GPIO_init(PA_5, OUTPUT);
while (1) {
    GPIO_write(PA_5, HIGH);
    delay_ms_HSI(500);           // about 500 ms
    GPIO_write(PA_5, LOW);
    delay_ms_HSI(500);
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `ms` | `uint32_t` | Delay time [ms] |

---

<a id="rcc_hsi_init"></a>

### RCC_HSI_init

```cpp
void RCC_HSI_init(void)
```

Defined in include/ecRCC2.h:71

Use the internal HSI oscillator (16 MHz) as the system clock.

> [!NOTE]
> Sets EC_SYSCLK to 16000000.


#### Example

```cpp
RCC_HSI_init();      // SYSCLK = 16 MHz, EC_SYSCLK = 16000000
SysTick_init();      // 1 ms tick computed from 16 MHz
```

---

<a id="rcc_pll_init"></a>

### RCC_PLL_init

```cpp
void RCC_PLL_init(void)
```

Defined in include/ecRCC2.h:87

Use the PLL (from HSI) as the system clock at 84 MHz.

PLL: 16 MHz x 84 / 8 / 2 = 84 MHz. Flash latency = 2 wait states. Bus clocks: AHB = 84 MHz, APB1 = 42 MHz, APB2 = 84 MHz. 
> [!NOTE]
> Sets EC_SYSCLK to 84000000. Call before [SysTick_init()](api-SysTick.md#systick_init) and USART setup, since they compute their timing from EC_SYSCLK.


#### Example

```cpp
RCC_PLL_init();      // SYSCLK = 84 MHz, EC_SYSCLK = 84000000
SysTick_init();      // 1 ms tick computed from 84 MHz
USART2_init();       // baud rate computed from APB1 = 42 MHz
```

---

<a id="rcc_gpioa_enable"></a>

### RCC_GPIOA_enable

```cpp
void RCC_GPIOA_enable(void)
```

Defined in include/ecRCC2.h:99

Enable the AHB1 clock of GPIO port A.

#### Example

```cpp
RCC_GPIOA_enable();                   // required before accessing GPIOA registers
GPIOA->MODER &= ~(3UL << (2*5));
GPIOA->MODER |=  (1UL << (2*5));      // PA_5 = OUTPUT
// GPIO_init() calls this automatically for port A pins.
```

---

<a id="rcc_gpiob_enable"></a>

### RCC_GPIOB_enable

```cpp
void RCC_GPIOB_enable(void)
```

Defined in include/ecRCC2.h:110

Enable the AHB1 clock of GPIO port B.

#### Example

```cpp
RCC_GPIOB_enable();                   // required before accessing GPIOB registers
GPIOB->MODER &= ~(3UL << (2*10));     // PB_10 = INPUT
// GPIO_init() calls this automatically for port B pins.
```

---

<a id="rcc_gpioc_enable"></a>

### RCC_GPIOC_enable

```cpp
void RCC_GPIOC_enable(void)
```

Defined in include/ecRCC2.h:121

Enable the AHB1 clock of GPIO port C.

#### Example

```cpp
RCC_GPIOC_enable();                   // required before accessing GPIOC registers
GPIOC->MODER &= ~(3UL << (2*13));     // PC_13 (user button) = INPUT
// GPIO_init() calls this automatically for port C pins.
```

---

<a id="rcc_gpiod_enable"></a>

### RCC_GPIOD_enable

```cpp
void RCC_GPIOD_enable(void)
```

Defined in include/ecRCC2.h:133

Enable the AHB1 clock of GPIO port D.

#### Example

```cpp
RCC_GPIOD_enable();                   // required before accessing GPIOD registers
GPIOD->MODER &= ~(3UL << (2*2));
GPIOD->MODER |=  (1UL << (2*2));      // PD_2 = OUTPUT
// GPIO_init() calls this automatically for port D pins.
```

---

<a id="rcc_gpioe_enable"></a>

### RCC_GPIOE_enable

```cpp
void RCC_GPIOE_enable(void)
```

Defined in include/ecRCC2.h:143

Enable the AHB1 clock of GPIO port E.

#### Example

```cpp
RCC_GPIOE_enable();                   // required before accessing GPIOE registers
// Port E pins are not routed on the Nucleo-F411RE headers.
```

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
| `void` | [`FND_init`](#fnd_init)  | Initialize the 8 segment pins (a\~g, dp) as outputs. |
| `void` | [`FND_select_init`](#fnd_select_init)  | Initialize the digit-select pins as outputs. |
| `void` | [`FND_display`](#fnd_display)  | Display a number on the currently selected digit. |
| `void` | [`FND_select`](#fnd_select)  | Select which digit of the display is active. |
| `void` | [`GPIO_pupd_arr`](#gpio_pupd_arr)  | Set pull-up/pull-down for array pins. |
| `void` | [`GPIO_otype_arr`](#gpio_otype_arr)  | Set output type for array pins. |
| `void` | [`GPIO_ospeed_arr`](#gpio_ospeed_arr)  | Set output speed for array pins. |
| `void` | [`led_toggle`](#led_toggle)  | Toggling LED lights. |

---

<a id="gpio_init"></a>

### GPIO_init

```cpp
void GPIO_init(PinName_t pinName, uint32_t mode)
```

Defined in include/ecGPIO2.h:75

Initialize a GPIO pin: enable the port clock and set the pin mode.

> [!NOTE]
> Call this before any other GPIO_* function on the same pin.


#### Example

```cpp
GPIO_init(PA_5, OUTPUT);     // LED (LD2)
GPIO_init(PC_13, INPUT);     // user button (B1)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to initialize (e.g., PA_5). See [ecPinNames.h](#ecpinnamesh). |
| `mode` | `uint32_t` | INPUT, OUTPUT, AF, or ANALOG |

---

<a id="gpio_mode"></a>

### GPIO_mode

```cpp
void GPIO_mode(PinName_t pinName, uint32_t mode)
```

Defined in include/ecGPIO2.h:87

Set the mode of a GPIO pin (MODER register).

#### Example

```cpp
GPIO_mode(PA_5, OUTPUT);     // LED
GPIO_mode(PA_2, AF);         // USART2 TX
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin (e.g., PA_5) |
| `mode` | `uint32_t` | INPUT, OUTPUT, AF, or ANALOG |

---

<a id="gpio_ospeed"></a>

### GPIO_ospeed

```cpp
void GPIO_ospeed(PinName_t pinName, uint32_t speed)
```

Defined in include/ecGPIO2.h:100

Set the output speed of a GPIO pin (OSPEEDR register).

> [!NOTE]
> Effective only in OUTPUT or AF mode.


#### Example

```cpp
GPIO_init(PA_5, OUTPUT);
GPIO_ospeed(PA_5, FASTSPEED);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `speed` | `uint32_t` | LOWSPEED, MEDSPEED, FASTSPEED, or HIGHSPEED |

---

<a id="gpio_otype"></a>

### GPIO_otype

```cpp
void GPIO_otype(PinName_t pinName, uint32_t type)
```

Defined in include/ecGPIO2.h:113

Set the output type of a GPIO pin (OTYPER register).

> [!NOTE]
> Effective only in OUTPUT or AF mode.


#### Example

```cpp
GPIO_init(PA_5, OUTPUT);
GPIO_otype(PA_5, PUSH_PULL);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `type` | `uint32_t` | PUSH_PULL or OPEN_DRAIN |

---

<a id="gpio_pupd"></a>

### GPIO_pupd

```cpp
void GPIO_pupd(PinName_t pinName, uint32_t pupd)
```

Defined in include/ecGPIO2.h:125

Set the internal pull-up/pull-down resistor of a GPIO pin (PUPDR register).

#### Example

```cpp
GPIO_init(PA_0, INPUT);
GPIO_pupd(PA_0, PULL_UP);    // switch connected between PA_0 and GND
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `pupd` | `uint32_t` | NPUPD, PULL_UP, or PULL_DOWN |

---

<a id="gpio_write"></a>

### GPIO_write

```cpp
void GPIO_write(PinName_t pinName, uint32_t Output)
```

Defined in include/ecGPIO2.h:138

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

```cpp
uint32_t GPIO_read(PinName_t pinName)
```

Defined in include/ecGPIO2.h:150

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

```cpp
void FND_init(PinName_t * pinFND)
```

Defined in include/ecGPIO2.h:165

Initialize the 8 segment pins (a\~g, dp) as outputs.

#### Example

```cpp
PinName_t pinsFND[8] = {PA_7, PB_6, PC_7, PA_9, PA_8, PB_10, PA_6, PA_10};  // a, b, c, d, e, f, g, dp
FND_init(pinsFND);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of 8 pins, in the order a, b, c, d, e, f, g, dp |

---

<a id="fnd_select_init"></a>

### FND_select_init

```cpp
void FND_select_init(PinName_t * selectFND)
```

Defined in include/ecGPIO2.h:176

Initialize the digit-select pins as outputs.

#### Example

```cpp
PinName_t pinsSel[4] = {PC_0, PC_1, PC_2, PC_3};    // digit 0, 1, 2, 3
FND_select_init(pinsSel);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `selectFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of digit-select pins |

---

<a id="fnd_display"></a>

### FND_display

```cpp
void FND_display(uint8_t num)
```

Defined in include/ecGPIO2.h:188

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
| `num` | `uint8_t` | Number to display, 0\~11 (index of numberFND) |

---

<a id="fnd_select"></a>

### FND_select

```cpp
void FND_select(uint8_t digit)
```

Defined in include/ecGPIO2.h:200

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

<a id="gpio_pupd_arr"></a>

### GPIO_pupd_arr

```cpp
void GPIO_pupd_arr(PinName_t * pinArr, int n, uint32_t pupd)
```

Defined in include/ecGPIO2.h:215

Set pull-up/pull-down for array pins.

> [!NOTE]
> GPIO_*_init() must be called first.


#### Example

```cpp
PinName_t pinsSel[4] = {PC_0, PC_1, PC_2, PC_3};
FND_select_init(pinsSel);
GPIO_pupd_arr(pinsSel, 4, NPUPD);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinArr` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of pins (as a pointer) |
| `n` | `int` | The size of the array (# of elements) |
| `pupd` | `uint32_t` | NPUPD, PULL_UP, or PULL_DOWN |

---

<a id="gpio_otype_arr"></a>

### GPIO_otype_arr

```cpp
void GPIO_otype_arr(PinName_t * pinArr, int n, uint32_t otype)
```

Defined in include/ecGPIO2.h:230

Set output type for array pins.

> [!NOTE]
> GPIO_*_init() must be called first.


#### Example

```cpp
PinName_t pinsSel[4] = {PC_0, PC_1, PC_2, PC_3};
FND_select_init(pinsSel);
GPIO_otype_arr(pinsSel, 4, PUSH_PULL);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinArr` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of pins (as a pointer) |
| `n` | `int` | The size of the array (# of elements) |
| `otype` | `uint32_t` | PUSH_PULL or OPEN_DRAIN |

---

<a id="gpio_ospeed_arr"></a>

### GPIO_ospeed_arr

```cpp
void GPIO_ospeed_arr(PinName_t * pinArr, int n, uint32_t ospeed)
```

Defined in include/ecGPIO2.h:245

Set output speed for array pins.

> [!NOTE]
> GPIO_*_init() must be called first.


#### Example

```cpp
PinName_t pinsSel[4] = {PC_0, PC_1, PC_2, PC_3};
FND_select_init(pinsSel);
GPIO_ospeed_arr(pinsSel, 4, MEDSPEED);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinArr` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of pins (as a pointer) |
| `n` | `int` | The size of the array (# of elements) |
| `ospeed` | `uint32_t` | LOWSPEED, MEDSPEED, FASTSPEED, or HIGHSPEED |

---

<a id="led_toggle"></a>

### led_toggle

```cpp
void led_toggle(PinName_t pinName)
```

Defined in include/ecGPIO2.h:255

Toggling LED lights.

#### Example

```cpp
led_toggle(LD2_PIN);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |

## Variables

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`numberFND`](#numberfnd)  | Segment patterns for each displayable number. [number][segment a\~g, dp] |

---

<a id="numberfnd"></a>

### numberFND

```cpp
int numberFND[12][8]
```

Defined in include/ecGPIO2.h:154

Segment patterns for each displayable number. [number][segment a\~g, dp]

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

```cpp
void SysTick_init(void)
```

Defined in include/ecSysTick2.h:56

Configure SysTick to interrupt every 1 ms and enable it (NVIC priority 15).

> [!NOTE]
> Call after [RCC_HSI_init()](api-RCC.md#rcc_hsi_init) or [RCC_PLL_init()](api-RCC.md#rcc_pll_init), because the reload value is computed from EC_SYSCLK.


#### Example

```cpp
RCC_PLL_init();
SysTick_init();      // msTicks increases by 1 every 1 ms
```

---

<a id="systick_handler"></a>

### SysTick_Handler

```cpp
void SysTick_Handler(void)
```

Defined in include/ecSysTick2.h:62

SysTick interrupt handler. Called by hardware every 1 ms; increments msTicks.

> [!NOTE]
> Do not call this function directly.


---

<a id="systick_counter"></a>

### SysTick_counter

```cpp
void SysTick_counter(void)
```

Defined in include/ecSysTick2.h:68

Increment msTicks by 1.

> [!NOTE]
> Called from [SysTick_Handler()](#systick_handler). Not intended for direct use.


---

<a id="delay_ms"></a>

### delay_ms

```cpp
void delay_ms(uint32_t msec)
```

Defined in include/ecSysTick2.h:88

Blocking delay in milliseconds, based on msTicks.

> [!NOTE]
> [SysTick_init()](#systick_init) must be called first.


> [!WARNING]
> Do not call inside an ISR whose priority is equal to or higher than SysTick (priority 15). msTicks will not increase and the program will hang.


#### Example

```cpp
SysTick_init();
GPIO_init(PA_5, OUTPUT);
while (1) {
    GPIO_write(PA_5, HIGH);
    delay_ms(500);
    GPIO_write(PA_5, LOW);
    delay_ms(500);
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `msec` | `uint32_t` | Delay time [ms] |

---

<a id="systick_reset"></a>

### SysTick_reset

```cpp
void SysTick_reset(void)
```

Defined in include/ecSysTick2.h:99

Reset the SysTick current value register (VAL) to 0.

> [!NOTE]
> This does not reset msTicks.


#### Example

```cpp
SysTick_reset();                  // restart the current 1 ms period from the reload value
uint32_t start = SysTick_val();
```

---

<a id="systick_val"></a>

### SysTick_val

```cpp
uint32_t SysTick_val(void)
```

Defined in include/ecSysTick2.h:112

Read the SysTick current value register (VAL).

#### Returns
Current counter value. It counts down from the reload value to 0 every 1 ms. 

#### Example

```cpp
uint32_t start = SysTick_val();
GPIO_write(PA_5, HIGH);
uint32_t end = SysTick_val();
uint32_t cycles = start - end;    // down-counter; valid if no reload happened in between
```

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

```cpp
void EXTI_init(PinName_t pinName, uint32_t trig_type, uint32_t priority)
```

Defined in include/ecEXTI2.h:61

Connect a GPIO pin to its EXTI line, set the trigger edge, and enable the interrupt in NVIC.

> [!NOTE]
> Initialize the pin as INPUT with [GPIO_init()](api-GPIO.md#gpio_init) first. The SYSCFG clock is enabled inside.


> [!NOTE]
> Each EXTI line is shared by all ports with the same pin number (e.g., PA_13 and PC_13 cannot both be used).


> [!NOTE]
> Lines 5\~9 share EXTI9_5_IRQHandler(), lines 10\~15 share EXTI15_10_IRQHandler().


#### Example

```cpp
GPIO_init(PC_13, INPUT);
GPIO_pupd(PC_13, PULL_UP);
EXTI_init(PC_13, FALL, 0);        // button press = falling edge, priority 0 (highest)
// Handler: EXTI15_10_IRQHandler() (line 13)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to use as interrupt source (e.g., PC_13). The pin number (0\~15) selects the EXTI line, the port selects the source. |
| `trig_type` | `uint32_t` | FALL, RISE, or BOTH |
| `priority` | `uint32_t` | NVIC priority, 0 (highest) \~ 15 (lowest) |

---

<a id="exti_enable"></a>

### EXTI_enable

```cpp
void EXTI_enable(PinName_t pinName)
```

Defined in include/ecEXTI2.h:74

Enable (unmask) the EXTI interrupt of a pin.

> [!NOTE]
> [EXTI_init()](#exti_init) must be called first.


#### Example

```cpp
EXTI_disable(PC_13);              // ignore the button while the motor runs
stepper_step(HALFSTEP_PER_REV, DIR_CW);
EXTI_enable(PC_13);               // accept the button again
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin whose EXTI line is enabled |

---

<a id="exti_disable"></a>

### EXTI_disable

```cpp
void EXTI_disable(PinName_t pinName)
```

Defined in include/ecEXTI2.h:86

Disable (mask) the EXTI interrupt of a pin. The configuration is kept.

#### Example

```cpp
EXTI_disable(PC_13);              // ignore the button while the motor runs
stepper_step(HALFSTEP_PER_REV, DIR_CW);
EXTI_enable(PC_13);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin whose EXTI line is disabled |

---

<a id="is_pending_exti"></a>

### is_pending_EXTI

```cpp
uint32_t is_pending_EXTI(PinName_t pinName)
```

Defined in include/ecEXTI2.h:106

Check whether the EXTI line of a pin has a pending interrupt.

#### Returns
1 if pending, 0 otherwise 

> [!NOTE]
> Use inside the IRQ handler to find which line triggered.


#### Example

```cpp
volatile uint32_t ledState = LOW;

void EXTI15_10_IRQHandler(void) {
    if (is_pending_EXTI(PC_13)) {     // was it line 13?
        ledState = !ledState;
        GPIO_write(PA_5, ledState);
        clear_pending_EXTI(PC_13);
    }
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to check |

---

<a id="clear_pending_exti"></a>

### clear_pending_EXTI

```cpp
void clear_pending_EXTI(PinName_t pinName)
```

Defined in include/ecEXTI2.h:125

Clear the pending flag of the EXTI line of a pin.

> [!NOTE]
> Call at the end of the IRQ handler. If not cleared, the handler is re-entered repeatedly.


#### Example

```cpp
volatile uint32_t ledState = LOW;

void EXTI15_10_IRQHandler(void) {
    if (is_pending_EXTI(PC_13)) {
        ledState = !ledState;
        GPIO_write(PA_5, ledState);
        clear_pending_EXTI(PC_13);    // without this, the handler is re-entered forever
    }
}
```

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
| [`StateNum`](#statenum)  | FSM state numbers. Full-step uses S0\~S3, half-step uses S0\~S7. |

---

<a id="statenum"></a>

### StateNum

```cpp
enum StateNum
```

Defined in include/ecStepper2.h:58

FSM state numbers. Full-step uses S0\~S3, half-step uses S0\~S7.

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

```cpp
void stepper_init(uint32_t mode, PinName_t * pinStepper)
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

```cpp
void stepper_speed(uint32_t speedRPM, uint32_t step_per_rev)
```

Defined in include/ecStepper2.h:92

Set the motor speed by converting [rpm] to the step period [ms].

#### Example

```cpp
stepper_speed(5, FULLSTEP_PER_REV);    // 5 rpm -> _step_delay = 60000/(2048*5) = 5 ms (full-stepping)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `speedRPM` | `uint32_t` | Motor speed [rpm] |
| `step_per_rev` | `uint32_t` | # of steps per revolution |

---

<a id="stepper_step"></a>

### stepper_step

```cpp
void stepper_step(uint32_t steps, uint32_t direction)
```

Defined in include/ecStepper2.h:106

Rotate the motor for a given number of steps (blocking).

> [!NOTE]
> Returns immediately if [stepper_init()](#stepper_init) was not called. While paused by [stepper_pause()](#stepper_pause), the function waits.


#### Example

```cpp
stepper_step(HALFSTEP_PER_REV*5, DIR_CW);    // 5 revolutions, CW (half-stepping)
stepper_step(FULLSTEP_PER_REV*5, DIR_CCW);   // 5 revolutions, CCW (full-stepping)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `steps` | `uint32_t` | Number of steps |
| `direction` | `uint32_t` | DIR_CW or DIR_CCW |

---

<a id="stepper_out"></a>

### stepper_out

```cpp
void stepper_out(uint32_t state)
```

Defined in include/ecStepper2.h:117

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
| `state` | `uint32_t` | FSM state number (S0\~S3 for FULL, S0\~S7 for HALF) |

---

<a id="stepper_pause"></a>

### stepper_pause

```cpp
void stepper_pause(void)
```

Defined in include/ecStepper2.h:132

Toggle pause/resume of the running [stepper_step()](#stepper_step).

> [!NOTE]
> Each call toggles the pause state. Typically called from an EXTI handler (button).


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

```cpp
void stepper_stop(void)
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

```cpp
void USART_init(USART_TypeDef * USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud)
```

Defined in include/ecUART2.h:96

Initialize a USART with given TX/RX pins and baud rate, and enable its RX interrupt.

Frame format: 8 data bits, no parity, 1 stop bit, oversampling x16. 
> [!NOTE]
> Call after [RCC_PLL_init()](api-RCC.md#rcc_pll_init) or [RCC_HSI_init()](api-RCC.md#rcc_hsi_init), since the baud rate is computed from EC_SYSCLK.


#### Example

```cpp
RCC_PLL_init();
USART_init(USART2, PA_2, PA_3, BAUD_9600);     // PC (USB virtual COM)
USART_init(USART1, PA_9, PA_10, BAUD_9600);    // e.g., Bluetooth module
```

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

```cpp
void USART_baud(USART_TypeDef * USARTx, uint32_t baud)
```

Defined in include/ecUART2.h:108

Set the baud rate of a USART (BRR register).

> [!NOTE]
> Uses APB1 (EC_SYSCLK / 2) for USART2 and APB2 (EC_SYSCLK) for USART1/6.


#### Example

```cpp
USART_baud(USART2, BAUD_115200);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |
| `baud` | `uint32_t` | Baud rate [bps] |

---

<a id="usart_pins"></a>

### USART_pins

```cpp
void USART_pins(USART_TypeDef * USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX)
```

Defined in include/ecUART2.h:120

Configure TX/RX pins for a USART: alternate function, push-pull, no pull-up/down, high speed.

#### Example

```cpp
USART_pins(USART6, PA_11, PA_12);   // TX = PA_11, RX = PA_12, AF8
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 (AF7 for USART1/2, AF8 for USART6) |
| `pin_GPIO_TX` | [`PinName_t`](api-PinNames.md#pinname_t) | TX pin |
| `pin_GPIO_RX` | [`PinName_t`](api-PinNames.md#pinname_t) | RX pin |

---

<a id="usart1_baud"></a>

### USART1_baud

```cpp
void USART1_baud(uint32_t baud)
```

Defined in include/ecUART2.h:132

Set the baud rate of USART1.

#### Example

```cpp
USART1_init();               // 9600 bps by default
USART1_baud(BAUD_38400);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `baud` | `uint32_t` | Baud rate [bps] |

---

<a id="usart2_baud"></a>

### USART2_baud

```cpp
void USART2_baud(uint32_t baud)
```

Defined in include/ecUART2.h:143

Set the baud rate of USART2.

#### Example

```cpp
USART2_init();               // 9600 bps by default
USART2_baud(BAUD_115200);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `baud` | `uint32_t` | Baud rate [bps] |

---

<a id="usart1_init"></a>

### USART1_init

```cpp
void USART1_init(void)
```

Defined in include/ecUART2.h:158

Initialize USART1 with default settings: PA_9 (TX), PA_10 (RX), 9600 bps, RX interrupt enabled.

#### Example

```cpp
RCC_PLL_init();
USART1_init();               // PA_9 = TX, PA_10 = RX, 9600 bps
```

---

<a id="usart2_init"></a>

### USART2_init

```cpp
void USART2_init(void)
```

Defined in include/ecUART2.h:170

Initialize USART2 with default settings: PA_2 (TX), PA_3 (RX), 9600 bps, RX interrupt enabled.

> [!NOTE]
> USART2 is connected to the PC through the Nucleo USB (virtual COM port).


#### Example

```cpp
RCC_PLL_init();
USART2_init();               // PA_2 = TX, PA_3 = RX, 9600 bps
printf("Hello Nucleo\r\n");  // printf() is sent to USART2
```

---

<a id="usart1_write"></a>

### USART1_write

```cpp
void USART1_write(uint8_t * buffer, uint32_t nBytes)
```

Defined in include/ecUART2.h:184

Send bytes through USART1 (blocking).

#### Example

```cpp
uint8_t msg[] = "AT\r\n";
USART1_write(msg, sizeof(msg) - 1);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `buffer` | `uint8_t *` | Data to send |
| `nBytes` | `uint32_t` | Number of bytes |

---

<a id="usart2_write"></a>

### USART2_write

```cpp
void USART2_write(uint8_t * buffer, uint32_t nBytes)
```

Defined in include/ecUART2.h:196

Send bytes through USART2 (blocking).

#### Example

```cpp
uint8_t msg[] = "Hello\r\n";
USART2_write(msg, sizeof(msg) - 1);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `buffer` | `uint8_t *` | Data to send |
| `nBytes` | `uint32_t` | Number of bytes |

---

<a id="usart1_read"></a>

### USART1_read

```cpp
uint8_t USART1_read(void)
```

Defined in include/ecUART2.h:211

Receive one byte from USART1 (blocking until data arrives).

#### Returns
Received byte 

#### Example

```cpp
void USART1_IRQHandler(void) {
    if (is_USART1_RXNE()) {
        uint8_t c = USART1_read();
        USART2_write(&c, 1);         // forward to PC
    }
}
```

---

<a id="usart2_read"></a>

### USART2_read

```cpp
uint8_t USART2_read(void)
```

Defined in include/ecUART2.h:226

Receive one byte from USART2 (blocking until data arrives).

#### Returns
Received byte 

#### Example

```cpp
void USART2_IRQHandler(void) {
    if (is_USART2_RXNE()) {
        uint8_t c = USART2_read();
        USART2_write(&c, 1);         // echo back to PC
    }
}
```

---

<a id="is_usart1_rxne"></a>

### is_USART1_RXNE

```cpp
uint32_t is_USART1_RXNE(void)
```

Defined in include/ecUART2.h:243

Check whether USART1 has received data (RXNE flag).

#### Returns
1 if data is ready to read, 0 otherwise 

#### Example

```cpp
void USART1_IRQHandler(void) {
    if (is_USART1_RXNE()) {          // data received?
        uint8_t c = USART1_read();
        USART2_write(&c, 1);
    }
}
```

---

<a id="is_usart2_rxne"></a>

### is_USART2_RXNE

```cpp
uint32_t is_USART2_RXNE(void)
```

Defined in include/ecUART2.h:258

Check whether USART2 has received data (RXNE flag).

#### Returns
1 if data is ready to read, 0 otherwise 

#### Example

```cpp
void USART2_IRQHandler(void) {
    if (is_USART2_RXNE()) {          // data received?
        uint8_t cmd = USART2_read();
        if (cmd == 'L') GPIO_write(PA_5, HIGH);
    }
}
```

---

<a id="usart_clock_enable"></a>

### USART_clock_enable

```cpp
void USART_clock_enable(USART_TypeDef * USARTx)
```

Defined in include/ecUART2.h:270

Enable the peripheral clock of a USART.

#### Example

```cpp
USART_clock_enable(USART6);      // APB2
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1 / USART6 (APB2) or USART2 (APB1) |

---

<a id="usart_write"></a>

### USART_write

```cpp
void USART_write(USART_TypeDef * USARTx, uint8_t * buffer, uint32_t nBytes)
```

Defined in include/ecUART2.h:283

Send bytes through a USART (blocking). Waits for TXE before each byte and TC at the end.

#### Example

```cpp
uint8_t msg[] = "Hello\r\n";
USART_write(USART6, msg, sizeof(msg) - 1);
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |
| `buffer` | `uint8_t *` | Data to send |
| `nBytes` | `uint32_t` | Number of bytes |

---

<a id="usart_read"></a>

### USART_read

```cpp
uint8_t USART_read(USART_TypeDef * USARTx)
```

Defined in include/ecUART2.h:299

Receive one byte from a USART (blocking until RXNE is set).

#### Returns
Received byte 

#### Example

```cpp
void USART6_IRQHandler(void) {
    if (is_USART_RXNE(USART6)) {
        uint8_t c = USART_read(USART6);
        USART_write(USART2, &c, 1);   // forward to PC
    }
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |

---

<a id="is_usart_rxne"></a>

### is_USART_RXNE

```cpp
uint32_t is_USART_RXNE(USART_TypeDef * USARTx)
```

Defined in include/ecUART2.h:315

Check whether a USART has received data (RXNE flag).

#### Returns
1 if data is ready to read, 0 otherwise 

#### Example

```cpp
void USART6_IRQHandler(void) {
    if (is_USART_RXNE(USART6)) {     // data received?
        uint8_t c = USART_read(USART6);
        USART_write(USART2, &c, 1);
    }
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |

---

<a id="usart_delay"></a>

### USART_delay

```cpp
void USART_delay(uint32_t us)
```

Defined in include/ecUART2.h:325

Approximate busy-wait delay used between transmitted bytes.

#### Example

```cpp
USART_delay(300);                // about 300 us (not calibrated)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `us` | `uint32_t` | Delay time [us] (approximate, not calibrated) |

<a id="mcuinitialization"></a>

# MCU Initialization

One-call default setup of the MCU.

<a id="inputcapturedriver"></a>

# Input Capture Driver

Capture the timer counter value at a pin edge to measure pulse width, period, or time interval (e.g., ultrasonic echo).

Supported pins use the same TIMx_CHn map as the PWM driver (PA0, PA1, PA5, PA6, PA8\~PA10, PA15, PB3\~PB10, PC6\~PC9).

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`ICAP_init`](#icap_init)  | Initialize input capture on a pin with its own channel (ICn = TIn) and a 1 usec tick. |
| `void` | [`ICAP_ICn_init`](#icap_icn_init)  | Initialize input capture on a pin with a user-selected IC channel and a 1 usec tick. |
| `void` | [`ICAP_tick`](#icap_tick)  | Set the counter tick of the capture timer in usec. ARR is kept at maximum. |
| `uint32_t` | [`ICAP_read`](#icap_read)  | Read the captured counter value of an IC channel. |
| `uint32_t` | [`is_CCIF`](#is_ccif)  | Check the capture/compare interrupt flag (CCnIF) of an IC channel. |
| `void` | [`clear_CCIF`](#clear_ccif)  | Clear the capture/compare interrupt flag (CCnIF) of an IC channel. |
| `void` | [`ICAP_pinmap`](#icap_pinmap)  | Internal: find the timer and channel (TIn) connected to a pin. |
| `void` | [`ICAP_filter`](#icap_filter)  | Internal: set the digital input filter (ICnF) of one IC channel. |
| `void` | [`ICAP_ICn_select`](#icap_icn_select)  | Internal: connect a timer input (TIn) to an input capture channel (ICn) via CCnS. |

---

<a id="icap_init"></a>

### ICAP_init

```cpp
void ICAP_init(PinName_t pinName, uint32_t edge_type)
```

Defined in include/ecCAP2.h:85

Initialize input capture on a pin with its own channel (ICn = TIn) and a 1 usec tick.

> [!NOTE]
> Enables the capture interrupt (CCnIE), the update interrupt (UIE), and NVIC (priority 2). Handle both in TIMx_IRQHandler(): UIF for overflow counting, CCnIF for captures.


> [!NOTE]
> ARR is set to the maximum (0xFFFF, or 0xFFFFFFFF for TIM2/TIM5).


#### Example

```cpp
ICAP_init(PB_6, IC_RISE);          // TIM4_CH1 → IC1, capture rising edge
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Capture input pin (e.g., PB_6 → TIM4_CH1 → IC1) |
| `edge_type` | `uint32_t` | IC_RISE, IC_FALL, or IC_BOTH |

---

<a id="icap_icn_init"></a>

### ICAP_ICn_init

```cpp
void ICAP_ICn_init(PinName_t pinName, uint32_t edge_type, uint32_t ICn)
```

Defined in include/ecCAP2.h:103

Initialize input capture on a pin with a user-selected IC channel and a 1 usec tick.

> [!NOTE]
> Use two ICs on one pin to capture both edges into separate registers (e.g., rising → IC1, falling → IC2) for pulse-width measurement.


> [!NOTE]
> Call order: [ICAP_init()](#icap_init) → [ICAP_ICn_init()](#icap_icn_init) → [ICAP_tick()](#icap_tick) (last).


#### Example

```cpp
ICAP_init(PB_6, IC_RISE);                 // TI1 → IC1 : rising edge
ICAP_ICn_init(PB_6, IC_FALL, IC_2);       // TI1 → IC2 : falling edge
ICAP_tick(PB_6, 10);                      // 1 tick = 10 usec  <- call last
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Capture input pin (TIn is decided by the pin) |
| `edge_type` | `uint32_t` | IC_RISE, IC_FALL, or IC_BOTH |
| `ICn` | `uint32_t` | IC_1 \~ IC_4. Either the same channel (ICn = TIn) or its pair (IC1↔TI2, IC2↔TI1, IC3↔TI4, IC4↔TI3) |

---

<a id="icap_tick"></a>

### ICAP_tick

```cpp
void ICAP_tick(PinName_t pinName, uint32_t usec)
```

Defined in include/ecCAP2.h:118

Set the counter tick of the capture timer in usec. ARR is kept at maximum.

> [!NOTE]
> Call this LAST: [ICAP_ICn_init()](#icap_icn_init) resets the tick to 1 usec.


> [!NOTE]
> Measured time [usec] = (captured ticks) * usec.


#### Example

```cpp
ICAP_init(PB_6, IC_RISE);
ICAP_ICn_init(PB_6, IC_FALL, IC_2);
ICAP_tick(PB_6, 10);               // 1 tick = 10 usec, 16-bit overflow every 655.36 msec
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Capture input pin (selects the timer) |
| `usec` | `uint32_t` | One tick [usec]. PSC is 16-bit → 1 \~ 780 usec at 84 MHz (0 is changed to 1) |

---

<a id="icap_read"></a>

### ICAP_read

```cpp
uint32_t ICAP_read(PinName_t pinName, uint32_t ICn)
```

Defined in include/ecCAP2.h:149

Read the captured counter value of an IC channel.

#### Returns
Captured CCRn value [ticks] 

> [!NOTE]
> Reading CCRn also clears its CCnIF flag in hardware.


#### Example

```cpp
volatile uint32_t ovf_cnt = 0;
volatile float time1 = 0, time2 = 0, timeInterval = 0;   // [usec]

void TIM4_IRQHandler(void) {
    if (is_UIF(TIM4)) {                        // counter overflow
        ovf_cnt++;
        clear_UIF(TIM4);
    }
    if (is_CCIF(TIM4, IC_1)) {                 // rising edge
        time1 = ICAP_read(PB_6, IC_1);
        clear_CCIF(TIM4, IC_1);
    }
    else if (is_CCIF(TIM4, IC_2)) {            // falling edge
        time2 = ICAP_read(PB_6, IC_2);
        timeInterval = ((time2 - time1) + ovf_cnt * (TIM4->ARR + 1)) * 10;  // 10 usec tick
        ovf_cnt = 0;
        clear_CCIF(TIM4, IC_2);
    }
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Capture input pin (selects the timer) |
| `ICn` | `uint32_t` | IC_1 \~ IC_4 |

---

<a id="is_ccif"></a>

### is_CCIF

```cpp
uint32_t is_CCIF(TIM_TypeDef * TIMx, uint32_t ICn)
```

Defined in include/ecCAP2.h:171

Check the capture/compare interrupt flag (CCnIF) of an IC channel.

#### Returns
1 if a capture occurred, 0 otherwise 

> [!NOTE]
> Use inside TIMx_IRQHandler() to find which channel captured.


#### Example

```cpp
void TIM4_IRQHandler(void) {
    if (is_CCIF(TIM4, IC_1)) {
        time1 = ICAP_read(PB_6, IC_1);
        clear_CCIF(TIM4, IC_1);
    }
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Capture timer |
| `ICn` | `uint32_t` | IC_1 \~ IC_4 |

---

<a id="clear_ccif"></a>

### clear_CCIF

```cpp
void clear_CCIF(TIM_TypeDef * TIMx, uint32_t ICn)
```

Defined in include/ecCAP2.h:186

Clear the capture/compare interrupt flag (CCnIF) of an IC channel.

> [!NOTE]
> Call at the end of each capture branch in the IRQ handler.


#### Example

```cpp
if (is_CCIF(TIM4, IC_2)) {
    time2 = ICAP_read(PB_6, IC_2);
    clear_CCIF(TIM4, IC_2);
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Capture timer |
| `ICn` | `uint32_t` | IC_1 \~ IC_4 |

---

<a id="icap_pinmap"></a>

### ICAP_pinmap

```cpp
void ICAP_pinmap(PinName_t pinName, TIM_TypeDef ** TIMx, int * chN)
```

Defined in include/ecCAP2.h:204

Internal: find the timer and channel (TIn) connected to a pin.

#### Example

```cpp
TIM_TypeDef *TIMx;
int TIn;
ICAP_pinmap(PB_6, &TIMx, &TIn);    // TIMx = TIM4, TIn = 1
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to look up |
| `TIMx` | `TIM_TypeDef **` | Timer of the pin |
| `chN` | `int *` | Timer input channel TIn, 1 \~ 4 |

---

<a id="icap_filter"></a>

### ICAP_filter

```cpp
void ICAP_filter(TIM_TypeDef * TIMx, uint32_t ICn, uint16_t filterOpt)
```

Defined in include/ecCAP2.h:218

Internal: set the digital input filter (ICnF) of one IC channel.

> [!NOTE]
> Only the given channel is written, so output channels of the same timer are not affected (ICxF bits share the position of OCxM bits in CCMRx).


#### Example

```cpp
ICAP_filter(TIM4, IC_1, IC_FILTER_0);  // remove the default 8-sample filter on IC1
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Capture timer |
| `ICn` | `uint32_t` | IC_1 \~ IC_4 |
| `filterOpt` | `uint16_t` | IC_FILTER_0 \~ IC_FILTER_3 |

---

<a id="icap_icn_select"></a>

### ICAP_ICn_select

```cpp
void ICAP_ICn_select(TIM_TypeDef * TIMx, uint32_t TIn, uint32_t ICn)
```

Defined in include/ecCAP2.h:232

Internal: connect a timer input (TIn) to an input capture channel (ICn) via CCnS.

> [!NOTE]
> ICn = TIn → CCnS = 01 (direct). ICn ≠ TIn → CCnS = 10 (paired input: IC1↔TI2, IC2↔TI1, IC3↔TI4, IC4↔TI3).


#### Example

```cpp
ICAP_ICn_select(TIM4, 1, IC_2);    // TI1 → IC2 (CC2S = 10)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Capture timer |
| `TIn` | `uint32_t` | Timer input channel of the pin, 1 \~ 4 |
| `ICn` | `uint32_t` | IC_1 \~ IC_4 |

<a id="pwmdriver"></a>

# PWM Driver

Generate PWM on a GPIO pin using a timer output-compare channel: period, duty ratio, and pulse width.

Supported pins (TIMx_CHn): 
Port  |Pin → Timer / Channel
--------- | ---------
GPIOA  |PA0 TIM2_CH1, PA1 TIM2_CH2, PA5 TIM2_CH1, PA6 TIM3_CH1, PA8 TIM1_CH1, PA9 TIM1_CH2, PA10 TIM1_CH3, PA15 TIM2_CH1
GPIOB  |PB3 TIM2_CH2, PB4 TIM3_CH1, PB5 TIM3_CH2, PB6 TIM4_CH1, PB7 TIM4_CH2, PB8 TIM4_CH3, PB9 TIM4_CH4, PB10 TIM2_CH3
GPIOC  |PC6 TIM3_CH1, PC7 TIM3_CH2, PC8 TIM3_CH3, PC9 TIM3_CH4

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`PWM_init_config`](#pwm_init_config)  | Internal helper: configure the pin as AF, set up the timer, and start PWM. |
| `void` | [`PWM_duty`](#pwm_duty)  | Set the duty ratio of a PWM pin. |
| `void` | [`PWM_init`](#pwm_init)  | Initialize PWM with the period in msec (low frequency, 1 kHz or below). |
| `void` | [`PWM_init_us`](#pwm_init_us)  | Initialize PWM with the period in usec (high frequency, 1 kHz \~ 100 kHz). |
| `void` | [`PWM_pulsewidth`](#pwm_pulsewidth)  | Set the pulse width (high time) of a PWM pin in msec. |
| `void` | [`PWM_pulsewidth_us`](#pwm_pulsewidth_us)  | Set the pulse width (high time) of a PWM pin in usec. |
| `void` | [`PWM_mode_config`](#pwm_mode_config)  | Internal: set the output-compare mode of a timer channel to PWM. |
| `void` | [`PWM_pinmap`](#pwm_pinmap)  | Internal: find the timer and channel connected to a pin. |

---

<a id="pwm_init_config"></a>

### PWM_init_config

```cpp
void PWM_init_config(PinName_t pinName, uint32_t usec)
```

Defined in include/ecPWM2.h:71

Internal helper: configure the pin as AF, set up the timer, and start PWM.

> [!NOTE]
> Used by [PWM_init()](#pwm_init) and [PWM_init_us()](#pwm_init_us). Call those instead.


> [!NOTE]
> Sets AF1 (TIM1, TIM2) or AF2 (TIM3, TIM4), PWM mode 1, active high, 50% duty.


> [!NOTE]
> TIM2/TIM5 or period <= 65,536 usec → [TIM_init_us()](api-TIM.md#tim_init_us) (1 usec tick). Otherwise (16-bit timer, long period) → [TIM_init_ms()](api-TIM.md#tim_init_ms) (0.1 msec tick).


> [!NOTE]
> Returns without doing anything if the pin has no PWM channel.


#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | PWM output pin (see the pin table above) |
| `usec` | `uint32_t` | PWM period [usec] |

---

<a id="pwm_duty"></a>

### PWM_duty

```cpp
void PWM_duty(PinName_t pinName, float duty)
```

Defined in include/ecPWM2.h:84

Set the duty ratio of a PWM pin.

> [!NOTE]
> CCR = (ARR + 1) * duty. Assumes PWM mode 1.


#### Example

```cpp
PWM_init(PA_5, 1);                 // 1 msec period (1 kHz)
PWM_duty(PA_5, 0.25f);             // 25% duty → LED brightness 25%
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | PWM output pin |
| `duty` | `float` | Duty ratio, 0.0 \~ 1.0 (clamped to this range) |

---

<a id="pwm_init"></a>

### PWM_init

```cpp
void PWM_init(PinName_t pinName, uint32_t msec)
```

Defined in include/ecPWM2.h:105

Initialize PWM with the period in msec (low frequency, 1 kHz or below).

> [!NOTE]
> Default: PWM mode 1, active high, 50% duty.


#### Example

```cpp
PWM_init(PA_1, 20);                // RC servo: 20 msec period (50 Hz)
PWM_pulsewidth_us(PA_1, 1500);     // 1.5 msec pulse → servo center
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | PWM output pin (see the pin table above) |
| `msec` | `uint32_t` | PWM period [msec]. 0 is changed to 1. TIM2/TIM5 (32-bit): 1 \~ 4,294,967 msec, TIM1/TIM3/TIM4 (16-bit): 1 \~ 6,553 msec |

---

<a id="pwm_init_us"></a>

### PWM_init_us

```cpp
void PWM_init_us(PinName_t pinName, uint32_t usec)
```

Defined in include/ecPWM2.h:119

Initialize PWM with the period in usec (high frequency, 1 kHz \~ 100 kHz).

> [!NOTE]
> Default: PWM mode 1, active high, 50% duty. 1 tick = 1 usec, so the duty resolution is 1/usec (e.g., 10 usec period → 10 steps).


#### Example

```cpp
PWM_init_us(PB_6, 50);             // DC motor: 50 usec period (20 kHz)
PWM_duty(PB_6, 0.6f);              // 60% speed
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | PWM output pin (see the pin table above) |
| `usec` | `uint32_t` | PWM period [usec]. Minimum 10 usec (100 kHz); smaller values are changed to 10. |

---

<a id="pwm_pulsewidth"></a>

### PWM_pulsewidth

```cpp
void PWM_pulsewidth(PinName_t pinName, uint32_t pulse_width_ms)
```

Defined in include/ecPWM2.h:137

Set the pulse width (high time) of a PWM pin in msec.

> [!NOTE]
> Converted to a duty ratio using the current PSC and ARR, then [PWM_duty()](#pwm_duty) is called.


#### Example

```cpp
PWM_init(PA_0, 100);               // 100 msec period
PWM_pulsewidth(PA_0, 30);          // 30 msec high → 30% duty
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | PWM output pin |
| `pulse_width_ms` | `uint32_t` | Pulse width [msec], must be <= PWM period |

---

<a id="pwm_pulsewidth_us"></a>

### PWM_pulsewidth_us

```cpp
void PWM_pulsewidth_us(PinName_t pinName, uint32_t pulse_width_us)
```

Defined in include/ecPWM2.h:151

Set the pulse width (high time) of a PWM pin in usec.

> [!NOTE]
> Converted to a duty ratio using the current PSC and ARR, then [PWM_duty()](#pwm_duty) is called.


#### Example

```cpp
PWM_init(PA_1, 20);                // RC servo: 20 msec period
PWM_pulsewidth_us(PA_1, 500);      // 0.5 msec → 0 deg
PWM_pulsewidth_us(PA_1, 2500);     // 2.5 msec → 180 deg
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | PWM output pin |
| `pulse_width_us` | `uint32_t` | Pulse width [usec], must be <= PWM period |

---

<a id="pwm_mode_config"></a>

### PWM_mode_config

```cpp
void PWM_mode_config(TIM_TypeDef * TIMx, int chN, uint32_t mode)
```

Defined in include/ecPWM2.h:171

Internal: set the output-compare mode of a timer channel to PWM.

> [!NOTE]
> Also enables CCR preload, sets active-high polarity, CCR = ARR/2 (50% duty), and enables the channel output (CCxE).


#### Example

```cpp
PWM_init(PA_5, 1);
PWM_mode_config(TIM2, 1, EC_PWM_MODE2);   // invert the output on TIM2_CH1
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer |
| `chN` | `int` | Channel number, 1 \~ 4 |
| `mode` | `uint32_t` | EC_PWM_MODE1 (OCxM = 110) or EC_PWM_MODE2 (OCxM = 111) |

---

<a id="pwm_pinmap"></a>

### PWM_pinmap

```cpp
void PWM_pinmap(PinName_t pinName, TIM_TypeDef ** TIMx, int * chN)
```

Defined in include/ecPWM2.h:186

Internal: find the timer and channel connected to a pin.

> [!NOTE]
> Only TIM1\~TIM4 are mapped. See the pin table above.


#### Example

```cpp
TIM_TypeDef *TIMx = NULL;
int ch = 0;
PWM_pinmap(PB_6, &TIMx, &ch);      // TIMx = TIM4, ch = 1
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to look up |
| `TIMx` | `TIM_TypeDef **` | Timer of the pin (unchanged if the pin has no PWM channel) |
| `chN` | `int *` | Channel number 1 \~ 4 |

<a id="timerdriver"></a>

# Timer Driver

Configure timer counters (PSC, ARR, direction), the update interrupt (UI), and the NVIC for TIM1\~TIM5, TIM9\~TIM11.

## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`TIM_UI_init_ms`](#tim_ui_init_ms)  | Initialize a timer with a period in msec and enable its update interrupt in NVIC. |
| `void` | [`TIM_UI_init_us`](#tim_ui_init_us)  | Initialize a timer with a period in usec and enable its update interrupt in NVIC. |
| `void` | [`TIM_UI_enable`](#tim_ui_enable)  | Enable the update interrupt (UIE) of a timer. |
| `void` | [`TIM_UI_disable`](#tim_ui_disable)  | Disable the update interrupt (UIE) of a timer. The counter keeps running. |
| `uint32_t` | [`is_UIF`](#is_uif)  | Check the update interrupt flag (UIF) of a timer. |
| `void` | [`clear_UIF`](#clear_uif)  | Clear the update interrupt flag (UIF) of a timer. |
| `void` | [`TIM_init_ms`](#tim_init_ms)  | Enable the timer clock, set the period in msec, set up-counting, and start the counter. |
| `void` | [`TIM_init_us`](#tim_init_us)  | Enable the timer clock, set the period in usec, set up-counting, and start the counter. |
| `void` | [`TIM_period_ms`](#tim_period_ms)  | Set the counter period in msec (PSC and ARR only). |
| `void` | [`TIM_period_us`](#tim_period_us)  | Set the counter period in usec (PSC and ARR only). |
| `void` | [`TIM_period_tick`](#tim_period_tick)  | Set both the tick size and the counter period in usec. |
| `uint32_t` | [`get_TIM_IRQn`](#get_tim_irqn)  | Get the NVIC IRQ number of a timer. |
| `void` | [`TIM_clock_enable`](#tim_clock_enable)  | Enable the RCC clock of a timer. |

---

<a id="tim_ui_init_ms"></a>

### TIM_UI_init_ms

```cpp
void TIM_UI_init_ms(TIM_TypeDef * TIMx, uint32_t msec)
```

Defined in include/ecTIM2.h:67

Initialize a timer with a period in msec and enable its update interrupt in NVIC.

> [!NOTE]
> Counter clock f_cnt = 10 kHz. NVIC priority is fixed at 2.


> [!NOTE]
> Calls [TIM_init_ms()](#tim_init_ms) → [TIM_UI_enable()](#tim_ui_enable) → NVIC enable.


#### Example

```cpp
volatile uint32_t count = 0;

TIM_UI_init_ms(TIM3, 1);           // update interrupt every 1 msec

void TIM3_IRQHandler(void) {
    if (is_UIF(TIM3)) {
        count++;
        clear_UIF(TIM3);
    }
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer to use (TIM1\~TIM5, TIM9\~TIM11) |
| `msec` | `uint32_t` | Update interrupt period [msec]. 16-bit timer: 1 \~ 6,553 msec |

---

<a id="tim_ui_init_us"></a>

### TIM_UI_init_us

```cpp
void TIM_UI_init_us(TIM_TypeDef * TIMx, uint32_t usec)
```

Defined in include/ecTIM2.h:87

Initialize a timer with a period in usec and enable its update interrupt in NVIC.

> [!NOTE]
> Counter clock f_cnt = 1 MHz. NVIC priority is fixed at 2.


> [!NOTE]
> Calls [TIM_init_us()](#tim_init_us) → [TIM_UI_enable()](#tim_ui_enable) → NVIC enable.


#### Example

```cpp
TIM_UI_init_us(TIM2, 100);         // update interrupt every 100 usec (10 kHz)

void TIM2_IRQHandler(void) {
    if (is_UIF(TIM2)) {
        // sampling task
        clear_UIF(TIM2);
    }
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer to use (TIM1\~TIM5, TIM9\~TIM11) |
| `usec` | `uint32_t` | Update interrupt period [usec]. 16-bit timer: 1 \~ 65,535 usec |

---

<a id="tim_ui_enable"></a>

### TIM_UI_enable

```cpp
void TIM_UI_enable(TIM_TypeDef * TIMx)
```

Defined in include/ecTIM2.h:100

Enable the update interrupt (UIE) of a timer.

> [!NOTE]
> The UIF flag is cleared first, so a stale flag does not fire immediately.


#### Example

```cpp
TIM_UI_disable(TIM3);              // pause the periodic task
// ...
TIM_UI_enable(TIM3);               // resume
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer to enable |

---

<a id="tim_ui_disable"></a>

### TIM_UI_disable

```cpp
void TIM_UI_disable(TIM_TypeDef * TIMx)
```

Defined in include/ecTIM2.h:110

Disable the update interrupt (UIE) of a timer. The counter keeps running.

#### Example

```cpp
TIM_UI_disable(TIM3);              // stop the periodic interrupt
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer to disable |

---

<a id="is_uif"></a>

### is_UIF

```cpp
uint32_t is_UIF(TIM_TypeDef * TIMx)
```

Defined in include/ecTIM2.h:127

Check the update interrupt flag (UIF) of a timer.

#### Returns
1 if an update event (overflow/underflow) occurred, 0 otherwise 

> [!NOTE]
> Use inside the timer IRQ handler.


#### Example

```cpp
void TIM3_IRQHandler(void) {
    if (is_UIF(TIM3)) {            // update event?
        GPIO_write(PA_5, !GPIO_read(PA_5));
        clear_UIF(TIM3);
    }
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer to check |

---

<a id="clear_uif"></a>

### clear_UIF

```cpp
void clear_UIF(TIM_TypeDef * TIMx)
```

Defined in include/ecTIM2.h:143

Clear the update interrupt flag (UIF) of a timer.

> [!NOTE]
> Call at the end of the IRQ handler. If not cleared, the handler is re-entered repeatedly.


#### Example

```cpp
void TIM3_IRQHandler(void) {
    if (is_UIF(TIM3)) {
        // task
        clear_UIF(TIM3);           // without this, the handler is re-entered forever
    }
}
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer whose UIF is cleared |

---

<a id="tim_init_ms"></a>

### TIM_init_ms

```cpp
void TIM_init_ms(TIM_TypeDef * TIMx, uint32_t msec)
```

Defined in include/ecTIM2.h:161

Enable the timer clock, set the period in msec, set up-counting, and start the counter.

> [!NOTE]
> f_cnt = 10 kHz (PSC = EC_SYSCLK / 10 kHz - 1). The update interrupt is NOT enabled. Use [TIM_UI_init_ms()](#tim_ui_init_ms) if an interrupt is needed.


#### Example

```cpp
TIM_init_ms(TIM3, 500);            // counter overflows every 500 msec
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer to use (TIM1\~TIM5, TIM9\~TIM11) |
| `msec` | `uint32_t` | Counter period [msec]. 16-bit timer: 1 \~ 6,553 msec |

---

<a id="tim_init_us"></a>

### TIM_init_us

```cpp
void TIM_init_us(TIM_TypeDef * TIMx, uint32_t usec)
```

Defined in include/ecTIM2.h:174

Enable the timer clock, set the period in usec, set up-counting, and start the counter.

> [!NOTE]
> f_cnt = 1 MHz (PSC = EC_SYSCLK / 1 MHz - 1). The update interrupt is NOT enabled. Use [TIM_UI_init_us()](#tim_ui_init_us) if an interrupt is needed.


#### Example

```cpp
TIM_init_us(TIM4, 1000);           // counter overflows every 1 msec, 1 tick = 1 usec
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer to use (TIM1\~TIM5, TIM9\~TIM11) |
| `usec` | `uint32_t` | Counter period [usec]. 16-bit timer: 1 \~ 65,535 usec |

---

<a id="tim_period_ms"></a>

### TIM_period_ms

```cpp
void TIM_period_ms(TIM_TypeDef * TIMx, uint32_t msec)
```

Defined in include/ecTIM2.h:186

Set the counter period in msec (PSC and ARR only).

> [!NOTE]
> f_cnt = 10 kHz, ARR = 10 * msec - 1. Clock and counter enable are not touched.


#### Example

```cpp
TIM_period_ms(TIM3, 100);          // change the period to 100 msec at runtime
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer to configure |
| `msec` | `uint32_t` | Counter period [msec]. 16-bit timer: 1 \~ 6,553 msec |

---

<a id="tim_period_us"></a>

### TIM_period_us

```cpp
void TIM_period_us(TIM_TypeDef * TIMx, uint32_t usec)
```

Defined in include/ecTIM2.h:198

Set the counter period in usec (PSC and ARR only).

> [!NOTE]
> f_cnt = 1 MHz, ARR = usec - 1. Clock and counter enable are not touched.


#### Example

```cpp
TIM_period_us(TIM3, 50);           // change the period to 50 usec (20 kHz)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer to configure |
| `usec` | `uint32_t` | Counter period [usec]. 16-bit timer: 1 \~ 65,535 usec |

---

<a id="tim_period_tick"></a>

### TIM_period_tick

```cpp
void TIM_period_tick(TIM_TypeDef * TIMx, uint32_t tick_us, uint32_t period_us)
```

Defined in include/ecTIM2.h:212

Set both the tick size and the counter period in usec.

> [!NOTE]
> PSC = (EC_SYSCLK / 1 MHz) * tick_us - 1, ARR = period_us / tick_us - 1.


> [!NOTE]
> (period_us / tick_us) must not exceed 65,536 on a 16-bit timer.


#### Example

```cpp
TIM_period_tick(TIM3, 10, 100000); // 1 tick = 10 usec, period = 100 msec (ARR = 9999)
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer to configure |
| `tick_us` | `uint32_t` | One counter tick [usec]. PSC is 16-bit → 1 \~ 780 usec at 84 MHz |
| `period_us` | `uint32_t` | Counter period [usec]. Must be a multiple of tick_us |

---

<a id="get_tim_irqn"></a>

### get_TIM_IRQn

```cpp
uint32_t get_TIM_IRQn(TIM_TypeDef * TIMx)
```

Defined in include/ecTIM2.h:226

Get the NVIC IRQ number of a timer.

#### Returns
IRQn value for NVIC_EnableIRQ() / NVIC_SetPriority() 

> [!NOTE]
> TIM1 update and TIM10 share TIM1_UP_TIM10_IRQn (TIM1_UP_TIM10_IRQHandler()). TIM9 → TIM1_BRK_TIM9_IRQn, TIM11 → TIM1_TRG_COM_TIM11_IRQn.


#### Example

```cpp
uint32_t irq = get_TIM_IRQn(TIM2); // TIM2_IRQn
NVIC_SetPriority(irq, 1);          // change priority after TIM_UI_init_xx()
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer (TIM1\~TIM5, TIM9\~TIM11) |

---

<a id="tim_clock_enable"></a>

### TIM_clock_enable

```cpp
void TIM_clock_enable(TIM_TypeDef * TIMx)
```

Defined in include/ecTIM2.h:237

Enable the RCC clock of a timer.

> [!NOTE]
> Called inside [TIM_init_ms()](#tim_init_ms) / [TIM_init_us()](#tim_init_us).


#### Example

```cpp
TIM_clock_enable(TIM2);            // RCC->APB1ENR |= TIM2EN
```

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `TIMx` | `TIM_TypeDef *` | Timer (TIM1, TIM9\~TIM11 on APB2 / TIM2\~TIM5 on APB1) |
