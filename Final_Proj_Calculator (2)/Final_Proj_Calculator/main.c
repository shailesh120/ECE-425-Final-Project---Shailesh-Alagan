#include "UART.h"
#include "Keypad.h"
#include "calculator.h"

void Test_Keypad_Raw(void);

int main(void)
{
		SysTick_Delay_Init();
    UART0_Init();  // Initialize UART
    EduBase_Keypad_Init(); // Initialize keypad
	  //UART0_Output_String("Keypad initialized.\r\n");
		//Test_Keypad();
    Calculator();  // Start the calculator
    return 0;
}


void Test_Keypad(void)
{
    while (1)
    {
        uint8_t key = Get_EduBase_Keypad_Status();
        if (key != 0) // 
        {
            UART0_Output_String("\r\nKey: ");
            UART0_Output_Unsigned_Decimal(key);
            UART0_Output_Newline();
        }
    }
}