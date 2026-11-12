/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 10-16-2026
Modified         : Nov-12-2026
Language/ver     : C in VS Code

Description      : LAB - Ultrasonic distance sensor

                   Measures the distance with ICAP of PB_6 (TIM4_CH1).
                   A PWM signal is generated on PA_6 (TIM3_CH1) 
                   Connect  PA_6 --> PB_6 
/----------------------------------------------------------------*/

#include "stm32f411xe.h"
#include "ecSTM32F4v2.h"

#define ICAP_PIN    PB_6        // TIM4_CH1 : capture input
#define PWM_PIN     PA_6        // TIM3_CH1 : pwm input signal (jumper PA_6 -> PB_6)
#define LED_PIN     PA_5

#define SOUND_HALF_CM_PER_US (340*100*1e-6f / 2)    // 340 m/s -> cm, µs convert, ÷2 since wangbok

uint32_t pwm_period_ms=100;
uint32_t uTick=10;
uint32_t timer_arr  = 65536;    // ticks per counter wrap (ARR+1)

volatile uint32_t ovf_cnt = 0;      //counts overflows
volatile uint32_t cap_prev   = 0;   // CCR of the previous rising edge
volatile uint32_t cap_curr    = 0;  // CCR of the current rising edge
volatile uint32_t counts = 0;       // [pulses]
volatile float timeInterval = 0;    // [usec]
float distance = 0;


void setup(void){
    RCC_PLL_init();
    SysTick_init();
    GPIO_init(LED_PIN,OUTPUT);
    //USART2_init();                    
    USART_init(USART2, PA_2, PA_3, 9600);

    // PWM: Test signal of period_ms of 50% duty 
    PWM_init(PWM_PIN, pwm_period_ms);
    TIM_period_tick(TIM3, 10, 100000);       // tick 10 us, overwrite with period 100 ms
    TIM3->EGR |= TIM_EGR_UG;                 // apply the changed PSC
    PWM_pulsewidth_us(PWM_PIN, 10);          // pulse width 10 us

    // Input capture  rising edge TIn=ICn, tick = uTick usec    
    ICAP_init(ICAP_PIN, IC_RISE);
    ICAP_ICn_init(ICAP_PIN, IC_FALL, IC_2);     //TI1 -> IC2, falling
    ICAP_tick(ICAP_PIN, uTick);         

    timer_arr=TIM4->ARR + 1;
}

int main(void){
    setup();
    printf("\n\n---------------------------\r\n");
    printf("mcu initialized\r\n");

    while(1){
        distance = timeInterval * SOUND_HALF_CM_PER_US;
        printf("distance: %.1f[cm]\r\n", distance);        
        led_toggle(LED_PIN);
        delay_ms(2000);
    }
}


void TIM4_IRQHandler(void){
    // Counter overflow 
    if(is_UIF(TIM4)){
        ovf_cnt++;
        clear_UIF(TIM4);
    }

    // Rising edge captured on IC1
    if(is_CCIF(TIM4, IC_1)){                    // IC1 : rising edge, SAME pin (TI1)
        cap_prev = ICAP_read(ICAP_PIN, IC_1);
        ovf_cnt    = 0;

        clear_CCIF(TIM4,IC_1);                                
    }
    // Falling edge captured on IC2
    if(is_CCIF(TIM4, IC_2)){                // IC2 : falling edge, SAME pin (TI1)       
        cap_curr = ICAP_read(ICAP_PIN, IC_2);

        // Ticks between the two edges, then scaled to usec.        
        counts = (cap_curr-cap_prev) + (ovf_cnt*timer_arr);
        timeInterval=counts*uTick;

        clear_CCIF(TIM4, IC_2);
    }   
}