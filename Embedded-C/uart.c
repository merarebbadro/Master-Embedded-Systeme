include "uart.h"

/* Registers Address */
/* Uart Registers */
#define UART0DR *((unsigned int* const)((unsigned int*)0x101f1000)) // casting for warning

/* Function Definations */
void Uart_Send_String(unsigned char* P_TX_Sting)
{
	while(*P_TX_Sting != '\0') /* Loop untill end of string */
	{
		UART0DR = (unsigned int)(*P_TX_Sting); // casting char(8) to int(32) // Transmit char
		P_TX_Sting++; // for next character  
	}
}
