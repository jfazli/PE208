
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COLOR_LCD_H
#define __COLOR_LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define UNKNOWN_LCD   0x00
#define RES480272_LCD 0x01
#define RES640480_LCD 0x02

/* LCD Registers */
#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R41            0x29
#define R43            0x2B
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R58            0x3A
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F

#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53

#define R96            0x60
#define R97            0x61
#define R106           0x6A

#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85

#define R144           0x90
#define R146           0x92
#define R147           0x93
#define R149           0x95
#define R151           0x97
#define R152           0x98

#define R229           0xE5

/* LCD Control pins */
#define CtrlPin_ONOFF    GPIO_Pin_6   /* PC.6 */

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Black          0x0000
#define Blue           0x001F
//#define Orange         0x051F
// Orange: Red=255, Green=153, Blue=0
#define Orange         0xFCC0
#define Red            0xF800
// Red=180, Green=0, Blue=0
#define DarkRed        0xE000
#define Magenta        0xF81F /* Magenta=Red|Blue */
#define Green          0x07E0
#define DarkGreen      0x0700
#define Cyan           0x7FFF
#define Yellow         0xFFE0 /* Yellow=Red|Green */

// color with alpha
#define ARed            0xFC00
#define AGreen          0x83E0
#define ABlue           0x801F
#define AYellow         0xFFE0

#define Line0          0
#define Line1          24
#define Line2          48
#define Line3          72
#define Line4          96
#define Line5          120
#define Line6          144
#define Line7          168
#define Line8          192
#define Line9          216
#define Line10         240

#define Horizontal     0x00
#define Vertical       0x01

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u32 StrLength(u8 *Str);

/*----- High layer function -----*/
void LCD_Init(void);
void LCD_SetTextColor(vu16 Color);
void LCD_SetBackColor(vu16 Color);
void LCD_ClearLine(u8 Line);
void LCD_Clear(void);
void LCD_ClearXor(u8 Xpos, u16 Ypos, u32 size);
void LCD_FillRect(u16 Xpos, u16 Ypos, u16 Width, u16 Height);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawChar(u16 Xpos, u16 Ypos, uc16 *c);
void LCD_DisplayChar(u16 Line, u16 Column, u8 Ascii);
void LCD_DisplayStringLine(u16 Line, u8 *ptr);
void LCD_DisplayStringLineAt(u16 Line, u16 Col, u8 *ptr);
void LCD_DisplayString(u16 Line, u8 *ptr);
void LCD_ScrollText(u16 Line, u8 *ptr);
void LCD_SetDisplayWindow(u16 Xpos, u16 Ypos, u8 Height, u16 Width);
void LCD_DrawLine(u16 Xpos, u16 Ypos, u16 Length, u8 Direction);
void LCD_DrawRect(u16 Xpos, u16 Ypos, u16 Width, u16 Height);
void LCD_DrawCircle(u16 Xpos, u16 Ypos, u16 Radius);
void LCD_DrawMonoPict(uc32 *Pict);
void LCD_FillRectL2(u16 Xpos, u16 Ypos, u16 Width, u16 Height);

void LCD_DisplayStringLineL2(u16 Line, u8 *ptr);

/*----- Medium layer function -----*/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_WriteRAMWord(u16 RGB_Code);
u16  LCD_ReadRAM(void);
void LCD_WriteRAM_PrepareF(void);
void LCD_PowerOn(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);

void LCD_WriteRAML2(u16 ARGB_Code);

/*----- Low layer function -----*/
void LCD_CtrlLinesConfig(void);
void LCD_PrllConfig(void);

#endif /* __COLOR_LCD_H */
