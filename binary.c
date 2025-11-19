#include "stm32l4xx.h"                  // Device header

//Instalising global counter variable 
	int i = 0;
//Instalising functions	
	int bitZero(void);
	int bitOne(void);
	int bitTwo(void);
	int bitThree(void);

int main(void){
	
//Enable clock for GPIOA, GPIOB, TIM7
	RCC->AHB2ENR |= 0X3UL;
	RCC->APB1ENR1 |= 0X1UL << 5;
	
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
		GPIOB->PUPDR |= (0X2AUL << 8);
	
//TIM7
	//Setting TIM7 to 3.33Hz (period = 0.3s)
		TIM7->PSC = 1200 - 1;
		TIM7->ARR = 1000 - 1;
		TIM7->CNT = 0;
		TIM7->CR1 |= 0X1UL;	
	
	while(1){
	//If timer 7 flags
		if(TIM7->SR && 0X1UL){
			//Add one the the count variable
				i = i + 1;
			//Resets tiemr 7 status register
				TIM7->SR &= ~0X1UL;
		}
		if(bitZero()){
			//Turns on orange LED
				GPIOA->ODR ^= (0X1UL << 3);
		}	else{
			//Turns off orange LED
				GPIOA->ODR &= ~(0X1UL << 3);
		}
		if(bitOne()){
			//Turns on green LED
				GPIOA->ODR ^= (0X1UL << 4);
		} else{
			//Turns off green LED
				GPIOA->ODR &= ~(0X1UL << 4);
		}
		if(bitTwo()){
			//Turns on yellow LED
				GPIOB->ODR ^= (0X1UL << 4);
		}	else{
			//Turns off yellow LED
				GPIOB->ODR &= ~(0X1UL << 4);
		}
		if(bitThree()){
			//Turns on red LED
				GPIOB->ODR ^= (0X1UL << 5);
		}	else{
			//Turns off red LED
				GPIOB->ODR &= ~(0X1UL << 5);	
		}
	//When counter reaches 16 or reset button is pushed
		if(i == 16 || (GPIOB->IDR & (0X1UL << 6))){
			//Reset counter
				i = 0;
		}
	}
}

//3.33Hz counter
	int bitZero(void){
		return(i%2);
	}

//1.67Hz counter
	int bitOne(void){
		return((i/2)%2);
	}

//0.833Hz counter
	int bitTwo(void){
		return((i/4)%2);
	}

//0.417Hz counter
	int bitThree(void){
		return((i/8)%2);
	}
