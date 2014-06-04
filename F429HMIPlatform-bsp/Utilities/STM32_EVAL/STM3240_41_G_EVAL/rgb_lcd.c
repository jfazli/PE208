/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : rgb_lcd.c
* Author             : MCD Tools development Team
* Date First Issued  : September 2012
* Description        : This file includes the LCD driver for AM-480272H3TMQW-T01H
*                      or AM-640480G5TNQW-T00H
*                      LCD Module on MB1046/MB1063
* Note: RGB LCD frame buffer needs to be located in external SRAM/SDRAM
********************************************************************************
* History:
* November 22, 2012: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "rgb_lcd.h"
#include "fmc_sram.h"
#include "fmc_sdram.h"


#ifdef USE_LCD_480272
LCD_480272_TypeDef *LCD_FrameBuf;
#else
LCD_640480_TypeDef *LCD_FrameBuf;
#endif


//this array in SRAM
LCD_FrameBUF2_Typedef *LCD_Ptr_FrameL2 ;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
  
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void delay_ms(__IO uint32_t nCount)
{
	__IO uint32_t index = 0;
	for(index = (100000 * nCount); index != 0; index--);
}

/*******************************************************************************
* Function Name  : LCD_RgbInit
* Description    : Init RGB LCD data/control lines in Alt Push-Pull mode
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_RgbInit(u8 buf)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  LTDC_InitTypeDef LTDC_InitStructure;

  // Enable LCD, GPIOI~k clock //
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOI |
                         RCC_AHB1Periph_GPIOJ | RCC_AHB1Periph_GPIOK, ENABLE);

  // Set PC6 as PP OUT - LCD_On/Off
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
  // Set PI14 as PP AF - CLK, MAX 15M for AM-480272H3TMQW,Typ 25M for AM-640480G5TNQW
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//#ifdef USE_LCD_640480
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//#endif
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOI, &GPIO_InitStructure);
  // Set PI12,13,15 as PP AF - HSYNC,VSYNC, R0
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
 // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOI, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOI, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource12, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource13, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource14, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource15, GPIO_AF_LTDC);
  // Set PJ0~15 as PP AF - R1~R7, G0~G4, B0~B3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource2, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource5, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource8, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource9, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource12, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource13, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource14, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOJ, GPIO_PinSource15, GPIO_AF_LTDC);
  // Set PK0~6, 7 as PP AF - G5~G7, B4~B7, DE
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                                GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOK, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource2, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource3, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource4, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource5, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOK, GPIO_PinSource7, GPIO_AF_LTDC);

  extern void delay_ms(u32 n_ms);
#ifdef USE_LCD_480272

#define HBP 22 //Horizontal Back Porch 2+20
#define VBP 3 //Vertical Back Porch 3

  // LTDC also uses PLLSAI, 9MHz Typical, 15M MAX
  RCC_PLLSAIConfig(240, 2, 3);// 120M SAI CLK_in, 80M LCD CLK_in
//  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div16);// div 2~16, to get 10M LCD CLK
  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div16);// div 2~16, to get 10M LCD CLK
  RCC_PLLSAICmd(ENABLE);
	
  // Wait till PLLSAI is ready - PLL_RDY @ bit 29
  while((RCC->CR & (1<<29)) == 0)
    ;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);
  LTDC_DeInit();
  delay_ms(1); // this delay seems optional for 480x272
  // invalid LOW for HSync,VSync, DE of AM-480272H3TMQW, latch @falling CLK
  LTDC_InitStructure.LTDC_HSPolarity = LTDC_HSPolarity_AL;
  LTDC_InitStructure.LTDC_VSPolarity = LTDC_VSPolarity_AL;
  LTDC_InitStructure.LTDC_DEPolarity = LTDC_DEPolarity_AL;
//  LTDC_InitStructure.LTDC_PCPolarity = LTDC_PCPolarity_IIPC;// display seems also OK,why?
  LTDC_InitStructure.LTDC_PCPolarity = LTDC_PCPolarity_IPC;// OK(expected with scope)
  LTDC_InitStructure.LTDC_HorizontalSync = 41-1;//Horizontal pulse width
  LTDC_InitStructure.LTDC_VerticalSync = 10-1;//Vertical pulse width
  LTDC_InitStructure.LTDC_AccumulatedHBP = 41-1 +HBP;//HBP=2+20
  LTDC_InitStructure.LTDC_AccumulatedVBP = 10-1 +VBP;//VBP=2
  LTDC_InitStructure.LTDC_AccumulatedActiveW = 41-1+HBP +480;//ActiveW=480
  LTDC_InitStructure.LTDC_AccumulatedActiveH = 10-1+VBP +272;//ActiveH=272
  LTDC_InitStructure.LTDC_TotalWidth = 41-1+HBP+480 +2+30;//HFP=2+30, Horizontal cycle is 575
  LTDC_InitStructure.LTDC_TotalHeigh = 10-1+VBP+272 +2;//VFP=2,Vertical cycle is 286
  LTDC_InitStructure.LTDC_BackgroundRedValue = 0x00;
  LTDC_InitStructure.LTDC_BackgroundGreenValue = 0x00;
  LTDC_InitStructure.LTDC_BackgroundBlueValue = 0x00;
  LTDC_Init(&LTDC_InitStructure);
  LTDC_Layer1->CFBLR = ((480*2) << 16) | (480*2 + 3);//width
//  LTDC_Layer1->CFBLR = ((480*2) << 16) | (0);//width
  LTDC_Layer1->CFBLNR = 272;//height
  // full-size window for Layer1, Layer2 not used
  LTDC_Layer1->WHPCR = ((63+479) << 16) | (63);//AccumulatedHBP+1, Hstart: 0,Hstop: 479
  LTDC_Layer1->WVPCR = ((13+271) << 16) | (13);//AccumulatedVBP+1, Vstart: 0,Vstop: 271


  if(buf == SRAM_BUF)
    LCD_FrameBuf = ((LCD_480272_TypeDef *) EVAL_SRAM_BASE);
  else
    LCD_FrameBuf = ((LCD_480272_TypeDef *) EVAL_SDRAM_BASE);

//  LCD_FrameBuf = (LCD_480272_TypeDef *)EVAL_SRAM_BASE_LCD;
  
//phv 5/2 ????
//	LTDC_Layer2->CFBLR = ((20*2) << 16) | (20*2 + 3);//width
//  LTDC_Layer2->CFBLNR = 20;//height
  // part-size window for Layer2
//  LTDC_Layer2->WHPCR = ((63+19) << 16) | (63+0);//AccumulatedHBP+1, Hstart: 0,Hstop: 19
//  LTDC_Layer2->WVPCR = ((13+19) << 16) | (13+0);//AccumulatedVBP+1, Vstart: 0,Vstop: 19
	
#else
#ifdef USE_LCD_640480
  
	// LTDC also uses PLLSAI, 25MHz Typical
  RCC_PLLSAIConfig(200, 10, 4);// 20M SAI CLK, 50M LCD CLK
  RCC_LCDCLKDivConfig(RCC_PLLSAIDivR_Div2);// div 2~16, to get 25M LCD CLK
  RCC_PLLSAICmd(ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);
	
  LTDC_DeInit();
  delay_ms(1); // this delay seems mandatory for 640x480
  // invalid LOW for HSync,VSync,DE of for AM-640480G5TNQW, latch @rising CLK
  LTDC_InitStructure.LTDC_HSPolarity = LTDC_HSPolarity_AL;
  LTDC_InitStructure.LTDC_VSPolarity = LTDC_VSPolarity_AL;
  LTDC_InitStructure.LTDC_DEPolarity = LTDC_DEPolarity_AL;
//  LTDC_InitStructure.LTDC_PCPolarity = LTDC_PCPolarity_IPC;// display seems also OK,why?
  LTDC_InitStructure.LTDC_PCPolarity = LTDC_PCPolarity_IIPC;// OK(expected with scope)
  LTDC_InitStructure.LTDC_HorizontalSync = 30-1;//HSync pulse width
  LTDC_InitStructure.LTDC_VerticalSync = 3-1;//VSync pulse width
  LTDC_InitStructure.LTDC_AccumulatedHBP = 30-1 +120;//HBP=120?
  LTDC_InitStructure.LTDC_AccumulatedVBP = 3-1 +35;//VBP=35?
  LTDC_InitStructure.LTDC_AccumulatedActiveW = 30-1+120 +640;//ActiveW=640
  LTDC_InitStructure.LTDC_AccumulatedActiveH = 3-1+35 +480;//ActiveH=480
  LTDC_InitStructure.LTDC_TotalWidth = 30-1+120+640 +10;//HFP=10?, Horizontal cycle is 800
  LTDC_InitStructure.LTDC_TotalHeigh = 3-1+35+480 +7;//VFP=7?,Vertical cycle is 525
  LTDC_InitStructure.LTDC_BackgroundRedValue = 0x00;
  LTDC_InitStructure.LTDC_BackgroundGreenValue = 0x00;
  LTDC_InitStructure.LTDC_BackgroundBlueValue = 0x00;
  LTDC_Init(&LTDC_InitStructure);
  LTDC_Layer1->CFBLR = ((640*2) << 16) | (640*2 + 3);//width
  LTDC_Layer1->CFBLNR = 480;//height
  // full-size window for Layer1, Layer2 not used
  LTDC_Layer1->WHPCR = ((150+639) << 16) | (150);//AccumulatedHBP+1, Hstart: 0,Hstop: 639
  LTDC_Layer1->WVPCR = ((38+479) << 16) | (38);//LTDC_AccumulatedVBP+1, Vstart: 0,Vstop: 479

  if(buf == SRAM_BUF)
    LCD_FrameBuf = ((LCD_640480_TypeDef *) EVAL_SRAM_BASE);
  else
    LCD_FrameBuf = ((LCD_640480_TypeDef *) EVAL_SDRAM_BASE);
#else
#error "either USE_LCD_480272 or USE_LCD_640480 should be defined"
#endif
#endif
  LTDC_Layer1->CFBAR = (u32)LCD_FrameBuf;
  // Pixelformat: RGB565
  LTDC_Layer1->PFCR = LTDC_Pixelformat_RGB565;
  // constant alpha is 1.00(0xFF) for layer1
  LTDC_Layer1->CACR = 0xFF;
  // default color is white
  LTDC_Layer1->DCCR = 0xffFFFF;
  // blending factors is use 100% from layer1 & 0% from BackGround for layer1
  LTDC_Layer1->BFCR = 0x405;
  // enable Layer1
  LTDC_Layer1->CR = LTDC_LxCR_LEN;//color look-up table & color keying disabled

	LCD_Ptr_FrameL2 = ((LCD_FrameBUF2_Typedef *)EVAL_SRAM_BASE);
	
  LTDC_Layer2->CFBAR = (u32)LCD_Ptr_FrameL2;
	
  // Pixelformat: ARGB1555
  LTDC_Layer2->PFCR = LTDC_Pixelformat_ARGB1555;
  // constant alpha is 1.00(0xFF) for layer2
  LTDC_Layer2->CACR = 0xFF;
  // default color is black (not shown)
  LTDC_Layer2->DCCR = 0x00;
  //bit alpha=constant alpha * bit
  // blending factors is use BitAlpha from layer2 & 1-BitAlpha from BackGround&layer1
  LTDC_Layer2->BFCR = 0x607;
  // enable Layer2
  LTDC_Layer2->CR = LTDC_LxCR_LEN;

  // reload register Immediately
  LTDC->SRCR = LTDC_SRCR_IMR;
  LTDC_Cmd(ENABLE);
  GPIO_SetBits(GPIOC, GPIO_Pin_6);
}

/*******************************************************************************
* Function Name  : LCD_RgbUpdate
* Description    : Update RGB LCD screen with data from frame buffer
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_RgbUpdate(void)
{
//  DMA_InitTypeDef DMA_InitStruct;

  // enable DMA2 clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
}

/*******************************************************************************
* Function Name  : LCD_RgbClear
* Description    : Clear RGB LCD screen with Color
* Input          : - Color: the Color to be used to clear screen.
*                    This parameter can be various values:
*                       White/Black/Grey/Red/Green/Blue ...
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_RgbClear(u16 Color)
{
  u32 u;
   for(u=0; u<(sizeof(*LCD_FrameBuf)/sizeof(vu16)); u++)
    LCD_FrameBuf->HWord[u] = Color;
}

/*******************************************************************************
* Function Name  : LCD_RgbL2SetDisplayWindow
* Description    : Sets a display window in Layer2
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
*                  - Height: display window height.
*                  - Width: display window width.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_RgbL2SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Width, u16 Height)
{
  LTDC_Layer2->CFBLR = ((Width*2) << 16) | (Width*2 + 3);//width
  LTDC_Layer2->CFBLNR = Height;//height
  // part-size window for Layer2
  LTDC_Layer2->WHPCR = ((63+Xpos+Width-1) << 16) | (63+Xpos);//AccumulatedHBP+1, Hstart: Xpos,Hstop: Xpos+Width-1
  LTDC_Layer2->WVPCR = ((13+Ypos+Height-1) << 16) | (13+Ypos);//AccumulatedVBP+1, Vstart: Ypos,Vstop: Ypos+Height-1
  // reload register Immediately
  LTDC->SRCR = LTDC_SRCR_IMR;
}
