#include "uart.h"

unsigned char* String_Buffer[100] = "Learn-in-depth: Badro";
//uint8_t const String_Buffer_C[100] = "Learn-in-depth: Badro";

void main(void)
{
	Uart_Send_String(String_Buffer);
}
