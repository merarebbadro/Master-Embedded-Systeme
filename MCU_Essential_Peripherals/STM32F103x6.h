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

#define FLASH_BASE 		0x08000000UL
#define SRAM_BASE               0x20000000UL
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
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;

}RCC_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: GPIO
//-*-*-*-*-*-*-*-*-*-*-*
typedef struct
{
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;

}GPIO_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: AFIO
//-*-*-*-*-*-*-*-*-*-*-*

typedef struct
{
	volatile uint32_t    EVCR;
	volatile uint32_t    MAPR;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;	 //at 0x14
	uint32_t 	Reserved0;      //at 0x18
	volatile uint32_t   MAPR2;	 //at 0x1C

}AFIO_TypeDef;

//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: EXTI
//-*-*-*-*-*-*-*-*-*-*-*

typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RSTR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;

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
//Interrupt Vector Table:
//-*-*-*-*-*-*-*-*-*-*-*
/*
 * EXTI
 */
#define EXTI0_IRQ		6
#define EXTI1_IRQ		7
#define EXTI2_IRQ		8
#define EXTI3_IRQ		9
#define EXTI4_IRQ		10
#define EXTI5_IRQ		23
#define EXTI6_IRQ		23
#define EXTI7_IRQ		23
#define EXTI8_IRQ		23
#define EXTI9_IRQ		23
#define EXTI10_IRQ		40
#define EXTI11_IRQ		40
#define EXTI12_IRQ		40
#define EXTI13_IRQ		40
#define EXTI14_IRQ		40
#define EXTI15_IRQ		40

//-*-*-*-*-*-*-*-*-*-*-*--*-*-*--*-*-*-
//NVIC IRQ Enable/Disable Macros
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-

/*  Enable Interrupt Requests  */

// We Started From IRQ6 Due To DataSheet
// ISER : Interrupt Set Register

#define NVIC_IRQ6_EXTI0_EN()			(NVIC_ISER0 |= 1<<6)				// EXTI0 ---> PIN6 in NVIC
#define NVIC_IRQ7_EXTI1_EN()			(NVIC_ISER0 |= 1<<7)				// EXTI1 ---> PIN7 in NVIC
#define NVIC_IRQ8_EXTI2_EN()			(NVIC_ISER0 |= 1<<8)				// EXTI2 ---> PIN8 in NVIC
#define NVIC_IRQ9_EXTI3_EN()			(NVIC_ISER0 |= 1<<9)				// EXTI3 ---> PIN9 in NVIC
#define NVIC_IRQ10_EXTI4_EN()			(NVIC_ISER0 |= 1<<10)				// EXTI4 ---> PIN10 in NVIC
#define NVIC_IRQ23_EXTI5_9_EN()			(NVIC_ISER0 |= 1<<23)				// EXTI5, EXTI6, EXTI7, EXTI8, EXTI9 ---> PIN23 in NVIC

// 40 - 32 = 8
#define NVIC_IRQ40_EXTI10_15_EN()		(NVIC_ISER1 |= 1<<8)				// EXTI10, EXTI11, EXTI12, EXTI13, EXTI14, EXTI15


/* Disable Interrupt Requests */

// We Started From IRQ6 Due To DataSheet
// ICER : Interrupt Clear Register

#define NVIC_IRQ6_EXTI0_DI()			(NVIC_ICER0 |= 1<<6)				// EXTI0 ---> PIN6 in NVIC
#define NVIC_IRQ7_EXTI1_DI()			(NVIC_ICER0 |= 1<<7)				// EXTI1 ---> PIN7 in NVIC
#define NVIC_IRQ8_EXTI2_DI()			(NVIC_ICER0 |= 1<<8)				// EXTI2 ---> PIN8 in NVIC
#define NVIC_IRQ9_EXTI3_DI()			(NVIC_ICER0 |= 1<<9)				// EXTI3 ---> PIN9 in NVIC
#define NVIC_IRQ10_EXTI4_DI()			(NVIC_ICER0 |= 1<<10)				// EXTI4 ---> PIN10 in NVIC
#define NVIC_IRQ23_EXTI5_9_DI()			(NVIC_ICER0 |= 1<<23)				// EXTI5, EXTI6, EXTI7, EXTI8, EXTI9 ---> PIN23 in NVIC

// 40 - 32 = 8
#define NVIC_IRQ40_EXTI10_15_DI()		(NVIC_ICER1 |= 1<<8)				// EXTI10, EXTI11, EXTI12, EXTI13, EXTI14, EXTI15
																			//  ---> PIN40 in NVIC

//-*-*-*-*-*-*-*-*-*-*-*-
//Generic Macros:
//-*-*-*-*-*-*-*-*-*-*-*



#endif /* STM32F103X6_H_ */
