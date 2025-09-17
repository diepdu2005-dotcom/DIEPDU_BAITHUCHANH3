#include "stm32f10x.h" 

void GPIO_PC6(void){
  //Set bit 0 to enable clock for AFIO
  RCC->APB2ENR |= (1<<0);
	
	//Set bit 4 to enable clock for PORT C
	RCC->APB2ENR |= (1<<4);
	
	//Set bit 5 to enable clock for PORT D
	RCC->APB2ENR |= (1<<5);
	
	//PC6 configuration is output push-pull, speed 2MHz
	//MODE6 = 10 (output mode, max speed 2 Mhx)
	//CNF6  = 00 (push-pull)
	GPIOC->CRL &= ~(0xF << 24); // clear MODE6 + CNF6
  GPIOC->CRL |=  (0x2 << 24); // MODE6=10, CNF6=00

	//PD11 configuration is input floating
	//MODE11 = 00(input)
	//CNF11  = 01(Floating input)
	GPIOD->CRH &= ~(0xF << 12); // clear MODE11 + CNF11
  GPIOD->CRH |=  (0x4 << 12); // MODE11=00, CNF11=01 (floating input)
	
	//EXTI11 connected to PD11
	AFIO->EXTICR[2] &= ~(0xF <<  12);
	AFIO->EXTICR[2] |=  (3 <<  12);
	
	//Set bit 11 to enable an interrupt on the falling edge
	EXTI->FTSR |= (1<<11);
	//Set bit 11 to 'unmask'(enable) the interrupt from EXTI11
	//sent to the NVIC
	EXTI->IMR |= (1<<11);
	
	//Set bit (40-32)=8 to enable ISER
	NVIC->ISER[1] |= (1<<8);
}

void GPIO_PC8(void){
  //Set bit 0 to enable clock for AFIO
  RCC->APB2ENR |= (1<<0);
	
	//Set bit 4 to enable clock for PORT C
	RCC->APB2ENR |= (1<<4);
	
	//Set bit 5 to enable clock for PORT D
	RCC->APB2ENR |= (1<<5);
	
	//PC8 configuration is output push-pull, speed 2MHz
	//MODE8 = 10 (output mode, max speed 2 Mhx)
	//CNF8  = 00 (push-pull)
	GPIOC->CRH &= ~(0xF << 0); // clear MODE8 + CNF8
  GPIOC->CRH |=  (0x2 << 0); // MODE8=10, CNF8=00

	//PD14 configuration is input floating
	//MODE14 = 00(input)
	//CNF14  = 01(Floating input)
	GPIOD->CRH &= ~(0xF << 24); // clear MODE14 + CNF14
  GPIOD->CRH |=  (0x4 << 24); // MODE14=00, CNF14=01 (floating input)
	
	//EXTI14 connected to PD14
	AFIO->EXTICR[3] &= ~(0xF << 8);
	AFIO->EXTICR[3] |=  (3 << 8);
	
	//Set bit 14 to enable an interrupt on the falling edge
	EXTI->FTSR |= (1<<14);
	//Set bit 11 to 'unmask'(enable) the interrupt from EXTI14
	//sent to the NVIC
	EXTI->IMR |= (1<<14);
	
	//Set bit (40-32)=8 to enable ISER
	NVIC->ISER[1] |= (1<<8);
}


void EXTI15_10_IRQHandler(void) {
    if(EXTI->PR & (1<<11)) {
        EXTI->PR = (1<<11);         // Clear pending
        GPIOC->ODR ^= (1<<6);       // Toggle PC6
    }
    if(EXTI->PR & (1<<14)) {
        EXTI->PR = (1<<14);         // Clear pending
        GPIOC->ODR ^= (1<<8);       // Toggle PC8
    }
}

int main(void){
    GPIO_PC6();
    GPIO_PC8();
    while(1){
        // CPU idle, waiting for interrupts
    }
}