/*
 * STM32_F103C6_GPIO_Driver.c
 *
 *  Created on: 06/09/2024
 *      Author: Merareb_badreddine
 */

#include "STM32_F103C6_GPIO_Driver.h"

//Functions Local to driver
uint8_t Get_CRLH_Position (uint16_t PinNumber)
{
	switch (PinNumber)
	{
	case GPIO_PIN_0:
	{
		return 0;
		break;
	}
	case GPIO_PIN_1:
	{
		return 4;
		break;
	}
	case GPIO_PIN_2:
	{
		return 8;
		break;
	}
	case GPIO_PIN_3:
	{
		return 12;
		break;
	}
	case GPIO_PIN_4:
	{
		return 16;
		break;
	}
	case GPIO_PIN_5:
	{
		return 20;
		break;
	}
	case GPIO_PIN_6:
	{
		return 24;
		break;
	}
	case GPIO_PIN_7:
	{
		return 28;
		break;
	}
	case GPIO_PIN_8:
	{
		return 0;
		break;
	}
	case GPIO_PIN_9:
	{
		return 4;
		break;
	}
	case GPIO_PIN_10:
	{
		return 8;
		break;
	}
	case GPIO_PIN_11:
	{
		return 12;
		break;
	}
	case GPIO_PIN_12:
	{
		return 16;
		break;
	}
	case GPIO_PIN_13:
	{
		return 20;
		break;
	}
	case GPIO_PIN_14:
	{
		return 24;
		break;
	}
	case GPIO_PIN_15:
	{
		return 28;
		break;
	}
	}
	return 0;
}

/**================================================================
 * @Fn			-MCAL_GPIO_Init
 * @brief 		-Initialize GPIOx PINy according to the specific parameters in the pinconfig
 * @param [in] 	-GPIOx: where x can be (A-E) to select GPIO Peripheral
 * @param [in] 	-PinConfig ptr to a GPIO_PinConfig_t struct that contains the configuration information for specified GPIO PIN
 * @retval 		-none
 * Note			-STM32F103C6 MCU has GPIO A,B,C,D,E Modules
 * 					but LQFP48 package has only GPIO A,B and part of C,D exported as external PINs from the MCU
 */

void MCAL_GPIO_Init (GPIO_TypeDef* GPIOx, GPIO_PinConfig_t* PinConfig)
{
	//Port configuration register low (GPIOx_CRL)  - configure pins 0 >> 7
	//Port configuration register high (GPIOx_CRH) - configure pins 8 >> 15

	volatile uint32_t* ConfigRegister = NULL;
	uint8_t PIN_Config = 0;

	ConfigRegister = (PinConfig->GPIO_PinNumber < GPIO_PIN_8)? &GPIOx->CRL : &GPIOx->CRH ;

	// clear CNFn[1:0] MODEn[1:0]
	(*ConfigRegister)&=~(0xF << Get_CRLH_Position(PinConfig->GPIO_PinNumber));

	//if pin is output
	if((PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_AF_OD) || (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_AF_PP) || (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_OD) || (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_PP))
	{
		// set CNFn[1:0] MODEn[1:0]
		PIN_Config = (((PinConfig->GPIO_MODE - 4) << 2) | PinConfig->GPIO_OUTPUT_Speed) & 0x0F;
	}
	//if pin is input
	else
	{
		if(PinConfig->GPIO_MODE == GPIO_MODE_INPUT_Analog || PinConfig->GPIO_MODE == GPIO_MODE_INPUT_FLO)
		{
			// set CNFn[1:0] - MODEn[1:0]= 00
			PIN_Config =  ((PinConfig->GPIO_MODE << 2) | 0x0) & 0x0F;
		}
		else if (PinConfig->GPIO_MODE == GPIO_MODE_AF_INPUT)
		{
			// set CNFn[1:0] - MODEn[1:0]= 00
			// same as Floating input
			PIN_Config =  ((GPIO_MODE_INPUT_FLO << 2) | 0x0) & 0x0F;

		}
		else //PU PD Input
		{
			PIN_Config =  ((GPIO_MODE_INPUT_PU << 2) | 0x0) & 0x0F;

			if (PinConfig->GPIO_MODE == GPIO_MODE_INPUT_PU)
			{
				//PxODR = 1 Input pull-up: Table 20. Port bit configuration table
				GPIOx->ODR |= PinConfig->GPIO_PinNumber; //PinNumber shifted in macro
			}
			else //GPIO_MODE == GPIO_MODE_INPUT_PD
			{
				//PxODR = 0 Input pull-down: Table 20. Port bit configuration table
				GPIOx->ODR &= ~PinConfig->GPIO_PinNumber;
			}
		}
	}
	//write on CRL or CRH
	(*ConfigRegister) |= (PIN_Config << Get_CRLH_Position(PinConfig->GPIO_PinNumber));

}

/**================================================================
 * @Fn			-MCAL_GPIO_DeInit
 * @brief 		-Reset all GPIO registers
 * @param [in] 	-GPIOx: where x can be (A-E) to select GPIO Peripheral
 * @retval 		-none
 * Note			-none
 */

void MCAL_GPIO_DeInit (GPIO_TypeDef* GPIOx)
{

	//Can reset GPIOx registers manually
	//	GPIOx->BRR  = 0x00000000;
	//	GPIOx->BSRR = 0x00000000;
	//	GPIOx->CRH  = 0x44444444;
	//	GPIOx->CRL  = 0x44444444;
	//	//GPIOx->IDR = 0x00000000; (read only)
	//	GPIOx->LCKR
	//	GPIOx->ODR  = 0x00000000;

	// Or use the Reset Controller
	//GPIO peripherals are found on APB2 BUS
	//APB2 peripheral reset register (RCC_APB2RSTR)
	//Set and clear by software

	if (GPIOx == GPIOA)
	{
		RCC->APB2RSTR |= (1 << 2);	//Bit 2 IOPARST: I/O port A reset
		RCC->APB2RSTR &= ~(1 << 2);
	}
	else if (GPIOx == GPIOB)
	{
		RCC->APB2RSTR |= (1 << 3);	//Bit 3 IOPBRST: IO port B reset
		RCC->APB2RSTR &= ~(1 << 3);
	}
	else if (GPIOx == GPIOC)
	{
		RCC->APB2RSTR |= (1 << 4);	//Bit 4 IOPCRST: IO port C reset
		RCC->APB2RSTR &= ~(1 << 4);
	}
	else if (GPIOx == GPIOD)
	{
		RCC->APB2RSTR |= (1 << 5);	//Bit 5 IOPDRST: I/O port D reset
		RCC->APB2RSTR &= ~(1 << 5);
	}
	else if (GPIOx == GPIOE)
	{
		RCC->APB2RSTR |= (1 << 6);	//Bit 6 IOPERST: I/O port E reset
		RCC->APB2RSTR &= ~(1 << 6);
	}

}

/**================================================================
 * @Fn			-MCAL_GPIO_ReadPin
 * @brief 		-Read specific PIN
 * @param [in] 	-GPIOx: where x can be (A-E) to select GPIO Peripheral
 * @param [in] 	-PinNumber: Set Pin Number according to @ref GPIO_PINS_define
 * @retval 		-the input pin value (two values based on @ref GPIO_PIN_State)
 * Note			-none
 */

uint8_t MCAL_GPIO_ReadPin (GPIO_TypeDef* GPIOx, uint16_t PinNumber)
{
	uint8_t pin_value;
	if(((GPIOx->IDR) & PinNumber) != GPIO_PIN_RESET)
	{
		pin_value = GPIO_PIN_SET;
	}
	else
	{
		pin_value = GPIO_PIN_RESET;
	}
	return pin_value;
}

/**================================================================
 * @Fn			-MCAL_GPIO_ReadPort
 * @brief 		-Read the input Port value
 * @param [in] 	-GPIOx: where x can be (A-E) to select GPIO Peripheral
 * @retval 		-the input port value
 * Note			-none
 */

uint16_t MCAL_GPIO_ReadPort (GPIO_TypeDef* GPIOx)
{
	uint16_t port_value;
	port_value = (uint16_t)(GPIOx->IDR); //type cast as register size is 32-bit
	return port_value;

}

/**================================================================
 * @Fn			-MCAL_GPIO_WritePin
 * @brief 		-Write specific PIN
 * @param [in] 	-GPIOx: where x can be (A-E) to select GPIO Peripheral
 * @param [in] 	-PinNumber: Set Pin Number according to @ref GPIO_PINS_define
 * @param [in]	-Value: Pin Value
 * @retval 		-none
 * Note			-none
 */

void MCAL_GPIO_WritePin (GPIO_TypeDef* GPIOx, uint16_t PinNumber, uint8_t value)
{
	if (value != GPIO_PIN_RESET)
	{
		//		GPIOx->ODR |= PinNumber;	//read ODR, OR, write to ODR
		//		or
		//		Bits 15:0 BSy: Port x Set bit y (y= 0 .. 15)
		//		These bits are write-only and can be accessed in Word mode only.
		//		0: No action on the corresponding ODRx bit
		//		1: Set the corresponding ODRx bit
		GPIOx->BSRR = (uint32_t)PinNumber;	//write directly without read
	}
	else
	{
		//GPIOx->ODR &= ~PinNumber;

		//		Bits 15:0 BRy: Port x Reset bit y (y= 0 .. 15)
		//		These bits are write-only and can be accessed in Word mode only.
		//		0: No action on the corresponding ODRx bit
		//		1: Reset the corresponding ODRx bit
		GPIOx->BRR = (uint32_t)PinNumber;
	}
}

/**================================================================
 * @Fn			-MCAL_GPIO_WritePort
 * @brief 		-Write the output Port value
 * @param [in] 	-GPIOx: where x can be (A-E) to select GPIO Peripheral
 * @param [in]	-Value: Port Value
 * @retval 		-none
 * Note			-none
 */

void MCAL_GPIO_WritePort (GPIO_TypeDef* GPIOx, uint16_t value)
{
	GPIOx->ODR = (uint32_t)value;
}

/**================================================================
 * @Fn			-MCAL_GPIO_TogglePin
 * @brief 		-Toggle specific GPIO Pin
 * @param [in] 	-GPIOx: where x can be (A-E) to select GPIO Peripheral
 * @param [in] 	-PinNumber: Set Pin Number according to @ref GPIO_PINS_define
 * @retval 		-none
 * Note			-none
 */

void MCAL_GPIO_TogglePin (GPIO_TypeDef* GPIOx, uint16_t PinNumber)
{
	GPIOx->ODR ^= (uint32_t)PinNumber;
}

/**================================================================
 * @Fn			-MCAL_GPIO_LockPin
 * @brief 		-The locking mechanism allows the IO configuration to be frozen
 * @param [in] 	-GPIOx: where x can be (A-E) to select GPIO Peripheral
 * @param [in] 	-PinNumber: Set Pin Number according to @ref GPIO_PINS_define
 * @retval 		-Enabled if pin config is locked or ERROR if pin not locked (Enabled & ERROR are defined @ref GPIO_RETURN_LOCK)
 * Note			-none
 */

uint8_t MCAL_GPIO_LockPin (GPIO_TypeDef* GPIOx, uint16_t PinNumber)
{
	//	Bit 16 LCKK[16]: Lock key
	//	This bit can be read anytime. It can only be modified using the Lock Key Writing Sequence.
	//	0: Port configuration lock key not active
	//	1: Port configuration lock key active. GPIOx_LCKR register is locked until the next reset.
	//	LOCK key writing sequence:
	//	Write 1
	//	Write 0
	//	Write 1
	//	Read 0
	//	Read 1 (this read is optional but confirms that the lock is active)
	//	Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
	//	Any error in the lock sequence will abort the lock.

	//Each lock bit freezes the corresponding 4 bits of the control register (CRL, CRH).

	//	Bits 15:0 LCKy: Port x Lock bit y (y= 0 .. 15)
	//	These bits are read write but can only be written when the LCKK bit is 0.
	//	0: Port configuration not locked
	//	1: Port configuration locked

	//set LCKK[16]
	volatile uint32_t temp = 1<<16;
	//set LCKy
	temp |= PinNumber;
	//	LOCK key writing sequence:
	//	Write 1
	GPIOx->LCKR = temp;
	//	Write 0
	GPIOx->LCKR = PinNumber;
	//	Write 1
	GPIOx->LCKR = temp;
	//	Read 0
	temp = GPIOx->LCKR;
	//	Read 1 (this read is optional but confirms that the lock is active)
	if ((uint32_t)((GPIOx->LCKR) & (1<<16)))
	{
		return GPIO_RETURN_LOCK_Enabled;
	}
	else
	{
		return GPIO_RETURN_LOCK_ERROR;
	}
}
