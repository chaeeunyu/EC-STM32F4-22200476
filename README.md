# Intro.md

**[github repository: EC-STM32F4-22200476](https://github.com/chaeeunyu/EC-STM32F4-22200476)**

Embedded Controller, HGU (26-Fall)

Firmware for STM32F4



folder structure

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

{#pinnames}

# Pin Names

Pin name types used by all EC library functions, and conversion from a pin name to its GPIO port and pin number.

## Enumerations

| Name | Description |
|------|-------------|
| [`PortName_t`](#portname_t)  | GPIO port index. Used as the upper nibble of [PinName_t](#pinname_t). |
| [`PinName_t`](#pinname_t)  | Pin name. Encoded as 0xXY: X = port index ([PortName_t](#portname_t)), Y = pin number (0~15). For example, PC_13 = 0x2D (port C = 2, pin 13 = 0xD). Arduino connector names (A0~A5, D0~D15) are aliases of the matching pins. NC means "not connected". |

---

{#portname_t}

### PortName_t

```cpp
enum PortName_t
```

Defined in include/ecPinNames.h:40

GPIO port index. Used as the upper nibble of [PinName_t](#pinname_t).

| Value | Description |
|-------|-------------|
| `PortA` |  |
| `PortB` |  |
| `PortC` |  |
| `PortD` |  |
| `PortE` |  |
| `PortF` |  |
| `PortG` |  |
| `PortH` |  |
| `PortI` |  |
| `PortJ` |  |
| `PortK` |  |

---

{#pinname_t}

### PinName_t

```cpp
enum PinName_t
```

Defined in include/ecPinNames.h:61

Pin name. Encoded as 0xXY: X = port index ([PortName_t](#portname_t)), Y = pin number (0~15). For example, PC_13 = 0x2D (port C = 2, pin 13 = 0xD). Arduino connector names (A0~A5, D0~D15) are aliases of the matching pins. NC means "not connected".

| Value | Description |
|-------|-------------|
| `PA_0` |  |
| `PA_1` |  |
| `PA_2` |  |
| `PA_3` |  |
| `PA_4` |  |
| `PA_5` |  |
| `PA_6` |  |
| `PA_7` |  |
| `PA_8` |  |
| `PA_9` |  |
| `PA_10` |  |
| `PA_11` |  |
| `PA_12` |  |
| `PA_13` |  |
| `PA_14` |  |
| `PA_15` |  |
| `PB_0` |  |
| `PB_1` |  |
| `PB_2` |  |
| `PB_3` |  |
| `PB_4` |  |
| `PB_5` |  |
| `PB_6` |  |
| `PB_7` |  |
| `PB_8` |  |
| `PB_9` |  |
| `PB_10` |  |
| `PB_12` |  |
| `PB_13` |  |
| `PB_14` |  |
| `PB_15` |  |
| `PC_0` |  |
| `PC_1` |  |
| `PC_2` |  |
| `PC_3` |  |
| `PC_4` |  |
| `PC_5` |  |
| `PC_6` |  |
| `PC_7` |  |
| `PC_8` |  |
| `PC_9` |  |
| `PC_10` |  |
| `PC_11` |  |
| `PC_12` |  |
| `PC_13` |  |
| `PC_14` |  |
| `PC_15` |  |
| `PD_2` |  |
| `PH_0` |  |
| `PH_1` |  |
| `A0` |  |
| `A1` |  |
| `A2` |  |
| `A3` |  |
| `A4` |  |
| `A5` |  |
| `D0` |  |
| `D1` |  |
| `D2` |  |
| `D3` |  |
| `D4` |  |
| `D5` |  |
| `D6` |  |
| `D7` |  |
| `D8` |  |
| `D9` |  |
| `D10` |  |
| `D11` |  |
| `D12` |  |
| `D13` |  |
| `D14` |  |
| `D15` |  |
| `NC` |  |
## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`ecPinmap`](#ecpinmap)  | Convert a pin name to its GPIO port and pin number. |

---

{#ecpinmap}

### ecPinmap

```cpp
void ecPinmap(PinName_t pinName, GPIO_TypeDef ** GPIOx, unsigned int * pin)
```

Defined in include/ecPinNames.h:155

Convert a pin name to its GPIO port and pin number.

:::note
Only ports A, B, C, D, H are mapped. Any other port falls back to GPIOA. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](#pinname_t) | Pin name (e.g., PA_5) |
| `GPIOx` | `GPIO_TypeDef **` | GPIO port pointer (e.g., GPIOA) |
| `pin` | `unsigned int *` | Pin number, 0~15 |


{#rccclockdriver}

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

{#delay_ms_hsi}

### delay_ms_HSI

```cpp
void delay_ms_HSI(uint32_t ms)
```

Defined in include/ecRCC2.h:49

Approximate blocking delay using a busy-wait loop.

:::note
The loop count is estimated from EC_SYSCLK, so the timing is not exact. Use [delay_ms()](api-SysTick.md#delay_ms) (SysTick) when accurate timing is needed. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `ms` | `uint32_t` | Delay time [ms] |

---

{#rcc_hsi_init}

### RCC_HSI_init

```cpp
void RCC_HSI_init(void)
```

Defined in include/ecRCC2.h:55

Use the internal HSI oscillator (16 MHz) as the system clock.

:::note
Sets EC_SYSCLK to 16000000. 

:::

---

{#rcc_pll_init}

### RCC_PLL_init

```cpp
void RCC_PLL_init(void)
```

Defined in include/ecRCC2.h:65

Use the PLL (from HSI) as the system clock at 84 MHz.

PLL: 16 MHz x 84 / 8 / 2 = 84 MHz. Flash latency = 2 wait states. Bus clocks: AHB = 84 MHz, APB1 = 42 MHz, APB2 = 84 MHz. 
:::note
Sets EC_SYSCLK to 84000000. Call before [SysTick_init()](api-SysTick.md#systick_init) and USART setup, since they compute their timing from EC_SYSCLK. 

:::

---

{#rcc_gpioa_enable}

### RCC_GPIOA_enable

```cpp
void RCC_GPIOA_enable(void)
```

Defined in include/ecRCC2.h:68

Enable the AHB1 clock of GPIO port A.

---

{#rcc_gpiob_enable}

### RCC_GPIOB_enable

```cpp
void RCC_GPIOB_enable(void)
```

Defined in include/ecRCC2.h:71

Enable the AHB1 clock of GPIO port B.

---

{#rcc_gpioc_enable}

### RCC_GPIOC_enable

```cpp
void RCC_GPIOC_enable(void)
```

Defined in include/ecRCC2.h:74

Enable the AHB1 clock of GPIO port C.

---

{#rcc_gpiod_enable}

### RCC_GPIOD_enable

```cpp
void RCC_GPIOD_enable(void)
```

Defined in include/ecRCC2.h:77

Enable the AHB1 clock of GPIO port D.

---

{#rcc_gpioe_enable}

### RCC_GPIOE_enable

```cpp
void RCC_GPIOE_enable(void)
```

Defined in include/ecRCC2.h:80

Enable the AHB1 clock of GPIO port E.

## Variables

| Return | Name | Description |
|--------|------|-------------|
| `volatile int` | [`EC_SYSCLK`](#ec_sysclk)  | Current system clock frequency [Hz]. Updated by [RCC_HSI_init()](#rcc_hsi_init) and [RCC_PLL_init()](#rcc_pll_init). |

---

{#ec_sysclk}

### EC_SYSCLK

```cpp
volatile int EC_SYSCLK
```

Defined in include/ecRCC2.h:41

Current system clock frequency [Hz]. Updated by [RCC_HSI_init()](#rcc_hsi_init) and [RCC_PLL_init()](#rcc_pll_init).


{#gpiodriver}

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

{#gpio_init}

### GPIO_init

```cpp
void GPIO_init(PinName_t pinName, uint32_t mode)
```

Defined in include/ecGPIO2.h:70

Initialize a GPIO pin: enable the port clock and set the pin mode.

:::note
Call this before any other GPIO_* function on the same pin. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to initialize (e.g., PA_5). See [ecPinNames.h](#ecpinnamesh). |
| `mode` | `uint32_t` | INPUT, OUTPUT, AF, or ANALOG |

---

{#gpio_mode}

### GPIO_mode

```cpp
void GPIO_mode(PinName_t pinName, uint32_t mode)
```

Defined in include/ecGPIO2.h:77

Set the mode of a GPIO pin (MODER register).

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin (e.g., PA_5) |
| `mode` | `uint32_t` | INPUT, OUTPUT, AF, or ANALOG |

---

{#gpio_ospeed}

### GPIO_ospeed

```cpp
void GPIO_ospeed(PinName_t pinName, uint32_t speed)
```

Defined in include/ecGPIO2.h:85

Set the output speed of a GPIO pin (OSPEEDR register).

:::note
Effective only in OUTPUT or AF mode. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `speed` | `uint32_t` | LOWSPEED, MEDSPEED, FASTSPEED, or HIGHSPEED |

---

{#gpio_otype}

### GPIO_otype

```cpp
void GPIO_otype(PinName_t pinName, uint32_t type)
```

Defined in include/ecGPIO2.h:93

Set the output type of a GPIO pin (OTYPER register).

:::note
Effective only in OUTPUT or AF mode. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `type` | `uint32_t` | PUSH_PULL or OPEN_DRAIN |

---

{#gpio_pupd}

### GPIO_pupd

```cpp
void GPIO_pupd(PinName_t pinName, uint32_t pupd)
```

Defined in include/ecGPIO2.h:100

Set the internal pull-up/pull-down resistor of a GPIO pin (PUPDR register).

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `pupd` | `uint32_t` | NPUPD, PULL_UP, or PULL_DOWN |

---

{#gpio_write}

### GPIO_write

```cpp
void GPIO_write(PinName_t pinName, uint32_t Output)
```

Defined in include/ecGPIO2.h:108

Write a logic level to a GPIO output pin.

:::note
The pin must be initialized in OUTPUT mode. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |
| `Output` | `uint32_t` | HIGH (1) or LOW (0) |

---

{#gpio_read}

### GPIO_read

```cpp
uint32_t GPIO_read(PinName_t pinName)
```

Defined in include/ecGPIO2.h:116

Read the logic level of a GPIO input pin.

#### Returns
HIGH (1) or LOW (0) 

:::note
The pin must be initialized in INPUT mode. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Target pin |

---

{#fnd_init}

### FND_init

```cpp
void FND_init(PinName_t * pinFND)
```

Defined in include/ecGPIO2.h:126

Initialize the 8 segment pins (a~g, dp) as outputs.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of 8 pins, in the order a, b, c, d, e, f, g, dp |

---

{#fnd_select_init}

### FND_select_init

```cpp
void FND_select_init(PinName_t * selectFND)
```

Defined in include/ecGPIO2.h:132

Initialize the digit-select pins as outputs.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `selectFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of digit-select pins |

---

{#fnd_display}

### FND_display

```cpp
void FND_display(uint8_t num)
```

Defined in include/ecGPIO2.h:139

Display a number on the currently selected digit.

:::note
[FND_init()](#fnd_init) must be called first. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `num` | `uint8_t` | Number to display, 0~11 (index of numberFND) |

---

{#fnd_select}

### FND_select

```cpp
void FND_select(uint8_t digit)
```

Defined in include/ecGPIO2.h:146

Select which digit of the display is active.

:::note
[FND_select_init()](#fnd_select_init) must be called first. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `digit` | `uint8_t` | Digit index |

---

{#fnd_select_pupd}

### FND_select_pupd

```cpp
void FND_select_pupd(PinName_t * selectFND, uint32_t pupd)
```

Defined in include/ecGPIO2.h:153

Set pull-up/pull-down for all digit-select pins.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `selectFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of digit-select pins |
| `pupd` | `uint32_t` | NPUPD, PULL_UP, or PULL_DOWN |

---

{#fnd_select_otype}

### FND_select_otype

```cpp
void FND_select_otype(PinName_t * selectFND, uint32_t otype)
```

Defined in include/ecGPIO2.h:160

Set output type for all digit-select pins.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `selectFND` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of digit-select pins |
| `otype` | `uint32_t` | PUSH_PULL or OPEN_DRAIN |

---

{#fnd_select_ospeed}

### FND_select_ospeed

```cpp
void FND_select_ospeed(PinName_t * selectFND, uint32_t ospeed)
```

Defined in include/ecGPIO2.h:167

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

{#numberfnd}

### numberFND

```cpp
int numberFND[12][8]
```

Defined in include/ecGPIO2.h:120

Segment patterns for each displayable number. [number][segment a~g, dp]


{#systicktimer}

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

{#systick_init}

### SysTick_init

```cpp
void SysTick_init(void)
```

Defined in include/ecSysTick2.h:51

Configure SysTick to interrupt every 1 ms and enable it (NVIC priority 15).

:::note
Call after [RCC_HSI_init()](api-RCC.md#rcc_hsi_init) or [RCC_PLL_init()](api-RCC.md#rcc_pll_init), because the reload value is computed from EC_SYSCLK. 

:::

---

{#systick_handler}

### SysTick_Handler

```cpp
void SysTick_Handler(void)
```

Defined in include/ecSysTick2.h:57

SysTick interrupt handler. Called by hardware every 1 ms; increments msTicks.

:::note
Do not call this function directly. 

:::

---

{#systick_counter}

### SysTick_counter

```cpp
void SysTick_counter(void)
```

Defined in include/ecSysTick2.h:63

Increment msTicks by 1.

:::note
Called from [SysTick_Handler()](#systick_handler). Not intended for direct use. 

:::

---

{#delay_ms}

### delay_ms

```cpp
void delay_ms(uint32_t msec)
```

Defined in include/ecSysTick2.h:72

Blocking delay in milliseconds, based on msTicks.

:::note
[SysTick_init()](#systick_init) must be called first. 

:::

:::warning
Do not call inside an ISR whose priority is equal to or higher than SysTick (priority 15). msTicks will not increase and the program will hang. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `msec` | `uint32_t` | Delay time [ms] |

---

{#systick_reset}

### SysTick_reset

```cpp
void SysTick_reset(void)
```

Defined in include/ecSysTick2.h:78

Reset the SysTick current value register (VAL) to 0.

:::note
This does not reset msTicks. 

:::

---

{#systick_val}

### SysTick_val

```cpp
uint32_t SysTick_val(void)
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

{#msticks}

### msTicks

```cpp
volatile uint32_t msTicks
```

Defined in include/ecSysTick2.h:43

Millisecond counter. Incremented by 1 every 1 ms in [SysTick_Handler()](#systick_handler).


{#extidriver}

# EXTI Driver

Configure external interrupts on GPIO pins: trigger edge, NVIC priority, enable/disable (mask), and pending-flag handling.

## Macros

| Name | Description |
|------|-------------|
| [`FALL`](#fall)  |  |
| [`RISE`](#rise)  |  |
| [`BOTH`](#both)  |  |

---

{#fall}

### FALL

```cpp
#define FALL 0
```

Defined in include/ecEXTI2.h:39

---

{#rise}

### RISE

```cpp
#define RISE 1
```

Defined in include/ecEXTI2.h:40

---

{#both}

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

{#exti_init}

### EXTI_init

```cpp
void EXTI_init(PinName_t pinName, uint32_t trig_type, uint32_t priority)
```

Defined in include/ecEXTI2.h:54

Connect a GPIO pin to its EXTI line, set the trigger edge, and enable the interrupt in NVIC.

:::note
Initialize the pin as INPUT with [GPIO_init()](api-GPIO.md#gpio_init) first. The SYSCFG clock is enabled inside. 

:::

:::note
Each EXTI line is shared by all ports with the same pin number (e.g., PA_13 and PC_13 cannot both be used). 

:::

:::note
Lines 5~9 share EXTI9_5_IRQHandler(), lines 10~15 share EXTI15_10_IRQHandler(). 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to use as interrupt source (e.g., PC_13). The pin number (0~15) selects the EXTI line, the port selects the source. |
| `trig_type` | `uint32_t` | FALL, RISE, or BOTH |
| `priority` | `uint32_t` | NVIC priority, 0 (highest) ~ 15 (lowest) |

---

{#exti_enable}

### EXTI_enable

```cpp
void EXTI_enable(PinName_t pinName)
```

Defined in include/ecEXTI2.h:61

Enable (unmask) the EXTI interrupt of a pin.

:::note
[EXTI_init()](#exti_init) must be called first. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin whose EXTI line is enabled |

---

{#exti_disable}

### EXTI_disable

```cpp
void EXTI_disable(PinName_t pinName)
```

Defined in include/ecEXTI2.h:67

Disable (mask) the EXTI interrupt of a pin. The configuration is kept.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin whose EXTI line is disabled |

---

{#is_pending_exti}

### is_pending_EXTI

```cpp
uint32_t is_pending_EXTI(PinName_t pinName)
```

Defined in include/ecEXTI2.h:75

Check whether the EXTI line of a pin has a pending interrupt.

#### Returns
1 if pending, 0 otherwise 

:::note
Use inside the IRQ handler to find which line triggered. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin to check |

---

{#clear_pending_exti}

### clear_pending_EXTI

```cpp
void clear_pending_EXTI(PinName_t pinName)
```

Defined in include/ecEXTI2.h:82

Clear the pending flag of the EXTI line of a pin.

:::note
Call at the end of the IRQ handler. If not cleared, the handler is re-entered repeatedly. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `pinName` | [`PinName_t`](api-PinNames.md#pinname_t) | Pin whose pending flag is cleared |


{#steppermotor}

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

{#half}

### HALF

```cpp
#define HALF 0
```

Defined in include/ecStepper2.h:47

Half-step mode (8 states)

---

{#full}

### FULL

```cpp
#define FULL 1
```

Defined in include/ecStepper2.h:48

Full-step mode (4 states)

---

{#dir_ccw}

### DIR_CCW

```cpp
#define DIR_CCW 0
```

Defined in include/ecStepper2.h:49

Counter-clockwise direction

---

{#dir_cw}

### DIR_CW

```cpp
#define DIR_CW 1
```

Defined in include/ecStepper2.h:50

Clockwise direction

---

{#fullstep_per_rev}

### FULLSTEP_PER_REV

```cpp
#define FULLSTEP_PER_REV (64*32)
```

Defined in include/ecStepper2.h:54

Full steps per output-shaft revolution (2048)

---

{#halfstep_per_rev}

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

{#statenum}

### StateNum

```cpp
enum StateNum
```

Defined in include/ecStepper2.h:58

FSM state numbers. Full-step uses S0~S3, half-step uses S0~S7.

| Value | Description |
|-------|-------------|
| `S0` |  |
| `S1` |  |
| `S2` |  |
| `S3` |  |
| `S4` |  |
| `S5` |  |
| `S6` |  |
| `S7` |  |
## Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`stepper_init`](#stepper_init)  | Select the stepping mode and initialize the 4 motor pins as outputs. |
| `void` | [`stepper_speed`](#stepper_speed)  | Set the motor speed by converting [rpm] to the step period [ms]. |
| `void` | [`stepper_step`](#stepper_step)  | Rotate the motor for a given number of steps (blocking). |
| `void` | [`stepper_out`](#stepper_out)  | Write the coil output pattern of an FSM state to the motor pins. |
| `void` | [`stepper_stop`](#stepper_stop)  | Toggle pause/resume of the running [stepper_step()](#stepper_step). |

---

{#stepper_init}

### stepper_init

```cpp
void stepper_init(uint32_t mode, PinName_t * pinStepper)
```

Defined in include/ecStepper2.h:76

Select the stepping mode and initialize the 4 motor pins as outputs.

:::note
[SysTick_init()](api-SysTick.md#systick_init) must also be called before [stepper_step()](#stepper_step), since it uses [delay_ms()](api-SysTick.md#delay_ms). 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `mode` | `uint32_t` | FULL or HALF |
| `pinStepper` | [`PinName_t`](api-PinNames.md#pinname_t) * | Array of 4 pins, in the order A, B, A', B' |

---

{#stepper_speed}

### stepper_speed

```cpp
void stepper_speed(uint32_t speedRPM)
```

Defined in include/ecStepper2.h:84

Set the motor speed by converting [rpm] to the step period [ms].

:::note
The step period has 1 ms resolution and is computed with HALFSTEP_PER_REV, so speeds above about 14 rpm give a 0 ms period. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `speedRPM` | `uint32_t` | Motor speed [rpm] |

---

{#stepper_step}

### stepper_step

```cpp
void stepper_step(uint32_t steps, uint32_t direction)
```

Defined in include/ecStepper2.h:93

Rotate the motor for a given number of steps (blocking).

:::note
Returns immediately if [stepper_init()](#stepper_init) was not called. While paused by [stepper_stop()](#stepper_stop), the function waits. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `steps` | `uint32_t` | Number of steps |
| `direction` | `uint32_t` | DIR_CW or DIR_CCW |

---

{#stepper_out}

### stepper_out

```cpp
void stepper_out(uint32_t state)
```

Defined in include/ecStepper2.h:100

Write the coil output pattern of an FSM state to the motor pins.

:::note
Called internally by [stepper_step()](#stepper_step). 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `state` | `uint32_t` | FSM state number (S0~S3 for FULL, S0~S7 for HALF) |

---

{#stepper_stop}

### stepper_stop

```cpp
void stepper_stop(void)
```

Defined in include/ecStepper2.h:107

Toggle pause/resume of the running [stepper_step()](#stepper_step).

:::note
Despite the name, it does not stop the motor permanently. Each call toggles the pause state. Typically called from an EXTI handler (button). 

:::


## Class Definitions

{#statestepper_t}

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

{#next}

##### next

```cpp
uint32_t next[2]
```

Defined in include/ecStepper2.h:64

Next state. Index = direction (DIR_CCW or DIR_CW)

---

{#out}

##### out

```cpp
uint8_t out[4]
```

Defined in include/ecStepper2.h:65

Output levels for coils A, B, A', B'


{#usartdriver}

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

{#pol}

### POL

```cpp
#define POL 0
```

Defined in include/ecUART2.h:46

Polling mode

---

{#int}

### INT

```cpp
#define INT 1
```

Defined in include/ecUART2.h:47

Interrupt mode

---

{#baud_9600}

### BAUD_9600

```cpp
#define BAUD_9600 9600
```

Defined in include/ecUART2.h:50

9600 bps

---

{#baud_19200}

### BAUD_19200

```cpp
#define BAUD_19200 19200
```

Defined in include/ecUART2.h:51

19200 bps

---

{#baud_38400}

### BAUD_38400

```cpp
#define BAUD_38400 38400
```

Defined in include/ecUART2.h:52

38400 bps

---

{#baud_57600}

### BAUD_57600

```cpp
#define BAUD_57600 57600
```

Defined in include/ecUART2.h:53

57600 bps

---

{#baud_115200}

### BAUD_115200

```cpp
#define BAUD_115200 115200
```

Defined in include/ecUART2.h:54

115200 bps

---

{#baud_921600}

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

{#usart_init}

### USART_init

```cpp
void USART_init(USART_TypeDef * USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX, uint32_t baud)
```

Defined in include/ecUART2.h:90

Initialize a USART with given TX/RX pins and baud rate, and enable its RX interrupt.

Frame format: 8 data bits, no parity, 1 stop bit, oversampling x16. 
:::note
Call after [RCC_PLL_init()](api-RCC.md#rcc_pll_init) or [RCC_HSI_init()](api-RCC.md#rcc_hsi_init), since the baud rate is computed from EC_SYSCLK. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |
| `pin_GPIO_TX` | [`PinName_t`](api-PinNames.md#pinname_t) | TX pin (e.g., PA_2) |
| `pin_GPIO_RX` | [`PinName_t`](api-PinNames.md#pinname_t) | RX pin (e.g., PA_3) |
| `baud` | `uint32_t` | Baud rate [bps] (e.g., BAUD_9600) |

---

{#usart_baud}

### USART_baud

```cpp
void USART_baud(USART_TypeDef * USARTx, uint32_t baud)
```

Defined in include/ecUART2.h:98

Set the baud rate of a USART (BRR register).

:::note
Uses APB1 (EC_SYSCLK / 2) for USART2 and APB2 (EC_SYSCLK) for USART1/6. 

:::

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1, USART2, or USART6 |
| `baud` | `uint32_t` | Baud rate [bps] |

---

{#usart_pins}

### USART_pins

```cpp
void USART_pins(USART_TypeDef * USARTx, PinName_t pin_GPIO_TX, PinName_t pin_GPIO_RX)
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

{#usart1_baud}

### USART1_baud

```cpp
void USART1_baud(uint32_t baud)
```

Defined in include/ecUART2.h:113

Set the baud rate of USART1.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `baud` | `uint32_t` | Baud rate [bps] |

---

{#usart2_baud}

### USART2_baud

```cpp
void USART2_baud(uint32_t baud)
```

Defined in include/ecUART2.h:119

Set the baud rate of USART2.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `baud` | `uint32_t` | Baud rate [bps] |

---

{#usart1_init}

### USART1_init

```cpp
void USART1_init(void)
```

Defined in include/ecUART2.h:129

Initialize USART1 with default settings: PA_9 (TX), PA_10 (RX), 9600 bps, RX interrupt enabled.

---

{#usart2_init}

### USART2_init

```cpp
void USART2_init(void)
```

Defined in include/ecUART2.h:135

Initialize USART2 with default settings: PA_2 (TX), PA_3 (RX), 9600 bps, RX interrupt enabled.

:::note
USART2 is connected to the PC through the Nucleo USB (virtual COM port). 

:::

---

{#usart1_write}

### USART1_write

```cpp
void USART1_write(uint8_t * buffer, uint32_t nBytes)
```

Defined in include/ecUART2.h:144

Send bytes through USART1 (blocking).

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `buffer` | `uint8_t *` | Data to send |
| `nBytes` | `uint32_t` | Number of bytes |

---

{#usart2_write}

### USART2_write

```cpp
void USART2_write(uint8_t * buffer, uint32_t nBytes)
```

Defined in include/ecUART2.h:151

Send bytes through USART2 (blocking).

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `buffer` | `uint8_t *` | Data to send |
| `nBytes` | `uint32_t` | Number of bytes |

---

{#usart1_read}

### USART1_read

```cpp
uint8_t USART1_read(void)
```

Defined in include/ecUART2.h:157

Receive one byte from USART1 (blocking until data arrives).

#### Returns
Received byte

---

{#usart2_read}

### USART2_read

```cpp
uint8_t USART2_read(void)
```

Defined in include/ecUART2.h:163

Receive one byte from USART2 (blocking until data arrives).

#### Returns
Received byte

---

{#is_usart1_rxne}

### is_USART1_RXNE

```cpp
uint32_t is_USART1_RXNE(void)
```

Defined in include/ecUART2.h:171

Check whether USART1 has received data (RXNE flag).

#### Returns
1 if data is ready to read, 0 otherwise

---

{#is_usart2_rxne}

### is_USART2_RXNE

```cpp
uint32_t is_USART2_RXNE(void)
```

Defined in include/ecUART2.h:177

Check whether USART2 has received data (RXNE flag).

#### Returns
1 if data is ready to read, 0 otherwise

---

{#usart_clock_enable}

### USART_clock_enable

```cpp
void USART_clock_enable(USART_TypeDef * USARTx)
```

Defined in include/ecUART2.h:185

Enable the peripheral clock of a USART.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `USARTx` | `USART_TypeDef *` | USART1 / USART6 (APB2) or USART2 (APB1) |

---

{#usart_write}

### USART_write

```cpp
void USART_write(USART_TypeDef * USARTx, uint8_t * buffer, uint32_t nBytes)
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

{#usart_read}

### USART_read

```cpp
uint8_t USART_read(USART_TypeDef * USARTx)
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

{#is_usart_rxne}

### is_USART_RXNE

```cpp
uint32_t is_USART_RXNE(USART_TypeDef * USARTx)
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

{#usart_delay}

### USART_delay

```cpp
void USART_delay(uint32_t us)
```

Defined in include/ecUART2.h:213

Approximate busy-wait delay used between transmitted bytes.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `us` | `uint32_t` | Delay time [us] (approximate, not calibrated) |


{#mcuinitialization}

# MCU Initialization

One-call default setup of the MCU.


