#include "calculator.h"
#include "UART.h"
#include "Keypad.h"


const char KEYPAD_MAP[16] = {
    '1', '2', '3', '+',
    '4', '5', '6', '-',
    '7', '8', '9', '*',
    'C', '0', '=', '/'
};



// Function to evaluate an arithmetic expression
int EvaluateExpression(uint8_t num1, char operator, uint8_t num2)
{
    switch (operator)
    {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return (num2 != 0) ? num1 / num2 : 0; // Avoid division by zero
        default: return 0; // Invalid operator
    }
}



void Calculator(void)
{
    uint8_t num1 = 0, num2 = 0;
    char operator = '\0';
    uint8_t key = 0;
    int result = 0;
    int state = 0;
		int num_print = 0;

    UART0_Output_String("Calculator ready. Use keypad to enter values.\r\n");

    while (1)
    {
        key = Get_EduBase_Keypad_Status(); // Get the key 
        if (key != 0) // 
        {
            char mapped_key = KEYPAD_MAP[key - 1]; 
            //UART0_Output_String("\r\nDetected Key: ");
            //UART0_Output_Character(mapped_key);
            //UART0_Output_Newline();

            if (mapped_key >= '0' && mapped_key <= '9') // Digit key
            {
                if (state == 0) // Inputting num1
                {
										num_print = 1;
                    num1 = (num1 * 10) + (mapped_key - '0');
										while (num_print == 1){
											UART0_Output_String("\r\nNUM1: ");
											UART0_Output_Unsigned_Decimal(num1);
											num_print = 0;
										}
										state = 1;

                }
                else if (state == 2) // Inputting num2
                {
										num_print = 1;
                    num2 = (num2 * 10) + (mapped_key - '0');
										while (num_print == 1){
											UART0_Output_String("\r\nNUM2: ");
											UART0_Output_Unsigned_Decimal(num2);
											num_print = 0;
										}
										state = 3;
                }
            }
            else if (mapped_key == '+' || mapped_key == '-' || mapped_key == '*' || mapped_key == '/')
            {
                if (state == 1) // Operator after num1
                {
                    operator = mapped_key;
										num_print = 1;
										while (num_print == 1){
											UART0_Output_String("\r\nOperator: ");
											UART0_Output_Character(operator);
											num_print = 0;
										}
                    state = 2; // 
                }
            }
            else if (mapped_key == '=')
            {
                if (state == 3) 
                {
										num_print = 1;
										while (num_print == 1){
											UART0_Output_Newline();
											UART0_Output_Unsigned_Decimal(num1);
											UART0_Output_Character(operator);
											UART0_Output_Unsigned_Decimal(num2);
											result = EvaluateExpression(num1, operator, num2);
											UART0_Output_String("\r\nResult: ");
											UART0_Output_Unsigned_Decimal(result);
											UART0_Output_Newline();
											num1 = 0;
											num2 = 0;
											operator = '\0';
											state = 0;
											num_print = 0;
										}
										break;
                }
                else
                {
                    UART0_Output_String("\r\nError: Incomplete expression.\r\n");
                }
            }
            if (mapped_key == 'C' && num_print == 0)
            {
                num1 = 0;
                num2 = 0;
                operator = '\0';
                state = 0;
								//num_print = 1;
						
								UART0_Output_String("\r\nCleared.\r\n");
								num_print = 2;
									
								
            }
        }
    }
}



