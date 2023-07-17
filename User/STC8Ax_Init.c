/*-------------------------------------------------------------------------------
/*       (C) Copyright 2023, PSYM. All Rights Reserved 
/*-------------------------------------------------------------------------------
/* File name   : STC8Ax_Init.c
/* Description : 
/* Author      : PSYM
/*-------------------------------------------------------------------------------
/* Update note:
/* -----------   -----------   --------------------------------------------------
/*    Date         Author                              Note
/* -----------   -----------   --------------------------------------------------
/* 2023-07-05       PSYM
/*
**/

/* ---------------------------------- 包含头文件 ---------------------------------- */
#include "STC8Ax_Init.h"

/* ---------------------------------- 私有宏定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有类型定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有变量 ---------------------------------- */
/* None. */

/* ---------------------------------- 扩展变量 ---------------------------------- */
/* None. */

/* --------------------------------- 私有函数原型 --------------------------------- */
void GPIO_Config(uint8 GPIO, uint8 GPIOx_Pin, uint8 GPIOx_Mode);
// void SPI_Config;
void Time0_Config();

/* ----------------------------------- 函数体 ---------------------------------- */

/*第一区 初始化外围设备 */
/**
 * @brief: None
 * @param: None
 * @return: None
 * @note: None
 */
void System_Init()
{
    // 初始化P0口为推挽输出
    GPIO_Config(GPIO_P0, GPIO_Pin_All, GPIO_OUT_PP);

    // 初始化P3口为推挽输出
    GPIO_Config(GPIO_P3, GPIO_Pin_All, GPIO_OUT_PP);

    // 初始化P41口为推挽输出
    GPIO_Config(GPIO_P4, GPIO_Pin_1, GPIO_OUT_PP);

    // OLED初始化
    OLED_Init();
    
    // 串口1初始化
    UART1_Init();
    
    // 定时器0初始化
	Time0_Config();
}

/* 第二区 初始化外围 */
/**
 * 功能：初始化外围设备
 * 参数：无
 * 返回：无
 * 说明：
 */
void Peripheral_Init()
{
	EA = 1;
}

/**
 * 功能：初始化GPIO
 * 参数：GPIOx：端口, GPIOx_Pin：引脚, GPIOx_Mode：模式
 * 返回：无
*/
void GPIO_Config(uint8 GPIO, uint8 GPIOx_Pin, uint8 GPIOx_Mode)
{
	GPIO_InitTypeDef GPIOx; // 定义结构体变量

	GPIOx.Mode = GPIOx_Mode;
	GPIOx.Pin = GPIOx_Pin;

	GPIO_Inilize(GPIO, &GPIOx);
}

/**
 * SSIG：SS 引脚功能控制位
 * SSIG=1，忽略 SS 引脚功能，使用 MSTR 确定器件是主机还是从机
 * SSIG=0，SS 引脚确定器件是主机还是从机
 * 
 * SPEN：SPI 使能控制位
 * SPEN=1，使能 SPI 功能
 * SPEN=0，关闭 SPI 功能
 * 
 * FirstBit：SPI 数据位发送/接收的顺序
 * FirstBit=1， 先发送/接收数据的低位（LSB）
 * FirstBit=0， 先发送/接收数据的高位（MSB）
 * 
 * MSTR：器件主/从模式选择位
 *  设置主机模式：
 *      若 SSIG＝0，则 SS 管脚必须为高电平且设置 MSTR 为 1
 *      若 SSIG＝1，则只需要设置 MSTR 为 1（忽略 SS 管脚的电平）
 *  设置从机模式：
 *      若 SSIG＝0，则 SS 管脚必须为低电平（与 MSTR 位无关）
 *      若 SSIG＝1，则只需要设置 MSTR 为 0（忽略 SS 管脚的电平）
 * 
 * CPOL：SPI 时钟极性控制
 * CPOL=1，SCLK 空闲时为高电平，SCLK 的前时钟沿为下降沿，后时钟沿为上升沿
 * CPOL=0，SCLK 空闲时为低电平，SCLK 的前时钟沿为上升沿，后时钟沿为下降沿
 * 
 * CPHA：SPI时钟相位选择。
 * CPHA=1， 数据在 SCLK 的前时钟沿驱动，后时钟沿采样
 * CPHA=0， 数据 SS 管脚为低电平驱动第一位数据并在 SCLK 的后时钟沿改变数据，前时钟沿采样数据（必须 SSIG＝0）
void SPI_Config()
{
    SPI_InitTypeDef SPIx; // 定义结构体变量

    SPIx.SPI_Enable = ENABLE;   // ENABLE,DISABLE
    SPIx.SPI_SSIG = ENABLE;     // ENABLE, DISABLE
    SPIx.SPI_FirstBit = SPI_MSB; // SPI_MSB, SPI_LSB
    SPIx.SPI_Mode = SPI_Mode_Slave;     // SPI_Mode_Master, SPI_Mode_Slave
    SPIx.SPI_CPOL = SPI_CPOL_High;     // SPI_CPOL_High,   SPI_CPOL_Low
    SPIx.SPI_CPHA = SPI_CPHA_2Edge;     // SPI_CPHA_1Edge,  SPI_CPHA_2Edge
    SPIx.SPI_Speed = ;    // SPI_Speed_4,SPI_Speed_16,SPI_Speed_64,SPI_Speed_128
}
*/
/**
 * 功能：初始化定时器0
 * 参数：无
 * 返回：无
*/
void Time0_Config()
{
	TIM_InitTypeDef TIME; // 定义结构体变量

	TIME.TIM_Mode = TIM_16BitAutoReload; 	// 16位自动重载
	TIME.TIM_ClkSource = TIM_CLOCK_12T;		// 时钟源			TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIME.TIM_ClkOut = DISABLE;				// 可编程时钟输出,	ENABLE,DISABLE
	TIME.TIM_Value = 0xF830;				// 装载初值
	TIME.TIM_Run = DISABLE;					// 是否运行			ENABLE,DISABLE

	Timer_Inilize(Timer0, &TIME);
}
/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
