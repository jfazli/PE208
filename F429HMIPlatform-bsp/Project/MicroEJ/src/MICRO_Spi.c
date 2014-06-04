/*!*******************************************************************************       
 * @file     MICRO_Spi.c
 * @brief    Configuration spi sur STM32F4xx
 * @version  1.0
 * @version  eclipse indgo ; gcc
 * @date     13-09-2013
 * @author   J.Fazli
 * @details   Carte en Master
 *			  Ajout fonction couche basse : SPI_Init
 *			  Modification des noms des fonctions de SPI_xxx en PCSPI_xxx
 *			  La clock du µC est à 168Mhz donc APB2 est à 84Mhz
 *			  Avec un baudrate de 128, la clock sera de 656Khz (APB2 / 128)
 *			  Sortie sur Spi2 sur carte PE165:
 *			  SPI2_SCK 	: PB10
 *			  SPI2_CS	: PB11, non utilisé car carte en master
 *			  SPI2_MISO	: PC2
 *			  SPI2_MOSI	: PC3
 *			  The NSS pin may also be used as an output if enabled (SSOE bit)
 *			  and driven low if the SPI is in master configuration.
*********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_Generic.h"
#include "stm32f4xx_spi.h"
#include "MICRO_Spi.h"
//===== DEFINEs  ================================================================
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================

/*!******************************************************************************
 * @brief       Initialisations des registres du spi2.
 * @param[in]   None
 * @return      None
 ********************************************************************************/
void MICRO_SPI_Init(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC

	/*Enables or disables the Low Speed APB (APB1) peripheral clock*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	/* Enable GPIOA, GPIOB clocks ==> MISO, MOSI, SCK, CS */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/*Configure GPIO in SPI*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);  /* SPI2_SCK */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);  /* SPI1_MOSI */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);  /* SPI1_MISO */
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_SPI2);  /* SPI1_CS */


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  /*SPI2_SCK */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//Init à 0
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//disable pullup or pull down
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;  /*SPI2_MISO,SPI2_MOSI */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//Init à 0
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//disable pullup or pull down
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;     /* SPI2_SS1 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*
	 * Mise à 1 de la pins slave select 1
	 */
	GPIO_SetBits(GPIOD,GPIO_Pin_2);



	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_5;	/*SPI2_ST1*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_9;	/*SPI2_SS2*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*
	 * Mise à 1 de la pins slave select 1
	 */
	GPIO_SetBits(GPIOC,GPIO_Pin_9);

	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_14;	/*SPI2_ST2*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);


	SPI_InitStruct.SPI_Direction 	= SPI_Direction_2Lines_FullDuplex; //Mode full duplex pour fonctionner dans les 2 sens
	SPI_InitStruct.SPI_Mode			  = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize		= SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL			  = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA			  = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS			  = SPI_NSS_Soft; //Slave select gérer par le protocole directement.
	SPI_InitStruct.SPI_BaudRatePrescaler	= 	SPI_BaudRatePrescaler_256;		//Prescaler à revoir car vitesse à 656Khz au lieu de 500khz
	SPI_InitStruct.SPI_FirstBit		= SPI_FirstBit_MSB;
	SPI_Init(SPI3, &SPI_InitStruct);

//	NVIC_InitStructure.NVIC_IRQChannel 						= SPI2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 1;
//	NVIC_Init(&NVIC_InitStructure);
//	SPI_ITConfig(SPI2,SPI_I2S_IT_TXE,ENABLE);

	SPI_Cmd(SPI3, ENABLE);

}

//void SPI2_IRQHandler(void)
//{
//	if(SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_TXE) != RESET )
//	{
//		SPI_I2S_ClearITPendingBit(SPI2,SPI_I2S_IT_TXE);
//		PCUART_Send("interrupt");
//		SPI_ITConfig(SPI2,SPI_I2S_IT_TXE,DISABLE);
////		PCSPI_MasterInterrupt();
//	}
//}

