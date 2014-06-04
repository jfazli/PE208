/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
 
#include "LLDISPLAY_STM32x0DISCO.h"
#include <stdint.h>

#include "LLDISPLAY_DECODER_PNG.h"
#include "LLDISPLAY_DECODER_BMPM.h"

#include "bsp_customization.h"

// IO structs
static struct LLDISPLAY_STM32x0DISCO lcd_driver;

// IO creation methods
void* createLCD(void);

// display stack tables
void* LCD_TABLE[] = {
	createLCD,
	0
};
void* IMAGE_DECODERS_TABLE[] = {
	LLDISPLAY_DECODER_PNG_createPNGImageDecoder,
	LLDISPLAY_DECODER_BMPM_createMonochromBMPImageDecoder,
	0
};

void* createLCD(void)
{
	LLDISPLAY_STM32x0DISCO_new(&lcd_driver);
	return &lcd_driver;
}


