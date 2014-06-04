/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */ 

#include "LLDISPLAY_STM32x0DISCO.h"
#define LLDISPLAY_SWITCH_IMPL LLDISPLAY_STM32x0DISCO
#include "LLDISPLAY_SWITCH_impl.h"
#include "cpu.h"

#include "stm32f4xx.h"
#include "fmc_sram.h"
#include "fmc_sdram.h"
#include "stm32f4xx_fmc.h"
#include "stm324x9i_eval_fmc_sram.h"
#include "stm324x9i_eval.h"
#include "stm324x9i_eval_lcd.h"

#include "microej.h"
#include "bsp_customization.h"
#include <stdio.h>

#ifndef MICROUI_PNG_BUFFER_SIZE
#define MICROUI_PNG_BUFFER_SIZE	10*1024 // TODO was 64*1024
#endif

#define WIDTH		LCD_PIXEL_WIDTH
#define HEIGHT	LCD_PIXEL_HEIGHT

#define LCD_FRAME_BUFFER  0xC0000000

#define BUFFER_SIZE (WIDTH*HEIGHT*16/8)
#define MEM_BUFFER (LCD_FRAME_BUFFER+BUFFER_SIZE)

#define MEM_WORKING_BUFFER_START (MEM_BUFFER+BUFFER_SIZE)
#define MEM_WORKING_BUFFER_SIZE MICROUI_PNG_BUFFER_SIZE

//extern 	LCD_480272_TypeDef *LCD_FrameBuf;

uint8_t LLDISPLAY_SWITCH_IMPL_initialize(LLDISPLAY_SWITCH* env) {
	
  /* Enable the LTDC Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

  LCD_Init();					//Included SDRAM initialization
	
	LTDC_LayerAddress(LTDC_Layer1,LCD_FRAME_BUFFER); 
	LTDC_LayerAddress(LTDC_Layer2,MEM_BUFFER);	
	
	/* Enable Layer1 */
  LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  LTDC_LayerCmd(LTDC_Layer2, DISABLE);
  /* Reload configuration of Layer1 */
  LTDC_ReloadConfig(LTDC_IMReload);
	
  /* Enable The LCD */
  LTDC_Cmd(ENABLE);
	
	LTDC_ITConfig(LTDC_FLAG_RR, ENABLE);
	
	//printf("lcd buffer 0x%x\n", LCD_FRAME_BUFFER);
	//printf("mem buffer 0x%x\n", MEM_BUFFER);
	//printf("buffer size %d\n", BUFFER_SIZE);
	//printf("working buffer %x\n", MEM_WORKING_BUFFER_START);
	//printf("working buffer size %d\n", MEM_WORKING_BUFFER_SIZE);
	
	return MICROEJ_TRUE;
}

int32_t LLDISPLAY_SWITCH_IMPL_getWorkingBufferStartAddress(LLDISPLAY_SWITCH* env) {
	return (int32_t)MEM_WORKING_BUFFER_START;
}


int32_t LLDISPLAY_SWITCH_IMPL_getDisplayBufferAddress(LLDISPLAY_SWITCH* env) {
	return LCD_FRAME_BUFFER;
}

static uint32_t buffer_src;
static uint32_t buffer_dst;
static uint32_t buffer_length;

void LLDISPLAY_SWITCH_IMPL_setDisplayBufferAddress(LLDISPLAY_SWITCH* env,int32_t addr, int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax) {
	
	if (addr == LCD_FRAME_BUFFER) {   
		LTDC_LayerCmd(LTDC_Layer1, ENABLE);
    LTDC_LayerCmd(LTDC_Layer2, DISABLE);
		buffer_src = (uint32_t)LCD_FRAME_BUFFER;
		buffer_dst = (uint32_t)MEM_BUFFER;
	} else {
 		LTDC_LayerCmd(LTDC_Layer2, ENABLE);
		LTDC_LayerCmd(LTDC_Layer1, DISABLE);
		buffer_dst = (uint32_t)LCD_FRAME_BUFFER;
		buffer_src = (uint32_t)MEM_BUFFER;
	}
#define OPTIM_SQUARE 1 // TODO test
#if OPTIM_SQUARE
	buffer_src += ((WIDTH * ymin) * 16 / 8) >> 0;
	buffer_dst += ((WIDTH * ymin) * 16 / 8) >> 0;
	buffer_length = ((WIDTH * (ymax - ymin + 1) * 16 / 8)) >> 0;
#else
	buffer_length = ((WIDTH * HEIGHT * 16 / 8)) >> 0;
#endif
	
	LTDC_ReloadConfig(LTDC_VBReload);
	LTDC_ClearFlag(LTDC_FLAG_RR);
		
	return ;
}

int32_t LLDISPLAY_SWITCH_IMPL_getWorkingBufferEndAddress(LLDISPLAY_SWITCH* env) {
	return (int32_t)(MEM_WORKING_BUFFER_START+MEM_WORKING_BUFFER_SIZE);
}


int32_t LLDISPLAY_SWITCH_IMPL_convertDisplayColorToRGBColor(
			LLDISPLAY_SWITCH* env,
			int32_t color) {
	//			RED													GREEN													BLUE
	//			XXXXX000.00000000						00000XXX.XXX00000							00000000.000XXXXX
	return ((color & 0xf800) << 8) | ((color & 0x07e0) << 5) | ((color & 0x001f) << 3);
}

int32_t LLDISPLAY_SWITCH_IMPL_convertRGBColorToDisplayColor(
			LLDISPLAY_SWITCH* env,
			int32_t color) {
	uint32_t rgbColor = (uint32_t)color;
	/* The RGB value encodes:
	0xf80000 = 1111 1000 0000 0000 0000 0000 - the RED part
	0x00fc00 =           1111 1100 0000 0000 - the GREEN part
	0xf8     =                     1111 1000 - the BLUE part
	*/
	return 	((rgbColor & 0xf80000) >> 8) |
			((rgbColor & 0x00fc00) >> 5) |
			((rgbColor & 0xf8) >> 3);
}


int32_t LLDISPLAY_SWITCH_IMPL_getBackBufferAddress(LLDISPLAY_SWITCH* env) 
{
	return (int32_t)MEM_BUFFER;
}

int32_t LLDISPLAY_SWITCH_IMPL_getHeight(LLDISPLAY_SWITCH* env, uint8_t logical) {
	return HEIGHT;
}

int32_t LLDISPLAY_SWITCH_IMPL_getWidth(LLDISPLAY_SWITCH* env, uint8_t logical) {
	return WIDTH;
}

void LLDISPLAY_SWITCH_IMPL_synchronize(LLDISPLAY_SWITCH* env) {
	
	if (buffer_length != 0){
		
		while(LTDC_GetFlagStatus(LTDC_FLAG_RR) == RESET);
		
		memcpy(buffer_dst, buffer_src, buffer_length);
		buffer_length = 0;
	} 
	
	// it is not possible to synchronize the display (missing hardware capacity)
	//LTDC_ReloadConfig(LTDC_VBReload);
}

void LLDISPLAY_SWITCH_IMPL_backlightOff(LLDISPLAY_SWITCH* env) {
}

void LLDISPLAY_SWITCH_IMPL_backlightOn(LLDISPLAY_SWITCH* env) {
}

int32_t LLDISPLAY_SWITCH_IMPL_getBacklight(LLDISPLAY_SWITCH* env) {
	return 0;
}

uint8_t LLDISPLAY_SWITCH_IMPL_hasBackLight(LLDISPLAY_SWITCH* env) {
	return MICROEJ_FALSE;
}

void LLDISPLAY_SWITCH_IMPL_setBacklight(LLDISPLAY_SWITCH* env, int32_t backLight) {
}

int32_t LLDISPLAY_SWITCH_IMPL_getContrast(LLDISPLAY_SWITCH* env) {
	return 0;
}

void LLDISPLAY_SWITCH_IMPL_setContrast(LLDISPLAY_SWITCH* env, int32_t contrast) {
}


int32_t LLDISPLAY_SWITCH_IMPL_getFontAlphaAdjustment(LLDISPLAY_SWITCH* env, int32_t bpp)
{
	// '25' has been calibrated manually
    return bpp == 2 ? 25 : 0;
}


uint8_t LLDISPLAY_SWITCH_IMPL_isColor(LLDISPLAY_SWITCH* env)
{
	return MICROEJ_TRUE;
}


int32_t LLDISPLAY_SWITCH_IMPL_getNumberOfColors(LLDISPLAY_SWITCH* env)
{
	return (1 << 16) - 1;
}

void display_test(void)
{
// 	printf("READY\n");
// 	LLDISPLAY_SWITCH_IMPL_initialize(0);
// 	memset((void*)LLDISPLAY_SWITCH_IMPL_getDisplayBufferAddress(), 0xff, 50);
// 	printf("READY2\n");
}

