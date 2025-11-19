#include "stm32l4xx.h"                  // Device header



int i = 0;
int j = 0;

int main(void)
{
	//Enable clock for GPIOA and GPIOB
	RCC->AHB2ENR |= 0x3UL;
	
	//Configure PA_0 and 	PA_1
	GPIOA->MODER &= ~0xFUL;
	GPIOA->MODER |= 0X5UL;
	
	GPIOA->PUPDR &= ~0xFUL;
	GPIOA->PUPDR |= 0xAUL;
	

	
	//Systick Timer Set to run at 12Hz
	SysTick->LOAD = (2000000/12) - 1;
	SysTick->VAL = 0;
	SysTick->CTRL |= 0x5UL;
	


	
	while(1){
		

		
		//If statement for each time the Systick timer resets
		if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
			//Add one to the counter variable every time the Systick timer resets
			i = i + 1;
			j = j + 1;
			//Statement for 3x the Systick time period so 4Hz
			if(i == 3){
				//Sends signal to PA1
				GPIOA->ODR ^= 0X2UL;
				i = 0;
			}
			//Statement for 4x the Systick time period so 3Hz
			if (j == 4){
				//Sends signal to PA0
				GPIOA->ODR ^= 0X1UL;
				//Resets counter variable
				j = 0;
			}
		}

}
}	
