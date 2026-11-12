#include "ecPinNames.h"

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