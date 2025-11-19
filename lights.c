#include "stm32l4xx.h"                  // Device header



//Instalising global counter variable
	int i = 0;
//Instalising functions	
	int redLightBarrierDown(void);
	int yellowLight(void);
	int greenLight(void);
	int barierDown(void);
	int buttonPressed(void);
	int resetCounter(void);


int main(void){
//Enable clock for GPIOA, GPIOB, TIM7 & TIM1
	RCC->AHB2ENR |= 0X3UL;
	RCC->APB1ENR1 |= 0X1UL << 5;
	RCC->APB2ENR |= 0X1UL << 11;
	
//GPIOA
	//Sets PA3 & PA4 to output
		GPIOA->MODER &= ~0XFFFFUL;
		GPIOA->MODER |= (0X5UL << 6);
	//Sets PA3 & PA4 to pull-down
		GPIOA->PUPDR &= ~0XFFFFUL;
		GPIOA->PUPDR |= (0XAUL << 6);
	
//GPIOB
	//Sets PB4 & PB5 to output
		GPIOB->MODER &= ~0XFFFFUL;
		GPIOB->MODER |= (0X5UL << 8);
	//Sets PB4 & PB5 to pull-down
		GPIOB->PUPDR &= ~0XFFFFUL;
		GPIOB->PUPDR |= (0XAUL << 8);

//TIM7
	//Setting TIM7 to 2Hz
		TIM7->PSC = 2000 - 1;
		TIM7->ARR = 1000 - 1;
		TIM7->CNT = 0;
	//Enabeling TIM7	
		TIM7->CR1 |= 0X1UL;

//PWM
	//Setting up GPIO PA8 for PWM
		GPIOA->MODER &= ~(0X3UL << 16);
		GPIOA->MODER |= (0X1UL << 17);
		GPIOA->AFR[1] &= ~0XFUL;
		GPIOA->AFR[1] |= 0X1UL;

	//Setting TIM1 for PWM
		//Setting it for 50Hz
			TIM1->PSC = 4000 - 1;
			TIM1->ARR = 20 - 1;
			TIM1->CNT = 0;
		//Setting capture compare mode register to output (PWM mode 1)
			TIM1->CCMR1 &= ~0X7UL & ~(0X1UL << 16);
			TIM1->CCMR1 |= (0X3UL << 5);
		//Enabeling capture compare
			TIM1->CCER |= 0X1UL;
		//Setting BDTR register
			TIM1->BDTR |= (0X1UL << 15);
		//Writing 1 to the control register to enable TIM1
			TIM1->CR1 |= 1;



		while(1){
		//If timer 7 flags (every 0.5s)
			if(TIM7->SR & 0X1UL){
			//Adds 1 to the counter variable
				i = i + 1;
			//Resets timer 7 status register
				TIM7->SR &= ~0X1UL;
			}
			if(redLightBarrierDown()){
			//Turns on red LED
				GPIOB->ODR ^= (0X1UL << 5);
			//Barrier Down
				TIM1->CCR1 = 2;
			}else{
			//Turns off red LED
				GPIOB->ODR &= ~(0X1UL << 5);
			//Barrier up
				TIM1->CCR1 = 1;	
			}
			if(yellowLight()){
			//Turns on yellow LED
				GPIOB->ODR ^= (0X1UL << 4);
			}else{
			//Turns off yellow LED
				GPIOB->ODR &= ~(0X1UL << 4);
			}
			if(greenLight()){
			//Turns on green LED
				GPIOA->ODR ^= (0X1UL << 4);
			}else{
			//Turns off green LED
				GPIOA->ODR &= ~(0X1UL << 4);
			}
			if(resetCounter()){
			//Resets counter variable
				i = 0;
			}
			if(GPIOB->IDR & (0X1UL << 6)){
			//Sets counter to 15 for button push condition		
				i = 15;
				}
			if(buttonPressed()){
			//Turns on all LEDs
				GPIOA->ODR ^= (0X3UL << 3);
				GPIOB->ODR ^= (0X3UL << 4);
			}else{
			//Switches off orange LED
				GPIOA->ODR &= ~(0X1UL << 3);
			}
				
		}
}

	
//Conditions to turn on red LED and close the barrier
	int redLightBarrierDown(void){
		return(i <= 5);
	}

//Conditions to turn on yellow LED
	int yellowLight(void){
		return(i == 4||i == 5||i == 13);
	}

//Conditions to turn on green LED
	int greenLight(void){
		return(i <= 12 && i >= 6);
	}

//Conditions to turn reset counter to zero
	int resetCounter(void){
		return(i == 14||i == 20);
	}

//Conditions when button is pressed
	int buttonPressed(void){
		return(i >= 15 && i <= 19);
	}
