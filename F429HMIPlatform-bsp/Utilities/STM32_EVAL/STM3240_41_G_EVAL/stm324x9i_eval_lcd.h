/**
  ******************************************************************************
  * @file    stm324x9i_eval_lcd.h
  * @author  MCD Application Team
  * @version V0.1.0
  * @date    07-December-2012
  * @brief   This file contains all the functions prototypes for the stm324x9i_eval_lcd.c
  *          driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM324x9I_EVAL_LCD_H
#define __STM324x9I_EVAL_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm324x9i_eval.h"
#include "stm324x9i_eval_fmc_sdram.h"
#include "../Common/fonts.h"
   
/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM324x9I_EVAL
  * @{
  */
    
/** @addtogroup STM324x9I_EVAL_LCD
  * @{
  */ 


/** @defgroup STM324x9I_EVAL_LCD_Exported_Types
  * @{
  */
typedef struct 
{
  int16_t X;
  int16_t Y;
} Point, * pPoint;   
/**
  * @}
  */ 



/** @defgroup STM324x9I_EVAL_LCD_Exported_Constants
  * @{
  */ 
  
/**
 * @brief Uncomment one of the two following lines according to use LCD
 *        AMPIRE 640x480 or AMPIRE 480x272  
 */
/* #define USE_LCD_AM640480 */ 
#define USE_LCD_AM480272       /* Default configuration */

/* LCD Size (Width and Height) */
#ifdef USE_LCD_AM640480
  #define  LCD_PIXEL_WIDTH    ((uint16_t)640)
  #define  LCD_PIXEL_HEIGHT   ((uint16_t)480)
#endif  /* USE_LCD_AM640480 */

#ifdef USE_LCD_AM480272
  #define  LCD_PIXEL_WIDTH    ((uint16_t)480)
  #define  LCD_PIXEL_HEIGHT   ((uint16_t)272)
#endif  /* USE_LCD_AM480272 */




typedef struct
{
  vu16 HWord[LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT];//16-bit per pixel
} LCD_480272_TypeDef;


/** 
  * @brief  LCD color  
  */ 
#define LCD_COLOR_WHITE          0xFFFF
#define LCD_COLOR_BLACK          0x0000
#define LCD_COLOR_GREY           0xF7DE
#define LCD_COLOR_BLUE           0x001F
#define LCD_COLOR_BLUE2          0x051F
#define LCD_COLOR_RED            0xF800
#define LCD_COLOR_MAGENTA        0xF81F
#define LCD_COLOR_GREEN          0x07E0
#define LCD_COLOR_CYAN           0x7FFF
#define LCD_COLOR_YELLOW         0xFFE0

/** 
  * @brief  LCD Lines depending on the chosen fonts.  
  */
#define LCD_LINE_0               ((uint16_t)0)
#define LCD_LINE_1               ((uint16_t)1)
#define LCD_LINE_2               ((uint16_t)2)
#define LCD_LINE_3               ((uint16_t)3)
#define LCD_LINE_4               ((uint16_t)4)
#define LCD_LINE_5               ((uint16_t)5)
#define LCD_LINE_6               ((uint16_t)6)
#define LCD_LINE_7               ((uint16_t)7)
#define LCD_LINE_8               ((uint16_t)8)
#define LCD_LINE_9               ((uint16_t)9)
#define LCD_LINE_10              ((uint16_t)10)
#define LCD_LINE_11              ((uint16_t)11)
#define LCD_LINE_12              ((uint16_t)12)
#define LCD_LINE_13              ((uint16_t)13)
#define LCD_LINE_14              ((uint16_t)14)
#define LCD_LINE_15              ((uint16_t)15)
#define LCD_LINE_16              ((uint16_t)16)
#define LCD_LINE_17              ((uint16_t)17)
#define LCD_LINE_18              ((uint16_t)18)
#define LCD_LINE_19              ((uint16_t)19)
#define LCD_LINE_20              ((uint16_t)20)
#define LCD_LINE_21              ((uint16_t)21)
#define LCD_LINE_22              ((uint16_t)22)
#define LCD_LINE_23              ((uint16_t)23)
#define LCD_LINE_24              ((uint16_t)24)
#define LCD_LINE_25              ((uint16_t)25)
#define LCD_LINE_26              ((uint16_t)26)
#define LCD_LINE_27              ((uint16_t)27)
#define LCD_LINE_28              ((uint16_t)28)
#define LCD_LINE_29              ((uint16_t)29)

/** 
  * @brief LCD default font 
  */ 
#define LCD_DEFAULT_FONT         Font16x24

/** 
  * @brief  LCD Direction  
  */ 
#define LCD_DIR_HORIZONTAL       0x0000
#define LCD_DIR_VERTICAL         0x0001

/** 
  * @brief  LCD Layer  
  */ 
#define LCD_BACKGROUND_LAYER     0x0000
#define LCD_FOREGROUND_LAYER     0x0001

/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_LCD_Exported_Macros
  * @{
  */
#define ASSEMBLE_RGB(R ,G, B)    ((((R)& 0xF8) << 8) | (((G) & 0xFC) << 3) | (((B) & 0xF8) >> 3)) 
/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_LCD_Exported_Functions
  * @{
  */

void     LCD_DeInit(void);   
void     LCD_Init(void);
void     LCD_SetLayer(__IO uint32_t Layerx);
void     LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor); 
void     LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor);
void     LCD_SetTextColor(__IO uint16_t Color);
void     LCD_SetBackColor(__IO uint16_t Color);
void     LCD_SetTransparency(uint8_t transparency);
void     LCD_ClearLine(uint16_t Line);
void     LCD_Clear(void);
void 		 LCD_ClearOriginal(uint16_t Color);
uint32_t LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void     LCD_SetColorKeying(uint32_t RGBValue);
void     LCD_ReSetColorKeying(void);
void     LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c);
void     LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii);
void     LCD_SetFont(sFONT *fonts);
sFONT *  LCD_GetFont(void);
void     LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr);
void     LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width);
void     LCD_WindowModeDisable(void);
void     LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction);
void     LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width);
void     LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     LCD_DrawEllipse(int Xpos, int Ypos, int Radius, int Radius2);
void     LCD_DrawFullEllipse(int Xpos, int Ypos, int Radius, int Radius2);
void     LCD_DrawMonoPict(const uint32_t *Pict);
void     LCD_WriteBMP(uint32_t BmpAddress);
void     LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     LCD_PolyLine(pPoint Points, uint16_t PointCount);
void     LCD_PolyLineRelative(pPoint Points, uint16_t PointCount);
void     LCD_ClosedPolyLine(pPoint Points, uint16_t PointCount);
void     LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount);
void     LCD_FillPolyLine(pPoint Points, uint16_t PointCount);
void     LCD_Triangle(pPoint Points, uint16_t PointCount);
void     LCD_FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3);

void     LCD_DisplayOn(void);
void     LCD_DisplayOff(void);

/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __STM324x9I_EVAL_LCD_H */
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

/*********************** (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
