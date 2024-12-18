#include "Keypad.h"



void EduBase_Keypad_Init(void)
{
	// Enable the clock to GPIO Port A by setting the 
	// R0 bit (Bit 0) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x01;
	
	// Enable the clock to GPIO Port D by setting the 
	// R3 bit (Bit 3) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x08;
	
	// Configure the PA5, PA4, PA3, and PA2 pins as input
	// by clearing Bits 5 to 2 in the DIR register
	GPIOA->DIR &= ~0x3C;
	
	// Configure the PA5, PA4, PA3, and PA2 pins to function as
	// GPIO pins by clearing Bits 5 to 2 in the AFSEL register
	GPIOA->AFSEL &= ~0x3C;
	
	// Enable the digital functionality for the PA5, PA4, PA3, and PA2 pins
	// by setting Bits 5 to 2 in the DEN register
	GPIOA->DEN |= 0x3C;	


	// Configure the PD3, PD2, PD1, and PD0 pins to function as
	// GPIO pins by clearing Bits 3 to 0 in the AFSEL register
	GPIOD->AFSEL &= ~0x0F;
	
	// Enable the digital functionality for the PD3, PD2, PD1, and PD0 pins
	// by setting Bits 3 to 0 in the DEN register
	GPIOD->DEN |= 0x0F;
}

uint8_t Get_EduBase_Keypad_Status(void)
{
	int row = 0;
	int col = 0;
	
	// Bit masks used for checking if one of the rows is active
	const uint8_t row_select[] = {0x01, 0x02, 0x04, 0x08};
	
	// Enable all rows
	GPIOD->DIR |= 0x0F;
	
	// Set the data row pins high
	GPIOD->DATA |= 0x0F;
	
	// Read all of the columns
	col = GPIOA->DATA & 0x3C;
	
	// Disable all of the rows
	GPIOD->DIR &= ~0x0F;
	
	// Return 0 if no key has been pressed
	if (col == 0) return 0;
	
	// Loop through each row to determine if a key has been pressed
	// It activates one row at a time and reads the input to see
	// if a specific column is active
	for (row = 0; row < 4; row++)
	{
		// Disable all rows
		GPIOD->DIR &= ~0x0F;
		
		// Enable the currently selected row
		GPIOD->DIR = GPIOD->DIR | row_select[row];
		
		// Turn the active row high and insert a short delay
		GPIOD->DATA |= 0x0F;
		SysTick_Delay1us(1);
		
		// Read all columns and if one of the inputs is low,
		// then one of the keys must be pressed
		col = GPIOA->DATA & 0x3C;
		if (col != 0) break;
	}
	
	GPIOD->DIR &= ~0x0F;
	if (row == 4) return 0;
	
	if (col == 0x04) return (row * 4 + 1); 			// Key in column 0
	else if (col == 0x08) return (row * 4 + 2); // Key in column 1
	else if (col == 0x10) return (row * 4 + 3); // Key in column 2
	else if (col == 0x20) return (row * 4 + 4); // Key in column 3
	else return 0;
}