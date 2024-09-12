/*
 * Stm32_F103C6_EXTI_Driver.c
 *
 *  Created on: 12/09/2024
 *      Author: Merareb_Badreddine
 */


#include "Stm32_F103C6_EXTI_Driver.h"
#include "Stm32_F103C6_GPIO_Driver.h"





/*
 *=========================================================================
 *							Generic Variables
 * ========================================================================
*/

void (* GP_IRQ_CallBack[16])(void) ;



/*
 *=========================================================================
 *							Generic Macros
 * ========================================================================
*/

#define AFIO_GPIO_EXTI_Mapping(x)	(	(x==GPIOA)?0:\
										(x==GPIOB)?1:\
										(x==GPIOC)?2:\
										(x==GPIOD)?3:0	)


/*
 *=========================================================================
 *							Generic Functions
 * ========================================================================
*/

void Enable_NVIC ( uint16_t IRQ)
{
	switch (IRQ) {
		case 0:
			NVIC_IRQ6_EXTI0_Enable ;
			break;
		case 1:
			NVIC_IRQ7_EXTI1_Enable ;
			break;
		case 2:
			NVIC_IRQ8_EXTI2_Enable ;
			break;
		case 3:
			NVIC_IRQ9_EXTI3_Enable ;
			break;
		case 4:
			NVIC_IRQ10_EXTI4_Enable ;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			NVIC_IRQ23_EXTI5_9_Enable ;
			break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			NVIC_IRQ40_EXTI10_15_Enable	;
			break;
	}
}

void Disable_NVIC ( uint16_t IRQ)
{
	switch (IRQ) {
		case 0:
			NVIC_IRQ6_EXTI0_Disable ;
			break;
		case 1:
			NVIC_IRQ7_EXTI1_Disable ;
			break;
		case 2:
			NVIC_IRQ8_EXTI2_Disable ;
			break;
		case 3:
			NVIC_IRQ9_EXTI3_Disable ;
			break;
		case 4:
			NVIC_IRQ10_EXTI4_Disable ;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			NVIC_IRQ23_EXTI5_9_Disable ;
			break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			NVIC_IRQ40_EXTI10_15_Disable	;
			break;
	}
}



void Update_EXTI(EXTI_PinConfig_t * EXTI_Config)
{
	//1- Configure GPIO to be AF Input -> Floating INPUT
	GPIO_PinConfig_t PinCfg ;

	PinCfg.GPIO_PinNumber = EXTI_Config->EXTI_PIN.GPIO_Pin;
	PinCfg.GPIO_Mode = GPIO_Mode_AF_Input;
	MCAL_GPIO_Init(EXTI_Config->EXTI_PIN.GPIO_Port, &PinCfg);

	//===================================================
	//2 UPDATE AFIO to Route between EXTI Line with PORT A, B, C, D

//	GPIO_TypeDef* Port_List[4] ={GPIOA,GPIOB,GPIOC,GPIOD};
//	while(Port_List[i] != EXTI_Config->EXTI_PIN.ETI_InputLineNumber)
//	{
//		i++;
//	}


	uint8_t AFIO_EXTICR_Index = EXTI_Config->EXTI_PIN.EXTI_InputLineNumber / 4 ;
	uint8_t AFIO_EXTICR_Position = (EXTI_Config->EXTI_PIN.EXTI_InputLineNumber % 4) * 4 ;

	//clear the four bit
	AFIO->EXTICR[AFIO_EXTICR_Index] &= ~(0xF << AFIO_EXTICR_Position) ;

	AFIO->EXTICR[AFIO_EXTICR_Index] |=((AFIO_GPIO_EXTI_Mapping(EXTI_Config->EXTI_PIN.GPIO_Port) &0xF) << AFIO_EXTICR_Position) ;

	//===========================================================

	//3- Update Rising or Falling Register
	EXTI->RTSR &= ~(1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);
	EXTI->FTSR &= ~(1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);

	switch(EXTI_Config->Trigger_Case)
	{
	case EXTI_Trigger_Rising :
		EXTI->RTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber) ;
		break ;
	case EXTI_Trigger_Falling :
		EXTI->FTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber) ;
		break ;
	default :
		EXTI->RTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber) ;
		EXTI->FTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber) ;
		break ;
	}

	//===========================================================

	//4- Update IRQ Handling CALLBACK

	GP_IRQ_CallBack[EXTI_Config->EXTI_PIN.EXTI_InputLineNumber] = EXTI_Config->P_IRQ_CallBack ;


	//=============================================
	//5- Enable/Disable IRQ EXTI and NVIC

	if (EXTI_Config->IRQ_EN == EXTI_IRQ_Enable )
	{
		EXTI->IMR |=  (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber) ;
		Enable_NVIC ( EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);

	}else
	{
		EXTI->IMR &= ~(1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber) ;
		Disable_NVIC ( EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);

	}


}













/**================================================================
 * @Fn				-MCAL_EXTI_GPIO_Init
 * @brief 			-this is used to initialize EXTI from Specific GPIO PIN and specify the mask/Trigger Condition and IRQ CallBack
 * @param [in] 		-EXTI_Config : set by @re EXTI_define, EXTI_Trigger_define and EXTI_IRQ_define
 * @retval 			-none
 * Note				-Stm32F103C6 MCU has GPIO A,B,C,D,E Modules
 * 					but LQFP48 package has only GPIO A,B PART of C/D exported as external PINS from the MCU
 * 					-Also Mandatory to Enable RCC for AFIO and the corresponding GPIO
 */

void MCAL_EXTI_GPIO_Init(EXTI_PinConfig_t * EXTI_Config)
{

	Update_EXTI(EXTI_Config) ;

}





/**================================================================
 * @Fn				-MCAL_EXTI_GPIO_DeInit
 * @brief 			-reset EXTI Registers and NVIC corresponding IRQ Mask
 * @retval 			-none
 * Note				-none
 */

void MCAL_EXTI_GPIO_DeInit(void)
{
	EXTI->IMR= 0x00000000;
	EXTI->EMR= 0x00000000;
	EXTI->RTSR= 0x00000000;
	EXTI->FTSR= 0x00000000;
	EXTI->SWIER= 0x00000000;
	//rc_w1 cleared by writing "1" into the bits
	EXTI->PR= 0xFFFFFFFF;

	//disable EXTI IRQ From NVIC
	NVIC_IRQ6_EXTI0_Disable	     ;
	NVIC_IRQ7_EXTI1_Disable	  	 ;
	NVIC_IRQ8_EXTI2_Disable		 ;
	NVIC_IRQ9_EXTI3_Disable	     ;
	NVIC_IRQ10_EXTI4_Disable	 ;
	NVIC_IRQ23_EXTI5_9_Disable	 ;
	NVIC_IRQ40_EXTI10_15_Disable ;

}



/**================================================================
 * @Fn				-MCAL_EXTI_GPIO_Update
 * @brief 			-this is used to initialize EXTI from Specific GPIO PIN and specify the mask/Trigger Condition and IRQ CallBack
 * @param [in] 		-EXTI_Config : set by @re EXTI_define, EXTI_Trigger_define and EXTI_IRQ_define
 * @retval 			-none
 * Note				-Stm32F103C6 MCU has GPIO A,B,C,D,E Modules
 * 					but LQFP48 package has only GPIO A,B PART of C/D exported as external PINS from the MCU
 * 					-Also Mandatory to Enable RCC for AFIO and the corresponding GPIO
 */

void MCAL_EXTI_GPIO_Update(EXTI_PinConfig_t * EXTI_Config)
{
	Update_EXTI(EXTI_Config) ;
}



/*
 *  *=========================================================================
 *=========================================================================
 *==================		APIs Functions Definitions		=============
 * ========================================================================
*/

void EXTI0_IRQHandler (void)
{
	//cleared by writing a 1 into the bit Pending register (EXTI_PR)
	EXTI->PR |= (1<<0) ;
	//CALL IRQ_CALL
	GP_IRQ_CallBack[0]();
}

void EXTI1_IRQHandler (void)
{
	EXTI->PR |= (1<<1);
	GP_IRQ_CallBack[1]();
}

void EXTI2_IRQHandler (void)
{
	EXTI->PR |= (1<<2) ;
	GP_IRQ_CallBack[2]();
}

void EXTI3_IRQHandler (void)
{
	EXTI->PR |= (1<<3) ;
	GP_IRQ_CallBack[3]();
}

void EXTI4_IRQHandler (void)
{
	EXTI->PR |= (1<<4) ;
	GP_IRQ_CallBack[4]();
}

void EXTI9_5_IRQHandler (void)
{

	uint8_t i = 5 ;
	while(!(EXTI->PR &=1<<i))
	{
		i++ ;
	}
	EXTI->PR |= (1<<i);
	GP_IRQ_CallBack[i]() ;

}

void EXTI15_10_IRQHandler (void)
{
	uint8_t i = 10 ;
	while(!(EXTI->PR &=1<<i))
	{
		i++ ;
	}
	EXTI->PR |= (1<<i);
	GP_IRQ_CallBack[i]() ;

}
