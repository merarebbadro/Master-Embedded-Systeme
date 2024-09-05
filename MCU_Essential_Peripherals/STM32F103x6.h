/*
 * STM32F103x6.h
 *
 *  Created on: 05/09/2024
 *      Author: Merareb_Badreddine
 */

#ifndef STM32F103X6_H_
#define STM32F103X6_H_

//-----------------------------
//Includes
//-----------------------------

#include "stdlib.h"
#include "stdint.h"

//-----------------------------
//Base addresses for Memories
//-----------------------------

#define FLASH_BASE 		   	0x08000000UL
#define SRAM_BASE			    0x20000000UL
#define PERIPHERALS_BASE	0x40000000UL

#define APB1_BUS_BASE     	(PERIPHERALS_BASE)
#define APB2_BUS_BASE     	(PERIPHERALS_BASE + 0x00010000UL)
#define AHB_BUS_BASE      	(PERIPHERALS_BASE + 0x00020000UL)

//-----------------------------
//Base addresses for AHB Peripherals
//-----------------------------

#define RCC_BASE            (AHB_BUS_BASE + 0x00001000UL)

//-----------------------------
//Base addresses for APB2 Peripherals
//-----------------------------

#define AFIO_BASE           (APB2_BUS_BASE + 0x00000000UL)
#define EXTI_BASE           (APB2_BUS_BASE + 0x00000400UL)
#define GPIOA_BASE          (APB2_BUS_BASE + 0x00000800UL)
#define GPIOB_BASE          (APB2_BUS_BASE + 0x00000C00UL)
#define GPIOC_BASE          (APB2_BUS_BASE + 0x00001000UL)
#define GPIOD_BASE          (APB2_BUS_BASE + 0x00001400UL)
#define GPIOE_BASE          (APB2_BUS_BASE + 0x00001800UL)

//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: RCC
//-*-*-*-*-*-*-*-*-*-*-*

typedef struct
{
	 uint32_t CR;
	 uint32_t CFGR;
	 uint32_t CIR;
	 uint32_t APB2RSTR;
	 uint32_t APB1RSTR;
	 uint32_t AHBENR;
	 uint32_t APB2ENR;
	 uint32_t APB1ENR;
	 uint32_t BDCR;
}RCC_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: GPIO
//-*-*-*-*-*-*-*-*-*-*-*

typedef struct
{
	 uint32_t CRL;
	 uint32_t CRH;
	 uint32_t IDR;
	 uint32_t ODR;
	 uint32_t BSRR;
	 uint32_t BRR;
	 uint32_t LCKR;
}GPIO_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: AFIO
//-*-*-*-*-*-*-*-*-*-*-*

typedef struct
{
	 uint32_t EVCR;
	 uint32_t MAPR;
	 uint32_t EXTICR0;
	 uint32_t EXTICR1;
	 uint32_t EXTICR2;
	 uint32_t EXTICR3;
	 uint32_t  RESERVED0;
	 uint32_t MAPR2;
}AFIO_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: EXTI
//-*-*-*-*-*-*-*-*-*-*-*

typedef struct
{
	 uint32_t IMR;
	 uint32_t EMR;
	 uint32_t RTSR;
	 uint32_t FTSR;
	 uint32_t SWIER;
	 uint32_t PR;
}EXTI_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral Instants:
//-*-*-*-*-*-*-*-*-*-*-*

#define GPIOA			((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB			((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOC			((GPIO_TypeDef*)GPIOC_BASE)
#define GPIOD			((GPIO_TypeDef*)GPIOD_BASE)
#define GPIOE			((GPIO_TypeDef*)GPIOE_BASE)

#define RCC			((RCC_TypeDef*)RCC_BASE)

#define EXTI			((EXTI_TypeDef*)EXTI_BASE)

#define AFIO			((AFIO_TypeDef*)AFIO_BASE)

//-*-*-*-*-*-*-*-*-*-*-*-
//clock enable Macros:
//-*-*-*-*-*-*-*-*-*-*-*

#define RCC_GPIOA_CLK_EN()	      (RCC->APB2ENR|= 1<<2)
#define RCC_GPIOB_CLK_EN()            (RCC->APB2ENR|= 1<<3)
#define RCC_GPIOC_CLK_EN()	      (RCC->APB2ENR|= 1<<4)
#define RCC_GPIOD_CLK_EN()	      (RCC->APB2ENR|= 1<<5)
#define RCC_GPIOE_CLK_EN()	      (RCC->APB2ENR|= 1<<6)

#define AFIO_CLK_EN()		      (RCC->APB2ENR|= 1<<0))

//-*-*-*-*-*-*-*-*-*-*-*-
//Generic Macros:
//-*-*-*-*-*-*-*-*-*-*-*



#endif /* STM32F103X6_H_ */
