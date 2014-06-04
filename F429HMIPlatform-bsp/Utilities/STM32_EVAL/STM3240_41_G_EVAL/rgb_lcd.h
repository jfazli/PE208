/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RGB_LCD_H
#define __RGB_LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"



#define USE_LCD_480272
//#define USE_LCD_640480

#ifdef USE_LCD_480272
#define LCD_PIXEL_WIDTH  480
#define LCD_PIXEL_HEIGHT  272
#else
#define LCD_PIXEL_WIDTH   640
#define LCD_PIXEL_HEIGHT  480
#endif

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  vu16 HWord[LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT];//16-bit per pixel
} LCD_480272_TypeDef;

typedef struct
{
  vu16 HWord[LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT];//16-bit per pixel
} LCD_640480_TypeDef;

/* Exported variables --------------------------------------------------------*/

typedef struct
{
	 u16 LCD_Frame2[480*24];
} LCD_FrameBUF2_Typedef;


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
// either LCD_480272(default) or LCD_640480 should be used


//#define LCD_FrameBuf1         ((LCD_480272_TypeDef *) EVAL_SRAM_BASE)
//#define LCD_FrameBuf2         ((LCD_640480_TypeDef *) EVAL_SRAM_BASE)

#ifdef USE_LCD_480272
extern LCD_480272_TypeDef *LCD_FrameBuf;
#else
extern LCD_640480_TypeDef *LCD_FrameBuf;
#endif


extern LCD_FrameBUF2_Typedef *LCD_Ptr_FrameL2;

#define SRAM_BUF  0
#define SDRAM_BUF 1

/* Exported functions ------------------------------------------------------- */

/*----- High layer function -----*/
void LCD_RgbInit(u8 buf);
void LCD_RgbClear(u16 Color);
void LCD_RgbL2SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Width, u16 Height);

/*----- Low layer function -----*/
void LCD_RgbConfig(void);

#endif /* __RGB_LCD_H */
