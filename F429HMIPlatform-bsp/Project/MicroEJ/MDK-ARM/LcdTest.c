/*
 * C
 *
 * Copyright 2013 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/**
 * This file proposes some basic LCD tests (draw pixels, fill
 * rectangles, framerate etc.).
 *
 * Note: the tests work only on a LCD driver which manages the
 * double buffer mode (switch or copy).
 *
 *
 * To use this test, you have:
 *
 * 1-	Fill "lcdTest_conf.h" which defines your configuration.
 * 		Please see comments in "lcdTest.h"
 *
 * 		Example:
 *
 *		#include "lcdTest.h"
 *		#define LCD_TYPE LCD_TYPE_SWITCH
 *		#define LCD_BPP LCD_BPP_16
 *		#define LCD_LAYOUT LCD_LAYOUT_XY
 *		#define LCD_LINK LCD_LINK_AGAINST_DISPLAY_STACK
 *		#define LLDISPLAY_SWITCH_IMPL LLDISPLAY_MYLCD
 *
 * 2-	From another C file, implements the functions defined in
 * 		"lcdTest_impl.h" file. These functions are implementation
 * 		dependent.
 *
 * 3-	From another C file (or the same as point 2-), call a
 * 		function defined in "lcdTest.h" to launch a test.
 *
 * You don't have to / no need to:
 *
 * -	Modify this file. There are enough basic tests which are
 * 		fully generic for all LCD drivers which implement functions
 * 		defined in "LLDISPLAY*h" file.
 *
 * -	Include the file "lcdTest_intern.h": it is an internal LCD
 * 		test file.
 */

/* ====================================================================================================================
 * Includes
 * ==================================================================================================================*/

#include "lcdTest.h"		// public lcdTest methods
#include "lcdTest_conf.h"	// specific BSP configuration
#include "lcdTest_impl.h"	// methods the BSP has to implement
#include "lcdTest_intern.h"	// internal lcdTest header file MUST NOT BE INCLUDED IN ANOTHER FILE

/* ====================================================================================================================
 * Macros
 * ==================================================================================================================*/

#define MIN(a,b) ((a) > (b) ? (b) : (a))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define RAND(a) (rand() % (a))

/* ====================================================================================================================
 * Defines
 * ==================================================================================================================*/

#define NULL 0

/*
 * Used to get logical or physical width / height
 */
#define PHYSICAL 0
#define LOGICAL 1

/*
 * Used to force or not to update the flush data
 */
#define FLUSH_DO_NOT_UPDATE 0
#define FLUSH_UPDATE 1

/*
 * RGB Colors
 */
#define BLACK  0x000000
#define BLUE  0x0000ff
#define CYAN  0x00ffff
#define GRAY  0x808080
#define GREEN  0x008000
#define LIME  0x00ff00
#define MAGENTA 0xff00ff
#define MAROON  0x800000
#define NAVY  0x000080
#define OLIVE  0x808000
#define PURPLE  0x800080
#define RED  0xff0000
#define SILVER  0xc0c0c0
#define TEAL  0x008080
#define WHITE  0xffffff
#define YELLOW  0xffff00

/*
 * Others
 */
#define TRUE 0
#define FALSE 1

/* ====================================================================================================================
 *
 * ==================================================================================================================*/

typedef void(*lcdTest_method)();

/* ====================================================================================================================
 * Locals
 * ==================================================================================================================*/

static lcdMem current_color;	// lcd color (not RGB color)
static lcdMem* working_buffer;
static int32_t fxmin, fxmax, fymin, fymax;	// flush data

#if LCD_TYPE == LCD_TYPE_SWITCH
static lcdMem* lcd_buffer;
#endif

/* ====================================================================================================================
 * Tools methods
 * ==================================================================================================================*/

/*
 * Reset flush data
 */
void lcdTest_resetFlush(void) {
	fxmin = LLDISPLAY_getWidth(LOGICAL);
	fxmax = 0;
	fymin = LLDISPLAY_getHeight(LOGICAL);
	fymax = 0;
}

/*
 * Update flush data
 */
void lcdTest_updateFlush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int8_t updateFlushData) {
	if (updateFlushData) {
		fxmin = MIN(x1, fxmin);
		fxmax = MAX(x2, fxmax);
		fymin = MIN(y1, fymin);
		fymax = MAX(y2, fymax);
	}
}

void lcdTest_forceFullFlush(void) {
	fxmin = 0;
	fxmax = LLDISPLAY_getWidth(LOGICAL) - 1;
	fymin = 0;
	fymax = LLDISPLAY_getHeight(LOGICAL) - 1;
}

void lcdTest_checkBounds(int32_t value, int32_t min, int32_t max){
	if (value < min || value > max){
		// invalid value -> stop execution
		while(1);
	}
}

/* ====================================================================================================================
 * Local methods
 * ==================================================================================================================*/

/*
 * Initialize the LCD driver and the LCD bench data
 */
void lcdTest_initialize(void){
	LLDISPLAY_initialize();
	current_color = 0;
	lcdTest_resetFlush();
	lcdTest_srandInit();

	working_buffer = (lcdMem*)LLDISPLAY_getBackBufferAddress();

#if LCD_TYPE == LCD_TYPE_SWITCH
	lcd_buffer = (lcdMem*)LLDISPLAY_getDisplayBufferAddress();
#endif
}

/*
 * Ask to the LCD driver to flush a part of the working buffer
 */
void lcdTest_flush(void){

	if (fxmin != LLDISPLAY_getWidth(LOGICAL) && fymin != LLDISPLAY_getHeight(LOGICAL)) {
	
#if LCD_TYPE == LCD_TYPE_COPY
		LLDISPLAY_copyBuffer(fxmin, fymin, fxmax, fymax);
#elif LCD_TYPE == LCD_TYPE_SWITCH
		lcdMem* temp;
		LLDISPLAY_setDisplayBufferAddress(working_buffer);
		temp = working_buffer;
		working_buffer = lcd_buffer;
		lcd_buffer = temp;
#endif
	}
	// else: nothing to flush
	
	lcdTest_resetFlush();
}

/*
 * Ask to the LCD driver to flush all working buffer
 */
void lcdTest_flushAll(void){
	lcdTest_forceFullFlush();
	lcdTest_flush();
}

/*
 * Store the current color
 */
lcdMem lcdTest_setCurrentColor(int32_t colorRGB) {
	current_color = (lcdMem)LLDISPLAY_convertRGBColorToDisplayColor(colorRGB);
	return current_color;
}

/* ====================================================================================================================
 * Methods to draw something With the Current Color (WCC)
 * ==================================================================================================================*/

/*
 * Draw a pixel
 */
void lcdTest_drawPixelWCC(int32_t x, int32_t y, int8_t updateFlushData) {
	*(working_buffer + ((y * LLDISPLAY_getWidth(PHYSICAL)) + x)) = current_color;
	lcdTest_updateFlush(x, y, x, y, updateFlushData);
}

/*
 * Draws an horizontal line. x1 <= x2 for sure (checked by callers)
 */
static void lcdTest_drawHLineWCC(int32_t x1, int32_t x2, int32_t y, int8_t updateFlushData){
	int32_t row = LLDISPLAY_getWidth(PHYSICAL);
	uint32_t color = (uint32_t)current_color;

	lcdTest_updateFlush(x1, y, x2, y, updateFlushData);

#if LCD_BPP == LCD_BPP_16

#if LCD_LAYOUT == LCD_LAYOUT_XY

	int32_t yrow = y * row;

	int32_t sPix = yrow + x1;
	if ((sPix & 1) == 1) {
		// plot first pixel
		lcdTest_drawPixelWCC(x1, y, FLUSH_DO_NOT_UPDATE);
		++sPix;
	}

	int ePix = yrow + x2;
	if ((ePix & 1) == 0) {
		// plot last pixel
		lcdTest_drawPixelWCC(x2, y, FLUSH_DO_NOT_UPDATE);
		--ePix;
	}

	// get first and last address
	uint32_t* sAddr = (uint32_t*)(working_buffer + sPix);
	uint32_t* eAddr = (uint32_t*)(working_buffer + ePix);

	// on one 'uint32_t', there is two pixels
	color = (color << 16) | (color & 0xffff);

	// draw pixels 2 by 2
	for(;sAddr <= eAddr; sAddr++) {
		*(sAddr) = color;
	}

#else /*LCD_LAYOUT_YX*/

	uint32_t sPix = x1*row+y;
	uint32_t ePix = x2*row+y;

	uint16_t* addr = (uint16_t*)(sPix + working_buffer);
	uint16_t* endAddr = (uint16_t*)(ePix + working_buffer);
	uint32_t inc = row;

	for(;addr<=endAddr;addr+=inc)
		*(addr) = color;

#endif	/*LCD_LAYOUT*/

#elif LCD_BPP == LCD_BPP_32

#if LCD_LAYOUT == LCD_LAYOUT_XY

	int32_t yrow = y * row;

	int32_t sPix = yrow + x1;
	int32_t ePix = yrow + x2;

	// get first and last address
	uint32_t* sAddr = (uint32_t*)(working_buffer + sPix);
	uint32_t* eAddr = (uint32_t*)(working_buffer + ePix);

	// draw pixels
	for(;sAddr <= eAddr; sAddr++) {
		*(sAddr) = color;
	}

#else /*LCD_LAYOUT_YX*/

	uint32_t sPix = x1*row+y;
	uint32_t ePix = x2*row+y;

	uint32_t* addr = (uint32_t*)(sPix + working_buffer);
	uint32_t* endAddr = (uint32_t*)(ePix + working_buffer);
	uint32_t inc = row;

	for(;addr<=endAddr;addr+=inc)
		*(addr) = color;

#endif	/*LCD_LAYOUT*/

#endif	/*LCD_BPP*/

}

/**
 * Draws a vertical line. y1 <= y2 for sure (checked by callers)
 */
void lcdTest_drawVLineWCC(int x, int y1, int y2, int8_t updateFlushData){

	int32_t row = LLDISPLAY_getWidth(PHYSICAL);
	uint32_t color = (uint32_t)current_color;

	lcdTest_updateFlush(x, y1, x, y2, updateFlushData);

#if LCD_BPP == LCD_BPP_16

#if LCD_LAYOUT == LCD_LAYOUT_XY

	uint32_t sPix = y1*row+x;
	uint32_t ePix = y2*row+x;

	uint16_t* addr = (uint16_t*)(sPix + working_buffer);
	uint16_t* endAddr = (uint16_t*)(ePix + working_buffer);
	uint32_t inc = row;

	for(;addr<=endAddr;addr+=inc)
		*(addr) = color;

#else /*LCD_LAYOUT_XY*/

	uint32_t xrow = x * row;

	uint32_t sPix = xrow + y1;
	if ((sPix & 1) == 1) {
		// plot first pixel
		lcdTest_drawPixelWCC(x, y1, FLUSH_DO_NOT_UPDATE);
		++sPix;
	}

	uint32_t ePix = xrow + y2;
	if ((ePix & 1) == 0) {
		// plot last pixel
		lcdTest_drawPixelWCC(x, y2, FLUSH_DO_NOT_UPDATE);
		--ePix;
	}

	// get first and last address
	uint32_t* sAddr = (uint32_t*)(working_buffer + sPix);
	uint32_t* eAddr = (uint32_t*)(working_buffer + ePix);

	// on one 'uint32_t', there is two pixels
	color = (color << 16) | (color & 0xffff);

	// draw pixels 2 by 2
	for(;sAddr <= eAddr; sAddr++) {
		*(sAddr) = color;
	}
#endif	/*LCD_LAYOUT*/

#elif LCD_BPP == LCD_BPP_32

#if LCD_LAYOUT == LCD_LAYOUT_XY

	uint32_t sPix = y1*row+x;
	uint32_t ePix = y2*row+x;

	uint32_t* addr = (uint32_t*)(sPix + working_buffer);
	uint32_t* endAddr = (uint32_t*)(ePix + working_buffer);
	uint32_t inc = row;

	for(;addr<=endAddr;addr+=inc)
		*(addr) = color;

#else /*LCD_LAYOUT_XY*/

	uint32_t xrow = x * row;

	uint32_t sPix = xrow + y1;
	uint32_t ePix = xrow + y2;

	// get first and last address
	uint32_t* sAddr = (uint32_t*)(working_buffer + sPix);
	uint32_t* eAddr = (uint32_t*)(working_buffer + ePix);

	// draw pixels
	for(;sAddr <= eAddr; sAddr++) {
		*(sAddr) = color;
	}

#endif	/*LCD_LAYOUT*/

#endif	/*LCD_BPP*/

}

/*
 * Fill a rectangle
 */
void lcdTest_fillRectWCC(int32_t x1, int32_t y1, int32_t x2, int32_t y2){

	int32_t row = LLDISPLAY_getWidth(PHYSICAL);
	uint32_t color = (uint32_t)current_color;

	lcdTest_updateFlush(x1, y1, x2, y2, FLUSH_UPDATE);

#if LCD_BPP == LCD_BPP_16

#if LCD_LAYOUT == LCD_LAYOUT_XY

	uint32_t yrow = y1 * row;

	uint32_t sPix = yrow + x1;
	if ((sPix & 1) == 1) {
		// plot first v line
		lcdTest_drawVLineWCC(x1, y1, y2, FLUSH_DO_NOT_UPDATE);
		++sPix;
	}

	uint32_t ePix = yrow + x2;
	if ((ePix & 1) == 0) {
		// plot last v line
		lcdTest_drawVLineWCC(x2, y1, y2, FLUSH_DO_NOT_UPDATE);
		--ePix;
	}

	// get first and last address
	uint32_t* sAddr = (uint32_t*)(working_buffer + sPix);
	uint32_t* eAddr = (uint32_t*)(working_buffer + ePix);
	uint32_t* sBaseAddr = sAddr;

	// on one 'uint32_t', there is two pixels
	color = (color << 16) | (color & 0xffff);

	// v increment
	uint32_t inc = row / 2 ;

	for(;y1<=y2;y1++) {
		// draw h line, draw pixels 2 by 2
		for(;sAddr <= eAddr; sAddr++) {
			*(sAddr) = color;
		}

		// go to next line
		sBaseAddr += inc;
		eAddr += inc;
		sAddr = sBaseAddr;
	}

#else /*LCD_LAYOUT_XY*/

	uint32_t xrow = x1 * row;

	uint32_t sPix = xrow + y1;
	uint32_t ((sPix & 1) == 1) {
		// plot first h line
		lcdTest_drawHLineWCC(x1, x2, y1, FLUSH_DO_NOT_UPDATE);
		++sPix;
	}

	uint32_t ePix = xrow + y2;
	if ((ePix & 1) == 0) {
		// plot last vline
		lcdTest_drawHLineWCC(x1, x2, y2, FLUSH_DO_NOT_UPDATE);
		--ePix;
	}

	// get first and last address
	uint32_t* sAddr = (uint32_t*)(working_buffer + sPix);
	uint32_t* eAddr = (uint32_t*)(working_buffer + ePix);
	uint32_t* sBaseAddr = sAddr;

	// on one 'uint32_t', there is two pixels
	color = (color << 16) | (color & 0xffff);

	// h increment
	uint32_t inc = row / 2;

	for(;x1<=x2;x1++) {
		// draw v line, draw pixels 2 by 2
		for(;sAddr <= eAddr; sAddr++) {
			*(sAddr) = color;
		}

		// go to next line
		sBaseAddr += inc;
		eAddr += inc;
		sAddr = sBaseAddr;
	}
#endif	/*LCD_LAYOUT*/

#elif LCD_BPP == LCD_BPP_32

#if LCD_LAYOUT == LCD_LAYOUT_XY

	uint32_t yrow = y1 * row;

	uint32_t sPix = yrow + x1;
	uint32_t ePix = yrow + x2;

	// get first and last address
	uint32_t* sAddr = (uint32_t*)(working_buffer + sPix);
	uint32_t* eAddr = (uint32_t*)(working_buffer + ePix);
	uint32_t* sBaseAddr = sAddr;

	// v increment
	uint32_t inc = row / 4;

	for(;y1<=y2;y1++) {
		// draw h line
		for(;sAddr <= eAddr; sAddr++) {
			*(sAddr) = color;
		}

		// go to next line
		sBaseAddr += inc;
		eAddr += inc;
		sAddr = sBaseAddr;
	}

#else /*LCD_LAYOUT_XY*/

	uint32_t xrow = x1 * row;

	uint32_t sPix = xrow + y1;
	uint32_t ePix = xrow + y2;

	// get first and last address
	uint32_t* sAddr = (uint32_t*)(working_buffer + sPix);
	uint32_t* eAddr = (uint32_t*)(working_buffer + ePix);
	uint32_t* sBaseAddr = sAddr;

	// h increment
	uint32_t inc = row / 4;

	for(;x1<=x2;x1++) {
		// draw v line
		for(;sAddr <= eAddr; sAddr++) {
			*(sAddr) = color;
		}

		// go to next line
		sBaseAddr += inc;
		eAddr += inc;
		sAddr = sBaseAddr;
	}

#endif	/*LCD_LAYOUT*/

#endif	/*LCD_BPP*/
}

/*
 * Fill all working buffer with current color
 */
void lcdTest_fillAllWCC(void) {
	lcdTest_fillRectWCC(0, 0, LLDISPLAY_getWidth(LOGICAL) - 1, LLDISPLAY_getHeight(LOGICAL) - 1);
}

/* ====================================================================================================================
 * Methods to draw something
 * ==================================================================================================================

/*
 * Draw a pixel on the working buffer
 */
void lcdTest_drawPixel(int32_t x, int32_t y, int32_t colorRGB) {
	lcdTest_setCurrentColor(colorRGB);
	lcdTest_drawPixelWCC(x, y, FLUSH_UPDATE);
}

/*
 * Draws an horizontal line. x1 <= x2 for sure (checked by callers)
 */
static void lcdTest_drawHLine(int32_t x1, int32_t x2, int32_t y, int32_t colorRGB){
	lcdTest_setCurrentColor(colorRGB);
	lcdTest_drawHLineWCC(x1 , x2 , y, FLUSH_UPDATE);
}

/**
 * Draws a vertical line. y1 <= y2 for sure (checked by callers)
 */
void lcdTest_drawVLine(int x, int y1, int y2, int colorRGB){
	lcdTest_setCurrentColor(colorRGB);
	lcdTest_drawVLineWCC(x, y1, y2, FLUSH_UPDATE);
}

/*
 * Fill a rectangle
 */
void lcdTest_fillRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t colorRGB){
	lcdTest_setCurrentColor(colorRGB);
	lcdTest_fillRectWCC(x1, y1, x2, y2);
}

/*
 * Fill all working buffer
 */
void lcdTest_fillAll(int32_t colorRGB) {
	lcdTest_setCurrentColor(colorRGB);
	lcdTest_fillAllWCC();
}

/*
 * Fill a random rectangle with a random color
 */
void lcdTest_fillRandRect(void) {
	int32_t temp;
	int32_t x1 = RAND(LLDISPLAY_getWidth(LOGICAL));
	int32_t x2 = RAND(LLDISPLAY_getWidth(LOGICAL));
	int32_t y1 = RAND(LLDISPLAY_getHeight(LOGICAL));
	int32_t y2 = RAND(LLDISPLAY_getHeight(LOGICAL));
	int32_t color = RAND(0xffffff);

	if (x1 > x2) {
		temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (y1 > y2) {
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	lcdTest_fillRect(x1, y1, x2, y2, color);
}

/*
 * Fill a rectangle at a random position with a random color
 */
static uint32_t lcdTest_fillRandRect2_width;
static uint32_t lcdTest_fillRandRect2_height;
static uint32_t lcdTest_fillRandRect2_randWidth;
static uint32_t lcdTest_fillRandRect2_randHeight;
static uint8_t lcdTest_fillRandRect2_align;
void lcdTest_fillRandRect2(void) {
	uint32_t x1 = RAND(lcdTest_fillRandRect2_randWidth);
	uint32_t y1 = RAND(lcdTest_fillRandRect2_randHeight);
	if (lcdTest_fillRandRect2_align){
		x1 *= lcdTest_fillRandRect2_width;
		y1 *= lcdTest_fillRandRect2_height;
	}

	uint32_t x2 = x1 + lcdTest_fillRandRect2_width - 1;
	uint32_t y2 = y1 + lcdTest_fillRandRect2_height - 1;
	int32_t color = RAND(0xffffff);

	lcdTest_fillRect(x1, y1, x2, y2, color);
}

/* ====================================================================================================================
 * Framerate methods
 * ==================================================================================================================*/

void lcdTest_framerateEmptyMethod(void) {

}

/*
 * Internal framerate method
 */
void lcdTest_framerate(lcdTest_method method, uint32_t nbLoops, uint8_t flush) {
	lcdTest_initialize();
	lcdTest_fillAll(BLACK);
	int32_t temp;
	uint64_t t0, emptyTime;
	uint64_t freq;

	while(1){
		
		// get time to do the loop (may be near to 0)
		emptyTime = lcdTest_getTime();
		for(temp = nbLoops; --temp>=0;){
			if (flush == TRUE) {
				LLDISPLAY_synchronize();
			}
			lcdTest_framerateEmptyMethod();
			if (flush == TRUE) {
				lcdTest_flush();
			}
		}
		emptyTime = lcdTest_getTime() - emptyTime;
		
		// get time
		t0 = lcdTest_getTime();
		for(temp = nbLoops; --temp>=0;){
			if (flush == TRUE) {
				LLDISPLAY_synchronize();
			}
			method();
			if (flush == TRUE) {
				lcdTest_flush();
			}
		}
		t0 = lcdTest_getTime() - t0 - emptyTime;
			
		freq = (1000 * nbLoops) / t0;
		t0 *= (1000000/nbLoops);
		
		lcdTest_printFramerate((uint32_t)t0, (uint32_t)freq);
	}
}

void lcdTest_framerateForceFlush(void) {
	lcdTest_forceFullFlush();
}

static lcdMem lcdTest_framerateFullRect_color1;
static lcdMem lcdTest_framerateFullRect_color2;
void lcdTest_framerateFullRect(void) {
	current_color = current_color == lcdTest_framerateFullRect_color1 ? lcdTest_framerateFullRect_color2 : lcdTest_framerateFullRect_color1;
	lcdTest_fillAllWCC();
}

/* ====================================================================================================================
 * Public methods
 * ==================================================================================================================*/

/*
 * Draw the square (0,0) to (1,1) with 4 colors
 */
void lcdTest_pixels1(void) {
	lcdTest_initialize();
	lcdTest_drawPixel(0, 0, RED);
	lcdTest_drawPixel(0, 1, GREEN);
	lcdTest_drawPixel(1, 1, BLUE);
	lcdTest_drawPixel(1, 0, YELLOW);
	lcdTest_flush();
}

/*
 * Draw a color in each LCD corner
 */
void lcdTest_pixels2(void) {
	lcdTest_initialize();
	lcdTest_drawPixel(0, 0, RED);
	lcdTest_drawPixel(0, LLDISPLAY_getHeight(LOGICAL) - 1, GREEN);
	lcdTest_drawPixel(LLDISPLAY_getWidth(LOGICAL) - 1, LLDISPLAY_getHeight(LOGICAL) - 1, BLUE);
	lcdTest_drawPixel(LLDISPLAY_getWidth(LOGICAL) - 1, 0, YELLOW);
	lcdTest_flush();
}

/*
 * Fill full LCD with 3 consecutive colors
 */
void lcdTest_fillRect1(void) {
	lcdTest_initialize();
	while(1){
		LLDISPLAY_synchronize();
		lcdTest_fillAll(RED);
		lcdTest_flushAll();
		LLDISPLAY_synchronize();
		lcdTest_fillAll(GREEN);
		lcdTest_flushAll();
		LLDISPLAY_synchronize();
		lcdTest_fillAll(BLUE);
		lcdTest_flushAll();
	}
}

/*
 * Fill random rectangles
 */
void lcdTest_fillRect2(void) {
	lcdTest_initialize();

	while(1){
		LLDISPLAY_synchronize();
		lcdTest_fillRandRect();
		lcdTest_flushAll();
	}
}

/*
 * Calculate the flush time only
 */
void lcdTest_framerateFlush(uint32_t nbLoops) {
	lcdTest_framerate(&lcdTest_framerateForceFlush, nbLoops, TRUE);
}

/*
 * Calculate the fill rect framerate (without flush)
 */
void lcdTest_framerateFill(uint32_t nbLoops) {
	lcdTest_framerateFullRect_color1 = lcdTest_setCurrentColor(RED);
	lcdTest_framerateFullRect_color2 = lcdTest_setCurrentColor(BLUE);
	lcdTest_framerate(&lcdTest_framerateFullRect, nbLoops, FALSE);
}

/*
 * Calculate the real framerate (draw all LCD + flush)
 */
void lcdTest_framerateFillAndFlush(uint32_t nbLoops) {
	lcdTest_framerateFullRect_color1 = lcdTest_setCurrentColor(RED);
	lcdTest_framerateFullRect_color2 = lcdTest_setCurrentColor(BLUE);
	lcdTest_framerate(&lcdTest_framerateFullRect, nbLoops, TRUE);
}

/*
 * Calculate a framerate with random rectangles
 */
void lcdTest_framerateRandomRect(uint32_t nbLoops) {
	lcdTest_framerate(&lcdTest_fillRandRect, nbLoops, TRUE);
}

/*
 * Calculate a framerate with random rectangles
 */
void lcdTest_framerateRandomRect2(uint32_t nbLoops, uint32_t width, uint32_t height, uint8_t align) {
	lcdTest_checkBounds(width, 0, LLDISPLAY_getWidth(LOGICAL));
	lcdTest_checkBounds(height, 0, LLDISPLAY_getHeight(LOGICAL));
	lcdTest_fillRandRect2_width = width;
	lcdTest_fillRandRect2_height = height;
	lcdTest_fillRandRect2_align = align;

	if (align){
		lcdTest_fillRandRect2_randWidth = (LLDISPLAY_getWidth(LOGICAL) - (LLDISPLAY_getWidth(LOGICAL) % lcdTest_fillRandRect2_width)) / lcdTest_fillRandRect2_width;
		lcdTest_fillRandRect2_randHeight = (LLDISPLAY_getHeight(LOGICAL) - (LLDISPLAY_getHeight(LOGICAL) % lcdTest_fillRandRect2_height)) / lcdTest_fillRandRect2_height;
	}
	else{
		lcdTest_fillRandRect2_randWidth = LLDISPLAY_getWidth(LOGICAL) - lcdTest_fillRandRect2_width;
		lcdTest_fillRandRect2_randHeight = LLDISPLAY_getHeight(LOGICAL) - lcdTest_fillRandRect2_height;
	}

	lcdTest_framerate(&lcdTest_fillRandRect2, nbLoops, TRUE);
}

