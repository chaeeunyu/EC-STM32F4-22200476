/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Yu Chaeeun
Created          : 05-03-2021
Modified         : Oct-12-2026 
Language/ver     : C++ in VS Code

Description      : Stepper Library. ecStepper2.c
/----------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "ecStepper2.h"


// Stepper Motor variable
static PinName_t stepperPins[4];
static volatile uint32_t _steps;
static volatile uint32_t _step_delay = 100;		
static const StateStepper_t *fsm;		// points at fsm_full or fsm_half (chosen in stepper_init)
static volatile uint32_t state = 0;		// current FSM state
volatile int pause_flag = 0;



// Output: A B A' B'
static const StateStepper_t fsm_Full[4] = {	
 	// {{next (Dir=0, Dir=1)}, {output (A, B, A', B')}}
	{{S1,S3},{HIGH,HIGH,LOW,LOW}},	// S0
	{{S2,S0},{LOW,HIGH,HIGH,LOW}},  // S1
	{{S3,S1},{LOW,LOW,HIGH,HIGH}},  // S2
	{{S0,S2},{HIGH,LOW,LOW,HIGH}},  // S3
};


static const StateStepper_t fsm_Half[8] = {  
	// {{next Dir=0, Dir=1}, {output A, B, A', B'}}
	{{S1,S7},{HIGH,LOW,LOW,LOW}},   // S0
	{{S2,S0},{HIGH,HIGH,LOW,LOW}},  // S1
	{{S3,S1},{LOW,HIGH,LOW,LOW}},   // S2
	{{S4,S2},{LOW,HIGH,HIGH,LOW}},  // S3
	{{S5,S3},{LOW,LOW,HIGH,LOW}},   // S4
	{{S6,S4},{LOW,LOW,HIGH,HIGH}},  // S5
	{{S7,S5},{LOW,LOW,LOW,HIGH}},   // S6
	{{S0,S6},{HIGH,LOW,LOW,HIGH}},  // S7
};


// Initialization of Stepper Motor
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

// Converts Motor [rpm] to step delay in [msec]
// speed -> by adjusting the step period(=_step_delay)
void stepper_speed (uint32_t speedRPM){      // rpm
		_step_delay = 60*1000/(FULLSTEP_PER_REV*speedRPM); 		//in [msec] ---------> MODIFY!!(HALF/FULL)	
		// [usec] can be used for a higher accuracy
		// But needs delay_us() 
}


// Run Stepper Motor for given steps and direction
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

// Stepper Motor Output for given state
void stepper_out(uint32_t state){
	
	// stepperPins[0]=A, stepperPins[1]=B, ...
	// out[0]=A, out[1]=B, ...

	// i want to give '1' to a certain pins(stepperPins[], A B A' B')
	for(uint32_t i=0; i<4; i++){
		GPIO_write(stepperPins[i], fsm[state].out[i]);
	}
}


// pause Stepper Motor
void stepper_pause(void) {
    
	pause_flag ^= 1;
}

// permanently stop motor
void stepper_stop(void){
	_steps = 0;    
    
	// All pins DigitalOut '0'
	for(uint32_t i=0; i<4; i++){
		GPIO_write(stepperPins[i], LOW);
	}
}



