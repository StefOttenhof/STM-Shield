/*
 * main.c
 *
 *  Created on: 11 feb. 2021
 *      Author: Stef Ottenhof
 */

#define RCC_AHBENR (*(unsigned int *)(0x40021000 + 0x14))
#define GPIOA_MODER (*(unsigned int *)(0x48000000 + 0x00))
#define GPIOB_MODER (*(unsigned int *)(0x48000400 + 0x00))
#define GPIOB_PUPDR (*(unsigned int *)(0x48000400 + 0x0C))
#define GPIOB_ODR (*(unsigned int *)(0x48000400 + 0x14))
#define GPIOB_IDR (*(unsigned int *)(0x48000400 + 0x10))
const int ledsArray[4] = { 3, 5, 4, 6 }; // Contains pin numbers ordered from left to right
unsigned int pos;
unsigned int buttonState;
unsigned int mode;

void shieldConfig(){
	/**
	 * Configures the I/O-ports used by the I/O-shield
	 */

	// Enable clock port A and B
	RCC_AHBENR |= (1 << 17);
	RCC_AHBENR |= (1 << 18);

	// Set pin 3 on port B to Output
	GPIOB_MODER |= (1 << 6);
	GPIOB_MODER &= ~(1 << (7 * 2));

	// Set pin 5 on port B to Output
	GPIOB_MODER |= (1 << 10);
	GPIOB_MODER &= ~(1 << (11 * 2));

	// Set pin 4 on port B to Output
	GPIOB_MODER |= (1 << 8);
	GPIOB_MODER &= ~(1 << (9 * 2));

	// Set pin 6 on port B to Output
	GPIOB_MODER |= (1 << 12);
	GPIOB_MODER &= ~(1 << (13 * 2));

	// Set pin 8 on port B to Input
	GPIOB_MODER |= (1 << 16);
	GPIOB_MODER &= ~(1 << 17);

	// set pin 8 on port B to pull-up
	GPIOB_PUPDR |= (1 << 16);
	GPIOB_PUPDR &= ~(1 << 17);

}

void ledWrite(int num, int on){
	/**
	* Turns led on or off based on index
	*/
	if(on == 1){
		GPIOB_ODR |= (1 << ledsArray[num]);
	} else {
		GPIOB_ODR &= ~(1 << ledsArray[num] );
	}
}

void buttonRead(){
	/**
	* Reads value from button pin and changes mode based on press
	*/
	  int idr_val = ~GPIOB_IDR;
	  if (idr_val & (1 << 8)) {
	    // The button is pressed; if it was not already pressed, change the mode
	    if (!buttonState) {
	      mode = !mode;
	    }
	    buttonState = 1;
	  }
	  else {
	    buttonState = 0;
	  }
}

void delay(){
	/**
	* Creates a delay based on the assembly "NO OPERATION" instruction
	*/
	for(int i = 0; i < 100; i++){
		buttonRead();
		for(int j = 0; j < 1000; j++){
			asm("nop");
		}
	}
}



void loopLeft(){
	/**
	* Loops through array off leds starting at the first led
	*/
	for(int i = 0; i < 4; i++){
		if(i == 0){
			ledWrite(0, 1);
			ledWrite(3, 0);
		} else {
			ledWrite(i, 1);
			ledWrite(i -1, 0);
		}

		delay();
	}
}

void loopRight(){
	/**
	* Loops through array of leds starting at the last led
	*/
	for(int i = 4; i >= 0; i--){
		if(i == 4){
			ledWrite(4, 1);
			ledWrite(0, 0);
		} else {
			ledWrite(i, 1);
			ledWrite(i + 1, 0);
		}

		delay();
	}
}

int main(){
	shieldConfig();
		while (1)
		{
			if(mode){
				loopRight();
			} else {
				loopLeft();
			}

		}
}

