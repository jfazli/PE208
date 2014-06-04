/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 * 10/1/2012 update Controler init
 */

#include "LLDISPLAY_STM32x0GEVAL.h"
#define LLDISPLAY_COPY_IMPL LLDISPLAY_STM32x0GEVAL
#include "LLDISPLAY_COPY_impl.h"
#include "cpu.h"
#if MCU == 2
#include "stm32f2xx.h"
#include "stm322xg_eval_lcd.h"
#include "stm322xg_eval_fsmc_sram.h"
#elif MCU == 4
#include "stm32f4xx.h"
#include "stm324xg_eval_lcd.h"
#include "stm324xg_eval_fsmc_sram.h"
#elif MCU == 42
#include "stm32f4xx.h"
#include "fmc_sram.h"
#include "fmc_sdram.h"
#include "stm32f4xx_fmc.h"
#include "stm324x9i_eval.h"
#include "stm324x9i_eval_lcd.h"
#else
#error Invalid MCU
#endif
#include "microej.h"
#include "bsp_customization.h"

#ifndef MICROUI_PNG_BUFFER_SIZE
#define MICROUI_PNG_BUFFER_SIZE	64*1024
#endif


//#include "RGB565_480x272.h"

#define LCD_BASE	((uint32_t)(0x60000000 | 0x04000000))
//#define LCD_BASE	0xC0000000
//#define LCD_BASE LCD_FrameBuf1
//#define LCD_BUFFER	((LCD_TypeDef *) LCD_BASE)

#define WIDTH		LCD_PIXEL_WIDTH
#define HEIGHT		LCD_PIXEL_HEIGHT


#define LCD_FRAME_BUFFER  0x64000000
//#define LCD_FRAME_BUFFER  0xC0000000

//#define MEM_BUFFER	LCD_FRAME_BUFFER 

#define MEM_BUFFER LCD_BASE

#define BUFFER_SIZE WIDTH*HEIGHT*16/8
//#define LCD_FRAME_BUFFER  MEM_BUFFER

#define MEM_WORKING_BUFFER_START MEM_BUFFER+BUFFER_SIZE
//#define MEM_WORKING_BUFFER_START LCD_BASE  
#define MEM_WORKING_BUFFER_SIZE MICROUI_PNG_BUFFER_SIZE
 

#include "RGB565_480x272.h"

extern 	LCD_480272_TypeDef *LCD_FrameBuf;

static void _delay_( uint32_t nCount)
{
	uint32_t index = 0;
	for(index = (10 * nCount); index != 0; index--);
}


uint8_t LLDISPLAY_COPY_IMPL_initialize(LLDISPLAY_COPY* env)
{

  //------------------------------------------------------------------
  //   LCD test
  //------------------------------------------------------------------
  // SRAM & SDRAM both may be used for LCD frame buffer
  //SRAM_Test_Init();
  //SDRAM_Test_Init();
  //SDRAM_Init();
  // Initialize the LCD
//  LCD_RgbInit(SRAM_BUF); 
//  LCD_RgbInit(SDRAM_BUF);
//    LCD_FrameBuf =  (LCD_480272_TypeDef *) EVAL_SRAM_BASE;
	//LCD_Config();
	
 LCD_FrameBuf =  (LCD_480272_TypeDef *) LCD_FRAME_BUFFER ;
  /* Enable the LTDC Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

  LCD_Init();
 //  LTDC_LayerAddress(LTDC_Layer1, MEM_BUFFER);  
  //LTDC_LayerAddress(LTDC_Layer1,(uint32_t)&RGB565_480x272);
	
	LTDC_LayerAddress(LTDC_Layer1,LCD_FRAME_BUFFER); 
	
	/* Enable Layer1 */
  LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  /* Reload configuration of Layer1 */
  LTDC_ReloadConfig(LTDC_IMReload);
  /* Enable The LCD */
  LTDC_Cmd(ENABLE);
	
  LCD_SetBackColor(LCD_COLOR_BLACK);
  LCD_SetTextColor(LCD_COLOR_BLUE);
 // LCD_Clear();
	//LTDC_LayerCmd(LTDC_Layer1, DISABLE);
	//LTDC_LayerAddress(LTDC_Layer2, MEM_BUFFER); 
  //LTDC_LayerCmd(LTDC_Layer2, ENABLE);
  /* Reload configuration of Layer1 */
  //LTDC_ReloadConfig(LTDC_IMReload);		
 
	
  /* Reload configuration of Layer1 */
 // LTDC_ReloadConfig(LTDC_IMReload);		
  //LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  /* Reload configuration of Layer1 */
  //LTDC_ReloadConfig(LTDC_IMReload);		
	
	
	return MICROEJ_TRUE;
}

int32_t LLDISPLAY_COPY_IMPL_getWorkingBufferStartAddress(LLDISPLAY_COPY* env)
{
	printf("mem working buf start = %x\n",MEM_WORKING_BUFFER_START);
	return MEM_WORKING_BUFFER_START;
}

int32_t LLDISPLAY_COPY_IMPL_getWorkingBufferEndAddress(LLDISPLAY_COPY* env)
{
		printf("mem working buf end = %x\n",MEM_WORKING_BUFFER_START+MEM_WORKING_BUFFER_SIZE);
	return MEM_WORKING_BUFFER_START+MEM_WORKING_BUFFER_SIZE;
}

void LLDISPLAY_COPY_IMPL_backlightOff(LLDISPLAY_COPY* env)
{
	// not available yet
}

void LLDISPLAY_COPY_IMPL_backlightOn(LLDISPLAY_COPY* env)
{
	// not available yet
}

int32_t LLDISPLAY_COPY_IMPL_convertDisplayColorToRGBColor(LLDISPLAY_COPY* env, int32_t color)
{
	//			RED													GREEN													BLUE
	//			XXXXX000.00000000						00000XXX.XXX00000							00000000.000XXXXX
	return ((color & 0xf800) << 8) | ((color & 0x07e0) << 5) | ((color & 0x001f) << 3);
}

int32_t LLDISPLAY_COPY_IMPL_convertRGBColorToDisplayColor(LLDISPLAY_COPY* env, int32_t color)
{
	uint32_t rgbColor = (uint32_t)color;
	//			RED															GREEN														BLUE
	//			XXXXX000.00000000								00000XXX.XXX00000								00000000.000XXXXX
	return ((rgbColor & 0xf80000) >> 8) | ((rgbColor & 0x00fc00) >> 5) | ((rgbColor & 0xf8) >> 3);
}

void LLDISPLAY_COPY_IMPL_copyBuffer(LLDISPLAY_COPY* env, int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax)
{
	return;
	uint16_t* addr = (uint16_t*)((int32_t)MEM_BUFFER + (ymin * WIDTH * 2));
	
	int32_t cpt = (ymax - ymin + 1) * WIDTH;	// number of pixels to copy (row aligned)
	
	//LCD_SetCursor(ymin, WIDTH-1);
	uint16_t* LCD_Ptr ;
	
	LCD_Ptr = (uint16_t *)LCD_SetCursor (xmin,ymin); //phv 5/2/13
	//LCD_Ptr= 0;
	
 	LTDC_LayerCmd(LTDC_Layer1, DISABLE);
	LTDC_LayerCmd(LTDC_Layer2, ENABLE);
  /* Reload configuration of Layer1 */
  LTDC_ReloadConfig(LTDC_IMReload);
	

	while(--cpt>=0) {
		//LCD_BUFFER->LCD_RAM = *(addr);
		// phv 18/01
		//val = *(addr++);
		//LCD_WriteRAM(val);
		*(LCD_Ptr++) = *(addr++);

		//LCD_WriteSDRAM(LCD_Ptr++,*(addr++));
//_delay_(1);

	}	
  //LTDC_LayerCmd(LTDC_Layer2, ENABLE);
  /* Reload configuration of Layer2 */
 	
//	  LTDC_ReloadConfig(LTDC_IMReload);
 	LTDC_LayerCmd(LTDC_Layer2, DISABLE);
	LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  /* Reload configuration of Layer1 */
  LTDC_ReloadConfig(LTDC_IMReload);	 
}

int32_t LLDISPLAY_COPY_IMPL_getBacklight(LLDISPLAY_COPY* env)
{
	// not available yet
	return 0;
}

int32_t LLDISPLAY_COPY_IMPL_getBackBufferAddress(LLDISPLAY_COPY* env)
{
	int val;
	// buffer == 0 for sure
	// size == 320*240*16/8 for sure
	//return (int32_t)LCD_buffer;
	printf("MEM_BUFFER = %x\n",MEM_BUFFER);
	return (int32_t)MEM_BUFFER;
}

int32_t LLDISPLAY_COPY_IMPL_getContrast(LLDISPLAY_COPY* env)
{
	// not available yet
	return 0;
}

int32_t LLDISPLAY_COPY_IMPL_getHeight(LLDISPLAY_COPY* env, uint8_t logical)
{
//		printf("HEIGHT = %x\n",HEIGHT);
	return HEIGHT;
}

int32_t LLDISPLAY_COPY_IMPL_getWidth(LLDISPLAY_COPY* env, uint8_t logical)
{
//	printf("WIDTH = %x\n",WIDTH);
	return WIDTH;
}

uint8_t LLDISPLAY_COPY_IMPL_hasBackLight(LLDISPLAY_COPY* env)
{
	return MICROEJ_FALSE;	// not available yet
}

void LLDISPLAY_COPY_IMPL_setBacklight(LLDISPLAY_COPY* env, int32_t backLight)
{
	// not available yet
}

void LLDISPLAY_COPY_IMPL_setContrast(LLDISPLAY_COPY* env, int32_t contrast)
{
	// not available yet
}

void LLDISPLAY_COPY_IMPL_synchronize(LLDISPLAY_COPY* env)
{
	// it is not possible to synchronize the display (missing hardware capacity)
}
