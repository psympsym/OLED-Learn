/* -----------------------------------------------------------------------------
/* File name   : OLED.h
/* Author      : PSYM
/* Copyright(C): 2023 PSYM. All rights reserved.
/* -----------------------------------------------------------------------------
 */
#ifndef __OLED_H_
#define __OLED_H_

/* --------------------------------- 包含头文件 --------------------------------- */
#include "STC8A_Delay.h"
#include "uart_printf.h"
#include <math.h>

/* --------------------------------- 类型定义 --------------------------------- */
sbit OLED_CLK  = P3 ^ 2;    // 时钟引脚
sbit OLED_MOSI = P3 ^ 4;   // D1数据
sbit OLED_CS   = P3 ^ 5;     // 片选
sbit OLED_RES  = P4 ^ 1;    // 复位
sbit OLED_DC   = P3 ^ 3;     // 命令/数据控制
 
/* --------------------------------- 宏定义 --------------------------------- */
#define MAX_COLUMN 128
#define MAX_PAGE 8

#define OLED_DAT 1
#define OLED_CMD 0
#define HIGHT 1
#define LOW 0

#define OLED_CLK_SET(n)  (OLED_CLK = n)  // 设置时钟
#define OLED_MOSI_SET(n) (OLED_MOSI = n) // 设置数据
#define OLED_CS_SET(n)   (OLED_CS = n)   // 片选
#define OLED_RES_SET(n)  (OLED_RES = n)  // 复位
#define OLED_DC_SET(n)   (OLED_DC = n)   // 0 命令 1 数据

#define PI 3.141592
/* --------------------------------- 扩展变量 --------------------------------- */
/* None. */

/* --------------------------------- 函数声明 --------------------------------- */
void OLED_Init();
void OLED_Clear(uint8 mode);
void OLED_ShowCCE(uint8 x, uint8 y, uint8 CCindex);
void OLED_ShowString(uint8 x, uint8 y, uint8 *str, uint8 width);
void OLED_ShowChar(uint8 x, uint8 y, uint8 chr, uint8 width);
void OLED_ShowPoint(uint8 x, uint8 y);
void OLED_ON();
void OLED_OFF();
void OLED_WRT_Cache(uint8 x, uint8 y, uint8 dat);
void OLED_Push_Cache();
void OLED_Draw_Circle(uint8 x, uint8 y, uint8 r);

#endif /* __OLED_H_ */

    /*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
