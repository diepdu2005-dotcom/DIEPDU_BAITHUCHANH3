#include "stm32f10x.h"

//set up pc13
void GPIO_Init(void){
	RCC->APB2ENR |= (1<<4);
// MODE13=10, CNF13=00
	GPIOC->CRH &= ~(0xF<<20);
	GPIOC->CRH |= (0x2<<20);
}

//set up uart
void UART1_Init(void){
	RCC->APB2ENR |= (1<<14);//UART
	RCC->APB2ENR |= (1<<2);//Port A
	
	// PA9 = TX
  GPIOA->CRH &= ~(0xF << 4);
  GPIOA->CRH |=  (0xB << 4);

  // PA10 = RX
  GPIOA->CRH &= ~(0xF << 8);
  GPIOA->CRH |=  (0x4 << 8);

  USART1->BRR = 7500;//Baud rate: 9600 bps -> BRR= 72MHz/9600
	USART1->CR1 &= ~(1<<12);//8 bit
	USART1->CR1 &= ~(1<<10);//none parity
	USART1->CR2 &= ~(3<<12);//stopbit = 1
	
	USART1->CR1 |= (1<<3);// TE 
	USART1->CR1 |= (1<<2);// RE
	USART1->CR2 &= ~(1<<11);// disable clock == uart
	USART1->CR1 |=(1<<13);//UE
	
	USART1->CR1 |= (1<<5);  // RXNE interrupt enable
	NVIC_EnableIRQ(USART1_IRQn);
}

void Send_char(char c){
	while(!(USART1->SR & (1<<7)));
	USART1->DR = c;
}

void Send_string(char *str){
	while(*str != '\0'){
		Send_char(*str);
		str++;
	}
}

void USART1_IRQHandler(void){
    if(USART1->SR & (1<<5)){        // RXNE (bit 5) = 1, có dữ liệu
        char c = USART1->DR;        // Ðọc ký tự từ thanh ghi dữ liệu

        if(c == '1'){
            GPIOC->BSRR = ~(1<<13);  // Bật LED
        } else if(c == '0'){
            GPIOC->BRR = (1<<13);   // Tắt LED
        } else if(c == '?'){
            if(GPIOC->ODR & (1<<13)){
                Send_string("Current LED state: ON\r\n");
            } else {
                Send_string("Current LED state: OFF\r\n");
            }
        }
    }
}
int main(void){
	GPIO_Init();
	UART1_Init();
	while(1){
		
    }
}

