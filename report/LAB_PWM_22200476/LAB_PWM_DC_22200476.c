/*----------------------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Nov-08-2026
Language/ver     : C in VS Code

Description      : LAB_PWM_DC
                   1) button(B1) -> fan MODE(state): OFF(0%), MID(50%), HIGH(100%)
                   2) PAUSE: When the object(face) is detected neraby, it automatically pauses the fan temporarily.
                           : the MODE should NOT change if the button B1 is pressed
                   3) RESUME: When the object(face) is away, it automatically resumes the fan running
                   4) LED: ON - fan on
                           OFF - fan off
                           blinking - fan pause
                   5) Serial Display: Print the Mode, PWM duty ratio and other necessary information every 2seconds
/----------------------------------------------------------------------------*/

#include "stm32f411xe.h"
#include "ecSTM32F4v2.h"

#define    PWM_DC         PA_0
#define    BUTTON_PIN     PC_13
#define    LED_PIN        PA_5  
#define    PROX_SENSOR    PC_3
#define    DIR_PIN        PC_2

// LED output codes
#define LED_OFF     0
#define LED_ON      1
#define LED_BLINK   2

#define DIR  0      // motor dir

// State names
#define S0          0   // OFF
#define S1          1   // MID (50%)
#define S2          2   // HIGH (100%)

// One output pair (V, L)
typedef struct {
    uint8_t V;          // PWM duty [%] (0, 50, 100)
    uint8_t L;          // LED_OFF / LED_ON / LED_BLINK
} Output_t;

// One state = one row of the FSM table
typedef struct {
    uint32_t next[4];   // next state for input 0~3
    Output_t out[4];    // output for input 0~3
} State_t;

State_t FSM[3] = {
    { {S0, S0, S1, S0}, { {0, LED_OFF}, {0, LED_BLINK}, {50, LED_ON}, {0, LED_BLINK} } },   // S0 (OFF)
    { {S1, S1, S2, S1}, { {50, LED_ON}, {0, LED_BLINK}, {100, LED_ON}, {0, LED_BLINK} } },  // S1 (MID) 
    { {S2, S2, S0, S2}, { {100, LED_ON}, {0, LED_BLINK}, {0, LED_OFF}, {0, LED_BLINK} } }   // S2 (HIGH)
};

const char *stateName[3] = {"OFF", "MID", "HIGH"};

volatile uint8_t mode_flag = 0;  // X: set by button exti
volatile uint32_t prevPress = 0;   // for button debounce


void setup(void){
	RCC_PLL_init();					// System Clock = 84MHz
	SysTick_init();		
    USART2_init();

    // PWM of 1 msec:  TIM2_CH1 (PA_0)
    PWM_init_us(PWM_DC, 1000);
    PWM_duty(PWM_DC, 0.0f);        // OFF, 0%

    GPIO_init(DIR_PIN, OUTPUT);
    GPIO_otype(DIR_PIN, LOW);
    GPIO_write(DIR_PIN, DIR);

    // initialize button(B1)
    GPIO_init(BUTTON_PIN, INPUT);
    GPIO_pupd(BUTTON_PIN, PULL_UP);

    // initialize proximity sensor
    GPIO_init(PROX_SENSOR, INPUT);
    GPIO_pupd(PROX_SENSOR, PULL_UP);

    // initialize button_pin
    EXTI_init(BUTTON_PIN, FALL, 10); 

    GPIO_init(LED_PIN, OUTPUT);   
    GPIO_write(LED_PIN, LOW);

}


int main(void){
    setup();
    printf("\n\n------------------ MCU initialized ------------------\r\n");

    uint32_t state = S0;
    uint32_t input = 0;
    uint8_t  X = 0, Y = 0;
    uint32_t prox_input = 0;
    Output_t out;
    float duty = 0.0f ;

    uint32_t prevPrint = 0;
    uint32_t prevBlink = 0;
    uint8_t  led = 0;

    while(1){
        
        X = mode_flag;
        mode_flag = 0;      // even consumed!

        prox_input = GPIO_read(PROX_SENSOR);  // active-low
        if(prox_input == LOW) Y = 1;    // object close
        else Y = 0;     // object far

        input = X*2 + Y ;   // column of the FSM table
        out = FSM[state].out[input] ;   // output from (current state, input)
        state = FSM[state].next[input]; // then move to the next state

        // button event
        duty = out.V / 100.0f ;
        if(DIR == 1) duty = 1.0f - duty;    // duty is inverted when DIR=1
        PWM_duty(PWM_DC, duty);

        if(out.L == LED_BLINK){
            if((msTicks - prevBlink) >= 1000){
                led ^= 1;
                GPIO_write(LED_PIN, led);
                prevBlink = msTicks;
            }
        }
        else GPIO_write(LED_PIN, out.L);

        if((msTicks - prevPrint) >= 2000){
            printf("Mode: %s, Duty ratio: %d %%, object: %s\r\n", 
                stateName[state], out.V, (Y == 1) ? "CLOSE(PAUSE)" : "FAR");
            prevPrint = msTicks;
        } 

		delay_ms(1); 

    }
}


// -------------- EXTI --------------------
void EXTI15_10_IRQHandler(void) {
	if (is_pending_EXTI(BUTTON_PIN)) {   // EXTI_BUTTON_PIN : mode change (0 > 1 > 2 > 0 >...)
		if(GPIO_read(BUTTON_PIN) == LOW && (msTicks - prevPress) >= 200){   // ignore bouncing
            mode_flag = 1;
            prevPress = msTicks;
        }
        clear_pending_EXTI(BUTTON_PIN);
	}
}