/**
  ******************************************************************************
  * @file    stm324xg_eval_lcd.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2012
  * @brief   This file includes the LCD driver for AM480272H3TMQW-T01H 
  *          and AM640480G5TNQW-T00H Liquid Crystal Display Modules
  *          of STM324xG-EVAL evaluation board(MB786) RevB.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm324xx_eval_lcd.h"
#include "fmc_sdram.h"

/** @addtogroup Utilities
  * @{
  */ 

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM324xG_EVAL
  * @{
  */
    
/** @defgroup STM324xG_EVAL_LCD 
  * @brief This file includes the LCD driver for AM480272H3TMQW-T01H 
  *        and AM640480G5TNQW-T00H Liquid Crystal Display Modules
  *        of STM324xG_EVAL board.
  * @{
  */ 

/** @defgroup STM324xG_EVAL_LCD_Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup STM324xG_EVAL_LCD_Private_Defines
  * @{
  */

#define MAX_POLY_CORNERS   200
#define POLY_X(Z)          ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)          ((int32_t)((Points + Z)->Y))
uint32_t Bank1_SDRAM_ADDRESS = 0xC0000000;
uint16_t LCD_PIXEL_WIDTH  = 0x01E0;
uint16_t LCD_PIXEL_HEIGHT = 0x0110;
uint32_t XADDRESS = 0;

/**
  * @}
  */ 

/** @defgroup STM324xG_EVAL_LCD_Private_Macros
  * @{
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X))     
/**
  * @}
  */ 
    
/** @defgroup STM324xG_EVAL_LCD_Private_Variables
  * @{
  */ 
static sFONT *LCD_Currentfonts;

  /* Global variables to set the written text color */
static __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;
static __IO uint32_t Layer = 0x0000;
  
/**
  * @}
  */ 


/** @defgroup STM324xG_EVAL_LCD_Private_FunctionPrototypes
  * @{
  */ 
#ifndef USE_Delay
static void delay(__IO uint32_t nCount);
#endif /* USE_Delay*/
void FMC_SDRAM_LCD_GPIOConfig(void);
static void PutPixel(int16_t x, int16_t y);
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed);


/**
  * @}
  */ 


/** @defgroup STM324xG_EVAL_LCD_Private_Functions
  * @{
  */ 

/**
  * @brief  DeInitializes the LCD.
  * @param  None
  * @retval None
  */
void LCD_DeInit(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< LCD Display Off */
  LCD_DisplayOff();
   
/*-- GPIO Configuration ------------------------------------------------------*/
  /* GPIOD configuration */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_MCO); 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_MCO);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_3  | GPIO_Pin_4  |
                                GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7  | GPIO_Pin_8  |
                                GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12  | 
                                GPIO_Pin_13 |GPIO_Pin_14  | GPIO_Pin_15;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);


  /* GPIOE configuration */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource2 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource3 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_MCO);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 |  
                                GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7 |
                                GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11|
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

  GPIO_Init(GPIOE, &GPIO_InitStructure);


  /* GPIOF configuration */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource0 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource1 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource2 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource3 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource4 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource5 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource6 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource7 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource8 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource9 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource10 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource11 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource12 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource13 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource14 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource15 , GPIO_AF_MCO);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3  | 
                                GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7  | 
                                GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_11 | GPIO_Pin_10 |
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;      

  GPIO_Init(GPIOF, &GPIO_InitStructure);


  /* GPIOG configuration */
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource0 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource1 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource2 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource3 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource4 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource5 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource6 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource7 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource8 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource9 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource10 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource13 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource14 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource15 , GPIO_AF_MCO);
  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 | 
                                GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7 |
                                GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11|
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;        

  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  
  /* GPIOH configuration */
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource2 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource3 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource5 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource6 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource7 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource8 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource9 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource10 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource11 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource12 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource13 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource14 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource15 , GPIO_AF_MCO);
  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  | GPIO_Pin_3  | GPIO_Pin_5  | GPIO_Pin_6  |
                                GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |  
                                GPIO_Pin_15;      

  GPIO_Init(GPIOH, &GPIO_InitStructure);

  /* GPIOI configuration */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource0 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource1 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource2 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource3 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource4 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource5 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource6 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource7 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource9 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource10 , GPIO_AF_MCO);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
                                GPIO_Pin_9 | GPIO_Pin_10;      

  GPIO_Init(GPIOI, &GPIO_InitStructure); 
}

/**
  * @brief  Initializes the LCD AM_640x480.
  * @param  None
  * @retval None
  */
void STM324xx_LCD_AM640480G5TNQW_Init(void)
{ 
  
  LTDC_InitTypeDef       LTDC_InitStruct;
  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;
  LTDC_Layer_TypeDef     LTDC_Layerx;

/* LCD Size (Width and Height) */  
LCD_PIXEL_WIDTH  = 0x0280; /* 640 */
LCD_PIXEL_HEIGHT = 0x01E0; /* 480 */
  
/* Configure the LCD Control pins --------------------------------------------*/
  LCD_AF_GPIOConfig();  

/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FMCConfig();
  
  /* Configure PLLSAI prescalers for SRAM */
  
  /* Enbale Pixel Clock */
  /* Configure PLLSAI prescalers */
  RCC_PLLSAIConfig(192, 7, 5);
  RCC_LCDCLKDivConfig(RCC_PLLSAIDivR_Div16);
  
  /* Enable PLLSAI Clock */
  RCC_PLLSAICmd(ENABLE);
  /* Wait for PLLSAI activation */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
  {
  }

  GPIO_SetBits(GPIOH, GPIO_Pin_11);
  
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;      /*!< Initialize the LTDC_HSPolarity member */ 
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;      /*!< Initialize the LTDC_VSPolarity member */
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;      /*!< Initialize the LTDC_DEPolarity member */
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;     /*!< Initialize the LTDC_PCPolarity member */
  LTDC_InitStruct.LTDC_HorizontalSync = 0x28;                /*!< Initialize the LTDC_HorizontalSync member */
  LTDC_InitStruct.LTDC_VerticalSync = 0x09;                  /*!< Initialize the LTDC_VerticalSync member */
  LTDC_InitStruct.LTDC_AccumulatedHBP = 0x2A;                /*!< Initialize the LTDC_AccumulatedHBP member */
  LTDC_InitStruct.LTDC_AccumulatedVBP = 0x0B;                /*!< Initialize the LTDC_AccumulatedVBP member */
  LTDC_InitStruct.LTDC_AccumulatedActiveW = 0x20A;           /*!< Initialize the LTDC_AccumulatedActiveW member */
  LTDC_InitStruct.LTDC_AccumulatedActiveH = 0x11B;           /*!< Initialize the LTDC_AccumulatedActiveH member */
  LTDC_InitStruct.LTDC_TotalWidth = 0x20C;                   /*!< Initialize the LTDC_TotalWidth member */
  LTDC_InitStruct.LTDC_TotalHeigh = 0x11D;                   /*!< Initialize the LTDC_TotalHeigh member */
  LTDC_InitStruct.LTDC_BackgroundRedValue = 0x00;            /*!< Initialize the LTDC_BackgroundRedValue member */
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0x00;          /*!< Initialize the LTDC_BackgroundGreenValue member */
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0x00;           /*!< Initialize the LTDC_BackgroundBlueValue member */
  LTDC_Init(&LTDC_InitStruct);
  
 if (Layer == LCD_Layer1)
  {   
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 0x2B;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_PIXEL_WIDTH + 0x2B - 1); 
  LTDC_Layer_InitStruct.LTDC_VarticalStart = 0x0C;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_PIXEL_HEIGHT + 0x0C - 1);           
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 0xFF;           
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0x00;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0x00;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0x00;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0x00;       
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;    
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = 0xC0000000;
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 2) + 3); 
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 2);  
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT; 
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
  }
  else
  { 
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 0x2B;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_PIXEL_WIDTH + 0x2B - 1); 
  LTDC_Layer_InitStruct.LTDC_VarticalStart = 0x0C;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_PIXEL_HEIGHT + 0x0C - 1);           
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 0xFF;           
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0x00;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0x00;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0x00;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0x00;       
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;    
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = 0xC00A0000;
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 2) + 3); 
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 2);  
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT;   
  LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);
  }

  
  LCD_SetFont(&LCD_DEFAULT_FONT);
}

/**
  * @brief  Initializes the LCD AM_480x272.
  * @param  None
  * @retval None
  */
void STM324xx_LCD_AM480272H3TMQW_Init(void)
{ 
  
  LTDC_InitTypeDef       LTDC_InitStruct;
  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;
  LTDC_Layer_TypeDef     LTDC_Layerx;
  
/* LCD Size (Width and Height) */  
 LCD_PIXEL_WIDTH  = 0x01E0; /* 480 */
 LCD_PIXEL_HEIGHT = 0x0110; /* 272 */  
  
/* Configure the LCD Control pins --------------------------------------------*/
  LCD_AF_GPIOConfig();  

/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FMCConfig();
  
  /* Configure PLLSAI prescalers for SRAM */
  
  /* Enbale Pixel Clock */
  /* Configure PLLSAI prescalers */
  RCC_PLLSAIConfig(192, 7, 5);
  RCC_LCDCLKDivConfig(RCC_PLLSAIDivR_Div16);
  
  /* Enable PLLSAI Clock */
  RCC_PLLSAICmd(ENABLE);
  /* Wait for PLLSAI activation */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
  {
  }

  GPIO_SetBits(GPIOH, GPIO_Pin_11);
  
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;      /*!< Initialize the LTDC_HSPolarity member */ 
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;      /*!< Initialize the LTDC_VSPolarity member */
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;      /*!< Initialize the LTDC_DEPolarity member */
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;     /*!< Initialize the LTDC_PCPolarity member */
  LTDC_InitStruct.LTDC_HorizontalSync = 0x28;                /*!< Initialize the LTDC_HorizontalSync member */
  LTDC_InitStruct.LTDC_VerticalSync = 0x09;                  /*!< Initialize the LTDC_VerticalSync member */
  LTDC_InitStruct.LTDC_AccumulatedHBP = 0x2A;                /*!< Initialize the LTDC_AccumulatedHBP member */
  LTDC_InitStruct.LTDC_AccumulatedVBP = 0x0B;                /*!< Initialize the LTDC_AccumulatedVBP member */
  LTDC_InitStruct.LTDC_AccumulatedActiveW = 0x20A;           /*!< Initialize the LTDC_AccumulatedActiveW member */
  LTDC_InitStruct.LTDC_AccumulatedActiveH = 0x11B;           /*!< Initialize the LTDC_AccumulatedActiveH member */
  LTDC_InitStruct.LTDC_TotalWidth = 0x20C;                   /*!< Initialize the LTDC_TotalWidth member */
  LTDC_InitStruct.LTDC_TotalHeigh = 0x11D;                   /*!< Initialize the LTDC_TotalHeigh member */
  LTDC_InitStruct.LTDC_BackgroundRedValue = 0x00;            /*!< Initialize the LTDC_BackgroundRedValue member */
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0x00;          /*!< Initialize the LTDC_BackgroundGreenValue member */
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0x00;           /*!< Initialize the LTDC_BackgroundBlueValue member */
  LTDC_Init(&LTDC_InitStruct);
  
 if (Layer == LCD_Layer1)
  {   
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 0x2B;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_PIXEL_WIDTH + 0x2B - 1); 
  LTDC_Layer_InitStruct.LTDC_VarticalStart = 0x0C;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_PIXEL_HEIGHT + 0x0C - 1);           
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 0xFF;           
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0x00;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0x00;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0x00;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0x00;       
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;    
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = 0xC0000000;
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 2) + 3); 
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 2);  
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT; 
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
  }
  else
  { 
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 0x2B;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_PIXEL_WIDTH + 0x2B - 1); 
  LTDC_Layer_InitStruct.LTDC_VarticalStart = 0x0C;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_PIXEL_HEIGHT + 0x0C - 1);           
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 0xFF;           
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0x00;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0x00;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0x00;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0x00;       
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;    
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = 0xC00A0000;
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 2) + 3); 
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 2);  
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT;   
  LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);
  }
  
  LCD_SetFont(&LCD_DEFAULT_FONT);
}


/**
  * @brief  Sets the LCD Layer.
  * @param  _Layer: specifies the Layer.
  * @retval None
  */
void LCD_SetLayer(__IO uint32_t _Layer)
{
  Layer = _Layer; 
}

/**
  * @brief  Sets the LCD Text and Background colors.
  * @param  _TextColor: specifies the Text Color.
  * @param  _BackColor: specifies the Background Color.
  * @retval None
  */
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor)
{
  TextColor = _TextColor; 
  BackColor = _BackColor;
}

/**
  * @brief  Gets the LCD Text and Background colors.
  * @param  _TextColor: pointer to the variable that will contain the Text 
            Color.
  * @param  _BackColor: pointer to the variable that will contain the Background 
            Color.
  * @retval None
  */
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor)
{
  *_TextColor = TextColor; *_BackColor = BackColor;
}

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetTextColor(__IO uint16_t Color)
{
  TextColor = Color;
}


/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetBackColor(__IO uint16_t Color)
{
  BackColor = Color;
}

/**
  * @brief  Sets the Text Font.
  * @param  fonts: specifies the font to be used.
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  Gets the Text Font.
  * @param  None.
  * @retval the used font.
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}

/**
  * @brief  Clears the selected line.
  * @param  Line: the Line to be cleared.
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..n
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
  uint16_t refcolumn = LCD_PIXEL_WIDTH - 1;
  /* Send the string character by character on lCD */
  while (((refcolumn + 1)& 0xFFFF) >= LCD_Currentfonts->Width)
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, ' ');
    /* Decrement the column position by 16 */
    refcolumn -= LCD_Currentfonts->Width;
  }
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background.
  * @retval None
  */
void LCD_Clear(uint16_t Color)
{
  uint32_t index = 0;
  
    /* erase memory */
  for (index = 0x00; index < 655360; index++)
  {
    *(__IO uint16_t*) (Bank1_SDRAM_ADDRESS + Layer + 2*index) = Color;
  } 
}

/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval None
  */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  
  XADDRESS = 2*(Xpos + LCD_PIXEL_WIDTH*Ypos);

}

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: the Line where to display the character shape.
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, i = 0, xpos =0;
  uint32_t  Xaddress = 0;
  
  xpos = Xpos*LCD_PIXEL_WIDTH*2*12;//LCD_Currentfonts->Height;
  Xaddress += LCD_PIXEL_WIDTH - 1 - Ypos;
  
  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
    
    for(i = 0; i < LCD_Currentfonts->Width; i++)
    {
  
      Bank1_SDRAM_ADDRESS += Layer;
          
      if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> i)) == 0x00) &&(LCD_Currentfonts->Width <= 12))||
        (((c[index] & (0x1 << i)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))

      {
          /* Write data value to all SDRAM memory */

         *(__IO uint16_t*) (Bank1_SDRAM_ADDRESS + 2*Xaddress + xpos) = BackColor;

      }
      else
      {
          /* Write data value to all SDRAM memory */

         *(__IO uint16_t*) (Bank1_SDRAM_ADDRESS + 2*Xaddress + xpos) = TextColor;
         
      }
      Xaddress++;
    }
      Xaddress += LCD_PIXEL_WIDTH - LCD_Currentfonts->Width;
  }
}

/**
  * @brief  Displays one character (16dots width, 24dots height).
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  */
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
  Ascii -= 32;
  LCD_DrawChar(Line, Column, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}

/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{
  
  uint16_t refcolumn = LCD_PIXEL_WIDTH - 1;
  uint32_t i = 0;

  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (((refcolumn + 1) & 0xFFFF) >= LCD_Currentfonts->Width))
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, *ptr);
    /* Decrement the column position by 16 */
    refcolumn -= LCD_Currentfonts->Width;
    /* Point on the next character */
    ptr++;
    i++;
 
  }
}

/**
  * @brief  Sets a display window
  * @param  Xpos: specifies the X bottom left position.
  * @param  Ypos: specifies the Y bottom left position.
  * @param  Height: display window height.
  * @param  Width: display window width.
  * @retval None
  */
void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;
  LTDC_Layer_TypeDef     LTDC_Layerx;
  
 if (Layer == LCD_Layer1)
  {       
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = Xpos + 0x2B;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (Xpos + Width + 0x2B - 1); 
  LTDC_Layer_InitStruct.LTDC_VarticalStart = Ypos + 0x0C;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (Ypos + Height + 0x0C - 1);           
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 0xFF;           
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0x00;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0x00;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0x00;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0x00;       
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;    
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = 0xC0000000;
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((Width * 2) + 3); 
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (Width * 2);  
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = Height; 
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
 }
 else
 {
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = Xpos + 0x2B;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (Xpos + Width + 0x2B - 1); 
  LTDC_Layer_InitStruct.LTDC_VarticalStart = Ypos + 0x0C;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (Ypos + Height + 0x0C - 1);           
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 0xFF;           
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0x00;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0x00;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0x00;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0x00;       
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;    
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = 0xC00A0000;
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((Width * 2) + 3); 
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (Width * 2);  
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = Height; 
  LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);   
  }
}

/**
  * @brief  Disables LCD Window mode.
  * @param  None
  * @retval None
  */
void LCD_WindowModeDisable(void)
{
  LCD_SetDisplayWindow(0, 0, LCD_PIXEL_HEIGHT, LCD_PIXEL_WIDTH);
  
}

/**
  * @brief  Displays a line.
  * @param Xpos: specifies the X position.
  * @param Ypos: specifies the Y position.
  * @param Length: line length.
  * @param Direction: line direction.
  *   This parameter can be one of the following values: Vertical or Horizontal.
  * @retval None
  */
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
    DMA2D_InitTypeDef      DMA2D_InitStruct;
  
    uint32_t  Xaddress = 0;
    uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;
  
  Xaddress = 0xC0000000 + Layer + 2*(LCD_PIXEL_WIDTH*Ypos + Xpos);
 
  Red_Value = (0xF800 & TextColor) >> 11;
  Blue_Value = 0x001F & TextColor;
  Green_Value = (0x07E0 & TextColor) >> 5;

  if(Direction == LCD_DIR_HORIZONTAL)
  {    
  DMA2D_DeInit();  
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;      
  DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;      
  DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;     
  DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;                
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;                  
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;                
  DMA2D_InitStruct.DMA2D_OutputOffset = 0;                
  DMA2D_InitStruct.DMA2D_NumberOfLine = 1;            
  DMA2D_InitStruct.DMA2D_PixelPerLine = Length;
  DMA2D_Init(&DMA2D_InitStruct); 
  
    /* Start Transfer */ 
  DMA2D_StartTransfer();
  
  /* Wait for CTC Flag activation */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  } 
  
  }
  else
  {
  DMA2D_DeInit();
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;      
  DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;      
  DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;     
  DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;                
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;                  
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;                
  DMA2D_InitStruct.DMA2D_OutputOffset = LCD_PIXEL_WIDTH - 1;                
  DMA2D_InitStruct.DMA2D_NumberOfLine = Length;            
  DMA2D_InitStruct.DMA2D_PixelPerLine = 1;
  DMA2D_Init(&DMA2D_InitStruct); 
  
    /* Start Transfer */ 
  DMA2D_StartTransfer();
  
  /* Wait for CTC Flag activation */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  } 
  
  }  

}

/**
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: display rectangle height.
  * @param  Width: display rectangle width.
  * @retval None
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
  LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
  LCD_DrawLine(Xpos, (Ypos+ Height), Width, LCD_DIR_HORIZONTAL);
  
  LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
  LCD_DrawLine((Xpos + Width), Ypos, Height, LCD_DIR_VERTICAL);
}


void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int x = -Radius, y = 0, err = 2-2*Radius, e2;
    uint32_t Xaddress = Bank1_SDRAM_ADDRESS + Layer;
    do {
        *(__IO uint16_t*) (Xaddress + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = TextColor; 
        *(__IO uint16_t*) (Xaddress + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = TextColor;
        *(__IO uint16_t*) (Xaddress + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = TextColor;
        *(__IO uint16_t*) (Xaddress + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = TextColor;
      
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);
}

void LCD_DrawFullEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
    int x = -Radius, y = 0, err = 2-2*Radius, e2;
    uint32_t Xaddress = Bank1_SDRAM_ADDRESS + Layer;
    float K = 0, rad1 = 0, rad2 = 0;
    
    
    rad1 = Radius;
    rad2 = Radius2;

      if (Radius > Radius2)
      { 
        do {
        K = (float)(rad1/rad2);
        LCD_DrawLine((Xpos+x), (Ypos-(uint16_t)(y/K)), (2*(uint16_t)(y/K) + 1), LCD_DIR_VERTICAL);
        LCD_DrawLine((Xpos-x), (Ypos-(uint16_t)(y/K)), (2*(uint16_t)(y/K) + 1), LCD_DIR_VERTICAL);

        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
        
    }
    while (x <= 0);
      }
      else
      {
        y = -Radius2; 
        x = 0;
       do { 
        K = (float)(rad2/rad1);
        
        LCD_DrawLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1), LCD_DIR_HORIZONTAL);
        LCD_DrawLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1), LCD_DIR_HORIZONTAL);

        e2 = err;
        if (e2 <= x) {
            err += ++x*2+1;
            if (-y == x && e2 <= y) e2 = 0;
        }
        if (e2 > y) err += ++y*2+1;
    }
    while (y <= 0);
    }
}

/**
  * @brief  Displays an Ellipse.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius: specifies Radius.
  * @param  Radius2: specifies Radius2.
  * @retval None
  */

void LCD_DrawEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
    int x = -Radius, y = 0, err = 2-2*Radius, e2;
    uint32_t Xaddress = Bank1_SDRAM_ADDRESS + Layer;
    float K = 0, rad1 = 0, rad2 = 0;
    
    
    rad1 = Radius;
    rad2 = Radius2;

      if (Radius > Radius2)
      { 
        do {
        K = (float)(rad1/rad2);
        *(__IO uint16_t*) (Xaddress + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K))))) = TextColor; 
        *(__IO uint16_t*) (Xaddress + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K))))) = TextColor;
        *(__IO uint16_t*) (Xaddress + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K))))) = TextColor;
        *(__IO uint16_t*) (Xaddress + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K))))) = TextColor;


        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);
      }
      else
      {
        y = -Radius2; 
        x = 0;
       do { 
        K = (float)(rad2/rad1);
        *(__IO uint16_t*) (Xaddress + (2*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y)))) = TextColor; 
        *(__IO uint16_t*) (Xaddress + (2*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y)))) = TextColor;
        *(__IO uint16_t*) (Xaddress + (2*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y)))) = TextColor;
        *(__IO uint16_t*) (Xaddress + (2*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y)))) = TextColor;

        e2 = err;
        if (e2 <= x) {
            err += ++x*2+1;
            if (-y == x && e2 <= y) e2 = 0;
        }
        if (e2 > y) err += ++y*2+1;
        
    }
    while (y <= 0);
    }
}


/**
  * @brief  Displays a mono-color picture.
  * @param  Pict: pointer to the picture array.
  * @retval None
  */
void LCD_DrawMonoPict(const uint32_t *Pict)
{
  uint32_t index = 0, i = 0;
  uint32_t  Xaddress = 0;
 
  Xaddress = Bank1_SDRAM_ADDRESS + Layer;
  
  
  for(index = 0; index < 2400; index++)
  {
    for(i = 0; i < 32; i++)
    {
      if((Pict[index] & (1 << i)) == 0x00)
      {
        *(__IO uint16_t*) (Xaddress) = BackColor;
      }
      else
      {
        *(__IO uint16_t*) (Xaddress) = TextColor;
      }
    }
  }
}

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
void LCD_WriteBMP(uint32_t BmpAddress)
{
  uint32_t index = 0, size = 0, width = 0, height = 0;
  uint32_t  Xaddress = 0;
  
  Xaddress = Bank1_SDRAM_ADDRESS + Layer;
    
  /* Read bitmap size */
  size = *(__IO uint16_t *) (BmpAddress + 2);
  size |= (*(__IO uint16_t *) (BmpAddress + 4)) << 16;
  /* Get bitmap data address offset */
  index = *(__IO uint16_t *) (BmpAddress + 10);
  index |= (*(__IO uint16_t *) (BmpAddress + 12)) << 16;
  size = (size - index)/2;
  
  /* Read bitmap width */
  width = *(uint16_t *) (BmpAddress + 18);
  width |= (*(uint16_t *) (BmpAddress + 20)) << 16;

  /* Read bitmap height */
  height = *(uint16_t *) (BmpAddress + 22);
  height |= (*(uint16_t *) (BmpAddress + 24)) << 16;
  
  BmpAddress += index;
  
  if (Layer == 0)
  {
    LTDC_LayerSize(LTDC_Layer1, width, height);
  }
  else
  {
    LTDC_LayerSize(LTDC_Layer2, width, height);
  }  
  LTDC_ReloadConfig(LTDC_IMReload);
  
  for(index = 0; index < size; index++)
  {
    *(__IO uint16_t*) (Xaddress) = *(__IO uint16_t *)BmpAddress;
    BmpAddress += 2;
    Xaddress += 2;
  }
}

/**
  * @brief  Displays a full rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @retval None
  */
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  DMA2D_InitTypeDef      DMA2D_InitStruct;
  
  uint32_t  Xaddress = 0;
  
  uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;
 
  Red_Value = (0xF800 & TextColor) >> 11;
  Blue_Value = 0x001F & TextColor;
  Green_Value = (0x07E0 & TextColor) >> 5;
  
  Xaddress = 0xC0000000 + Layer + 2*(LCD_PIXEL_WIDTH*Ypos + Xpos);
  
  DMA2D_DeInit();
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;      
  DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;      
  DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;     
  DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;                
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;                  
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;                
  DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_PIXEL_WIDTH - Width);                
  DMA2D_InitStruct.DMA2D_NumberOfLine = Height;            
  DMA2D_InitStruct.DMA2D_PixelPerLine = Width;
  DMA2D_Init(&DMA2D_InitStruct); 
  
  /* Start Transfer */ 
  DMA2D_StartTransfer();
  
  /* Wait for CTC Flag activation */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  } 

  LCD_SetTextColor(TextColor);
}

/**
  * @brief  Displays a full circle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius
  * @retval None
  */
void LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;    /* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  
  D = 3 - (Radius << 1);

  CurX = 0;
  CurY = Radius;

  while (CurX <= CurY)
  {
    if(CurY > 0) 
    {
      LCD_DrawLine(Xpos - CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
      LCD_DrawLine(Xpos + CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
    }

    if(CurX > 0) 
    {
      LCD_DrawLine(Xpos - CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
      LCD_DrawLine(Xpos + CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
    }
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }

  LCD_DrawCircle(Xpos, Ypos, Radius);
  
}

/**
  * @brief  Displays an uni-line (between two points).
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
void LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    PutPixel(x, y);             /* Draw the current pixel */
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}

/**
  * @brief  Displays an triangle (between 3 points).
  * @param  Points: pointer to the points array.
  * @retval None
  */
void LCD_Triangle(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0;
  pPoint First = Points;

  if(PointCount != 3)
  {
    return;
  }

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    LCD_DrawUniLine(X, Y, Points->X, Points->Y);
  }
  LCD_DrawUniLine(First->X, First->Y, Points->X, Points->Y);
}

/**
  * @brief  Fill an triangle (between 3 points).
  * @param  Points: pointer to the points array.
  * @retval None
  */
void LCD_FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{ 
  
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    LCD_DrawUniLine(x, y, x3, y3);
    
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }  
  
  
}
/**
  * @brief  Displays an poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLine(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0;

  if(PointCount < 2)
  {
    return;
  }

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    LCD_DrawUniLine(X, Y, Points->X, Points->Y);
  }
}

/**
  * @brief  Displays an relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @param  Closed: specifies if the draw is closed or not.
  *           1: closed, 0 : not closed.
  * @retval None
  */
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed)
{
  int16_t X = 0, Y = 0;
  pPoint First = Points;

  if(PointCount < 2)
  {
    return;
  }  
  X = Points->X;
  Y = Points->Y;
  while(--PointCount)
  {
    Points++;
    LCD_DrawUniLine(X, Y, X + Points->X, Y + Points->Y);
    X = X + Points->X;
    Y = Y + Points->Y;
  }
  if(Closed)
  {
    LCD_DrawUniLine(First->X, First->Y, X, Y);
  }  
}

/**
  * @brief  Displays a closed poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLine(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLine(Points, PointCount);
  LCD_DrawUniLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
}

/**
  * @brief  Displays a relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLineRelative(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLineRelativeClosed(Points, PointCount, 0);
}

/**
  * @brief  Displays a closed relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLineRelativeClosed(Points, PointCount, 1);
}


/**
  * @brief  Displays a  full poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_FillPolyLine(pPoint Points, uint16_t PointCount)
{
 
  int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, i = 0;
  uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;  

  IMAGE_LEFT = IMAGE_RIGHT = Points->X;
  IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

  for(i = 1; i < PointCount; i++)
  {
    pixelX = POLY_X(i);
    if(pixelX < IMAGE_LEFT)
    {
      IMAGE_LEFT = pixelX;
    }
    if(pixelX > IMAGE_RIGHT)
    {
      IMAGE_RIGHT = pixelX;
    }
    
    pixelY = POLY_Y(i);
    if(pixelY < IMAGE_TOP)
    { 
      IMAGE_TOP = pixelY;
    }
    if(pixelY > IMAGE_BOTTOM)
    {
      IMAGE_BOTTOM = pixelY;
    }
  }  
  
  if(PointCount < 2)
  {
    return;
  }
  
  X_center = (IMAGE_LEFT + IMAGE_RIGHT)/2;
  Y_center = (IMAGE_BOTTOM + IMAGE_TOP)/2;
 
  X_first = Points->X;
  Y_first = Points->Y;
  
  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    X2 = Points->X;
    Y2 = Points->Y;    
  
    LCD_FillTriangle(X, X2, X_center, Y, Y2, Y_center);
    LCD_FillTriangle(X, X_center, X2, Y, Y_center, Y2);
    LCD_FillTriangle(X_center, X2, X, Y_center, Y2, Y);
    
  }

    LCD_FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
    LCD_FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
    LCD_FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);
  
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOn(void)
{
  /* Display On */
  LTDC_Cmd(ENABLE); /* display ON */
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOff(void)
{
  /* Display Off */
  LTDC_Cmd(DISABLE); 
}

/**
  * @brief  Configures the Parallel interface (FMC) for LCD(Parallel mode)
  * @param  None
  * @retval None
  */
void LCD_FMCConfig(void)
{
  FMC_SDRAMTimingInitTypeDef  FMC_SDRAMTimingInitStructure;
  FMC_SDRAMInitTypeDef  FMC_SDRAMInitStructure;
  
  FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay = 2;
  FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 1;
  FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime = 1;
  FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay = 1;
  FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime = 2;
  FMC_SDRAMTimingInitStructure.FMC_RPDelay = 1;
  FMC_SDRAMTimingInitStructure.FMC_RCDDelay = 1;
  
  FMC_SDRAMInitStructure.FMC_Bank = FMC_Bank1_SDRAM;
  FMC_SDRAMInitStructure.FMC_ColumnBitsNumber = FMC_ColumnBits_Number_9b;
  FMC_SDRAMInitStructure.FMC_RowBitsNumber = FMC_RowBits_Number_13b;
  FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth = FMC_SDMemory_Width_16b;
  FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4;
  FMC_SDRAMInitStructure.FMC_CASLatency = FMC_CAS_Latency_2;  
  FMC_SDRAMInitStructure.FMC_WriteProtection = FMC_Write_Protection_disable;
  FMC_SDRAMInitStructure.FMC_SDClockPeriod = FMC_SDClock_Period_2; 
  FMC_SDRAMInitStructure.FMC_ReadBurst = FMC_Read_Burst_enable;
  FMC_SDRAMInitStructure.FMC_ReadPipeDelay = FMC_ReadPipe_Delay_1;
  FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct = &FMC_SDRAMTimingInitStructure;
  
  /* FMC SDRAM GPIOs Configuration*/
  LCD_CtrlLinesConfig();
  
  /* FMC registers configuration */
  FMC_SDRAMInit(&FMC_SDRAMInitStructure);    
    
  /* FMC SDRAM initialization */
  FMC_SDRAM_InitSequence();
    
  /* Disable write protection */
  FMC_SDRAMWriteProtectionConfig(FMC_Bank1_SDRAM,DISABLE);
}

/**
  * @brief  Displays a pixel.
  * @param  x: pixel x.
  * @param  y: pixel y.  
  * @retval None
  */
static void PutPixel(int16_t x, int16_t y)
{ 
  if(x < 0 || x > (LCD_PIXEL_WIDTH - 1) || y < 0 || y > (LCD_PIXEL_HEIGHT - 1))
  {
    return;  
  }
  LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
}


#ifndef USE_Delay
/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}
#endif /* USE_Delay*/

/**
  * @brief  Configures all SDRAM memory I/Os pins. 
  * @param  None. 
  * @retval None.
  */
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIOs clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF |
                         RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI, ENABLE);
  
  /* Enable FMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);
   
/*-- GPIOs Configuration -----------------------------------------------------*/
/*
 +-------------------+--------------------+--------------------+--------------------+
 +                       SDRAM pins assignment                                      +
 +-------------------+--------------------+--------------------+--------------------+
 | PD0  <-> FMC_D2   | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0    | PG0 <-> FMC_A10    |
 | PD1  <-> FMC_D3   | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1    | PG1 <-> FMC_A11    |
 | PD4  <-> FMC_NOE  | PE7  <-> FMC_D4    | PF2  <-> FMC_A2    | PG2 <-> FMC_A12    |
 | PD5  <-> FMC_NWE  | PE8  <-> FMC_D5    | PF3  <-> FMC_A3    | PG3 <-> FMC_A13    |
 | PD8  <-> FMC_D13  | PE9  <-> FMC_D6    | PF4  <-> FMC_A4    | PG4 <-> FMC_A14    |
 | PD9  <-> FMC_D14  | PE10 <-> FMC_D7    | PF5  <-> FMC_A5    | PG5 <-> FMC_A15    |
 | PD10 <-> FMC_D15  | PE11 <-> FMC_D8    | PF11 <-> FC_NRAS   | PG8 <-> FC_SDCLK   |
 | PD14 <-> FMC_D0   | PE12 <-> FMC_D9    | PF12 <-> FMC_A6    | PG9 <-> FMC_NE2    |
 | PD15 <-> FMC_D1   | PE13 <-> FMC_D10   | PF13 <-> FMC_A7    | PG15 <-> FMC_NCAS  |
 +-------------------+ PE14 <-> FMC_D11   | PF14 <-> FMC_A8    |--------------------+ 
                     | PE15 <-> FMC_D12   | PF15 <-> FMC_A9    |
 +-------------------+--------------------+--------------------+
 | PH2 <-> FMC_SDCKE0 | PI4 <-> FMC_NBL2  |
 | PH3 <-> FMC_SDNE0  | PI5 <-> FMC_NBL3  |
 | PH5 <-> FMC_SDNWE  |-------------------+
 | PH6 <-> FMC_SDNE1  |
 | PH7 <-> FMC_SDCKE1 |
 +--------------------+
 +-------------------+------------------+
 +   32-bits Mode: D31-D16              +
 +-------------------+------------------+
 | PH8 <-> FMC_D16   | PI0 <-> FMC_D24  |
 | PH9 <-> FMC_D17   | PI1 <-> FMC_D25  |
 | PH10 <-> FMC_D18  | PI2 <-> FMC_D26  |
 | PH11 <-> FMC_D19  | PI3 <-> FMC_D27  |
 | PH12 <-> FMC_D20  | PI6 <-> FMC_D28  |
 | PH13 <-> FMC_D21  | PI7 <-> FMC_D29  |
 | PH14 <-> FMC_D22  | PI9 <-> FMC_D30  |
 | PH15 <-> FMC_D23  | PI10 <-> FMC_D31 |
 +------------------+-------------------+
*/

  /* GPIOD configuration */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FMC); 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_3  | GPIO_Pin_4  |
                                GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7  | GPIO_Pin_8  |
                                GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12  | 
                                GPIO_Pin_13 |GPIO_Pin_14  | GPIO_Pin_15;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);


  /* GPIOE configuration */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource2 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource3 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 |  
                                GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7 |
                                GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11|
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

  GPIO_Init(GPIOE, &GPIO_InitStructure);


  /* GPIOF configuration */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource0 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource1 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource2 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource3 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource4 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource5 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource6 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource7 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource8 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource9 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource10 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource11 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource12 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource13 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource14 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource15 , GPIO_AF_FMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3  | 
                                GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7  | 
                                GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_11 | GPIO_Pin_10 |
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;      

  GPIO_Init(GPIOF, &GPIO_InitStructure);


  /* GPIOG configuration */
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource0 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource1 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource2 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource3 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource4 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource5 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource6 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource7 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource8 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource9 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource10 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource13 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource14 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource15 , GPIO_AF_FMC);
  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 | 
                                GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7 |
                                GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11|
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;        

  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  
  /* GPIOH configuration */
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource2 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource3 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource5 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource6 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource7 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource8 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource9 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource10 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource11 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource12 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource13 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource14 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource15 , GPIO_AF_FMC);
  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  | GPIO_Pin_3  | GPIO_Pin_5  | GPIO_Pin_6  |
                                GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |  
                                GPIO_Pin_15;      

  GPIO_Init(GPIOH, &GPIO_InitStructure);

  /* GPIOI configuration */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource0 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource1 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource2 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource3 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource4 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource5 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource6 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource7 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource9 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource10 , GPIO_AF_FMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
                                GPIO_Pin_9 | GPIO_Pin_10;      

  GPIO_Init(GPIOI, &GPIO_InitStructure);  
}


/**
  * @brief GPIO config for LTDC.
  * @retval
  *  0: Test passed
  *  Value different from 0: Test failed
  *  None
  */

  static void LCD_AF_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*!< Enable GPIOI, GPIOJ, GPIOK AHB Clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOJ | \
                         RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOF | \
                         RCC_AHB1Periph_GPIOH,  ENABLE);

  /*!< Connect PI.00 to LCDTFT G5 */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_LTDC);

  /*!< Connect PI.01 to LCDTFT G6 */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_LTDC);

  /*!< Connect PI.02 to LCDTFT G7 */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_LTDC);

  /*!< Connect PI.04 to LCDTFT B4 */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource4, GPIO_AF_LTDC);

  /*!< Connect PI.05 to LCDTFT B5 */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_LTDC);

  /*!< Connect PI.06 to LCDTFT B6 */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_LTDC);

  /*!< Connect PI.07 to LCDTFT B7 */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_LTDC);

  /*!< Connect PI.12 to LCDTFT HSYNC */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource12, GPIO_AF_LTDC);

  /*!< Connect PI.13 to LCDTFT VSYNC */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource13, GPIO_AF_LTDC);

  /*!< Connect PI.15 to LCDTFT R0 */
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource15, GPIO_AF_LTDC);

  /*!< Connect PJ.00 to LCDTFT R1 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource0, GPIO_AF_LTDC);

  /*!< Connect PJ.01 to LCDTFT R2 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource1, GPIO_AF_LTDC);

  /*!< Connect PJ.02 to LCDTFT R3 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource2, GPIO_AF_LTDC);

  /*!< Connect PJ.03 to LCDTFT R4 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource3, GPIO_AF_LTDC);

  /*!< Connect PJ.04 to LCDTFT R5 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource4, GPIO_AF_LTDC);

  /*!< Connect PJ.05 to LCDTFT R6 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource5, GPIO_AF_LTDC);

  /*!< Connect PJ.06 to LCDTFT R7 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource6, GPIO_AF_LTDC);

  /*!< Connect PJ.07 to LCDTFT G0 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource7, GPIO_AF_LTDC);

  /*!< Connect PJ.08 to LCDTFT G1 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource8, GPIO_AF_LTDC);

  /*!< Connect PJ.09 to LCDTFT G2 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource9, GPIO_AF_LTDC);

  /*!< Connect PJ.10 to LCDTFT G3 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource10, GPIO_AF_LTDC);

  /*!< Connect PJ.11 to LCDTFT G4 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource11, GPIO_AF_LTDC);

  /*!< Connect PJ.12 to LCDTFT B0 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource12, GPIO_AF_LTDC);

  /*!< Connect PJ.13 to LCDTFT B1 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource13, GPIO_AF_LTDC);

  /*!< Connect PJ.14 to LCDTFT B2 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource14, GPIO_AF_LTDC);

  /*!< Connect PJ.15 to LCDTFT B3 */
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource15, GPIO_AF_LTDC);

  /*!< Connect PF.10 to LCDTFT DE */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);

  /*!< Connect PG.07 to LCDTFT CLK */
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | \
                             GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_12 | \
                             GPIO_Pin_13 | GPIO_Pin_15;
                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOI, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                             GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | \
                             GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
                             GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOJ, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOF, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOG, &GPIO_InitStruct);
  
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOH, &GPIO_InitStruct);
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */  

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
