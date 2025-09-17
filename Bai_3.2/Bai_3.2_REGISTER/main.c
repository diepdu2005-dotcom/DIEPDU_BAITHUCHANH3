#include "stm32f10x.h"

void UART1_Init(void){
	//Set clock for GPIOA and UART1
	RCC->APB2ENR |= (1<<14); //UART1
	RCC->APB2ENR |= (1<<2); //PORT A
	
  //GPIO Configuration
	//PA9 - TX: MODE9: 11, CNF9: 10
	GPIOA->CRH &= ~(0xF<<4);
	GPIOA->CRH |=  (0xB<<4);
  
  //PA10- RX: MODE10: 00, CNF10:01
	GPIOA->CRH &= ~(0xF<<8);
	GPIOA->CRH |=  (0x4<<8);
  //UART Configuration
  // 96008N1
	USART1->BRR = 7500;//Baud rate: 9600 bps -> BRR= 72MHz/9600
	USART1->CR1 &= ~(1<<12);//8 bit
	USART1->CR1 &= ~(1<<10);//none parity
	USART1->CR2 &= ~(3<<12);//stopbit = 1
	
	USART1->CR1 |= (1<<3);// TE 
	USART1->CR1 |= (1<<2);// RE
	USART1->CR2 &= ~(1<<11);// disable clock == uart
	USART1->CR1 |=(1<<13);//UE
}

  //"Hello Diep_Van_Du via UART!\r\n"

void UART1_SendChar(char c){
    while(!(USART1->SR & (1<<7))); // Wait until TXE=1
    USART1->DR = c;
}
void UART1_SendString(char *str){
    while(*str){
        UART1_SendChar(*str++);
    }
}

void delay_ms(uint32_t ms){
    SysTick->LOAD = 72000 - 1;  // 1ms (72MHz / 72000 = 1kHz)
    SysTick->VAL  = 0;
    SysTick->CTRL = 5;          // Enable SysTick, no interrupt
    for(uint32_t i=0; i<ms; i++){
        while(!(SysTick->CTRL & (1<<16))); // Ch? c? COUNTFLAG
    }
    SysTick->CTRL = 0;
}

int main(void){
    UART1_Init();

    while(1){
        UART1_SendString("Hello Diep_Van_Du via UART!\r\n");
        delay_ms(1000); // 1 giây
    }
}
