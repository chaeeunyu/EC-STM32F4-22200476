# PreLAB: Timer Interrupt & PWM



## Code

### Timer Interrupt



#### TIM_init_ms()

```c
// Timer Counter Initialization
// Default : f_cnt =  10kHz, msec= 1 to 6,553 msec 
void TIM_init_ms(TIM_TypeDef* TIMx, uint32_t msec){ 
// 1. Enable Timer CLOCK
	TIM_clock_enable(TIMx);

// 2. Set CNT period
	TIM_period_ms(TIMx, msec); 			

// 3. COUNTER Direction
	// Up-Counter	
	TIMx->CR1 &= ~TIM_CR1_DIR;	
	// [YOUR CODE GOES HERE !!!!] -> completed
	
	// Down-Counter
	//TIMx->CR1 |= TIM_CR1_DIR;

// 4. Enable Timer Counter
	TIMx->CR1 |= TIM_CR1_CEN;		
}
```



#### TIM_init_us()

```c
// Timer Counter Initialization
// Default : f_cnt =  1 MHz, usec= 1 to 65,530 usec
void TIM_init_us(TIM_TypeDef* TIMx, uint32_t usec){ 
// 1. Enable Timer CLOCK
	TIM_clock_enable(TIMx);		// completed

// 2. Set CNT period
	TIM_period_us(TIMx, usec); 

// 3. CNT Direction
	TIMx->CR1 &= ~TIM_CR1_DIR; 		// up, completed

// 4. Enable Timer Counter
	TIMx->CR1 |= TIM_CR1_CEN;	// completed
}
```





#### TIM_period_us

```c
// Limits    : 16-bit ARR -> f_cnt =  1 MHz, usec= 1 to 65,530 usec (ARR < 0xFFFF)
void TIM_period_us(TIM_TypeDef* TIMx, uint32_t usec){
	// initialize
	uint32_t Sys_CLK = 0;		uint32_t PSCval = 0;
	uint32_t f_cnt = 0;			uint32_t ARRval = 0;

	Sys_CLK = EC_SYSCLK;					// 84MHz (PLL) or 16MHz (HSI)
	ARRval=0;

	// Counter Frequency (f_cnt) 
	f_cnt   =  1000000UL;					// 16-bit CNT: f_cnt=1MHz (default)
	
	// Prescaler Value
	PSCval = Sys_CLK / f_cnt;				// 84MHz -> 1 MHz (PSC=84) -> "how much time is it per 1 tick"
	TIMx->PSC = PSCval - 1;					
	// [YOUR CODE GOES HERE !!!!] --- completed

	// Reload Value (ARR) for usec unit
	ARRval = (f_cnt / 1000000UL) * usec;			
	// [YOUR CODE GOES HERE !!!!]	--- completed
	TIMx->ARR = ARRval - 1;
}

```





------

### PWM

#### PWM_init_config()

```C
// Internal Helper function: for PWM Initialization
void PWM_init_config(PinName_t pinName, uint32_t usec){
// 0. Match TIMx from given Port_y and Pin_x
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);
	TIM_TypeDef *TIMx = NULL;
	int chN = 0;
	PWM_pinmap(pinName, &TIMx, &chN);

// 1. Initialize GPIO port and pin as AF
	if(TIMx == NULL) return;
	GPIO_init(pinName, AF);  

// 2. Configure GPIO AFR by Pin x num.
	//  AFR[0] holds pin 0~7, AFR[1] holds pin 8~15 : 4 bits per pin
	uint32_t afrIdx   = pin >> 3;				// same as (pin/8)
	uint32_t afrShift = 4 * (pin % 8);
	uint32_t afNum    = 0;

	//  AF1 for TIM1,TIM2     AF2 for TIM3,TIM4
	if((TIMx == TIM1)||(TIMx == TIM2))		afNum = 1;	// AF1
	else if(TIMx== TIM3 || TIMx == TIM4)	afNum = 2;	// AF2
	// You can add more  for TIM5 to TIM11, if necessary [YOUR CODE GOES HERE!!]		


	// Set AFR register for the pin
	port->AFR[afrIdx] &= ~(0xFUL << afrShift);	// 4 bits per pin	
	port->AFR[afrIdx] |= afNum << afrShift; 
	// YOUR CODE GOES HERE!!]	-------- completed


// 3. Initialize Timer : select CLK_CNT according to PWM period 
	//   TIM2/TIM5 			(32-bit ARR) : 1 < usec < 2^32 usec 	-> TIM_init_us() 
	//   TIM1/TIM3/TIM4    	(16-bit ARR) : 1 < usec <=65,536 usec 	-> TIM_init_us() 
	//                            			65 < msec <65,536 msec	-> TIM_init() 
	if((TIMx == TIM2) || (TIMx == TIM5) || (usec <= 65536)){
		TIM_init_us(TIMx, usec);					
	}
	else {
		uint32_t msec = usec / 1000;
		if(msec > 6553) msec = 6553;			// 16-bit ARR has limit of 6.5sec
		TIM_init_ms(TIMx, msec);				// 65 < msec <65,536 msec	-> TIM_init() 	--- completed
	}
// 4. Configure Timer Output mode as PWM
	// Default: PWM Mode 1, active high, duty ratio = 50%
	PWM_mode_config(TIMx, chN, EC_PWM_MODE1);

// 5. Enable Timer Counter
	// For TIM1 ONLY
	if(TIMx == TIM1) TIMx->BDTR |= TIM_BDTR_MOE;					// Main output enable (MOE): 0 = Disable, 1 = Enable		
	// All Timer: First, Clear the counter and Clear UI Flag
	TIMx->EGR |= TIM_EGR_UG;					
	TIMx->SR   = ~TIM_SR_UIF;					
	// Enable timers
	TIMx->CR1  |= TIM_CR1_CEN;  	

}
```



#### PWM_duty()

```c
// Duty Ratio:  from 0.0 to 1.0
void (PinName_t pinName, float duty){
// 0. Match TIMx from  Port and Pin
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port, &pin);
	TIM_TypeDef *TIMx = NULL;
	int chN = 0;
	PWM_pinmap(pinName, &TIMx, &chN);

// 1. Check the duty ratio range from 0.0 to 1.0
	if(duty < 0.0f) duty = 0.0f;
	if(duty > 1.0f) duty = 1.0f; // --- completed

// 2. Duty ratio -> CCR counts.  Assume PWM mode 1 :
	if(TIMx == NULL) return;
	uint32_t CCvalue = (uint32_t)( ((float)(TIMx->ARR) + 1.0f) * duty );	// -- completed
	// [YOUR CODE GOES HERE!!]
	
	if(chN == 1)      { TIMx->CCR1 = CCvalue; }	//set channel
	else if(chN == 2) { TIMx->CCR2 = CCvalue; }
	// REPEAT for CHn= 3, 4
	else if(chN == 3) { TIMx->CCR3 = CCvalue; }
	else if(chN == 4) { TIMx->CCR4 = CCvalue; }
	// [YOUR CODE GOES HERE!!]
	// [YOUR CODE GOES HERE!!] - completed

}

```

