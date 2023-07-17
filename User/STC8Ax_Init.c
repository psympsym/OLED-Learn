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

/* ---------------------------------- ����ͷ�ļ� ---------------------------------- */
#include "STC8Ax_Init.h"

/* ---------------------------------- ˽�к궨�� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�����Ͷ��� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�б��� ---------------------------------- */
/* None. */

/* ---------------------------------- ��չ���� ---------------------------------- */
/* None. */

/* --------------------------------- ˽�к���ԭ�� --------------------------------- */
void GPIO_Config(uint8 GPIO, uint8 GPIOx_Pin, uint8 GPIOx_Mode);
// void SPI_Config;
void Time0_Config();

/* ----------------------------------- ������ ---------------------------------- */

/*��һ�� ��ʼ����Χ�豸 */
/**
 * @brief: None
 * @param: None
 * @return: None
 * @note: None
 */
void System_Init()
{
    // ��ʼ��P0��Ϊ�������
    GPIO_Config(GPIO_P0, GPIO_Pin_All, GPIO_OUT_PP);

    // ��ʼ��P3��Ϊ�������
    GPIO_Config(GPIO_P3, GPIO_Pin_All, GPIO_OUT_PP);

    // ��ʼ��P41��Ϊ�������
    GPIO_Config(GPIO_P4, GPIO_Pin_1, GPIO_OUT_PP);

    // OLED��ʼ��
    OLED_Init();
    
    // ����1��ʼ��
    UART1_Init();
    
    // ��ʱ��0��ʼ��
	Time0_Config();
}

/* �ڶ��� ��ʼ����Χ */
/**
 * ���ܣ���ʼ����Χ�豸
 * ��������
 * ���أ���
 * ˵����
 */
void Peripheral_Init()
{
	EA = 1;
}

/**
 * ���ܣ���ʼ��GPIO
 * ������GPIOx���˿�, GPIOx_Pin������, GPIOx_Mode��ģʽ
 * ���أ���
*/
void GPIO_Config(uint8 GPIO, uint8 GPIOx_Pin, uint8 GPIOx_Mode)
{
	GPIO_InitTypeDef GPIOx; // ����ṹ�����

	GPIOx.Mode = GPIOx_Mode;
	GPIOx.Pin = GPIOx_Pin;

	GPIO_Inilize(GPIO, &GPIOx);
}

/**
 * SSIG��SS ���Ź��ܿ���λ
 * SSIG=1������ SS ���Ź��ܣ�ʹ�� MSTR ȷ���������������Ǵӻ�
 * SSIG=0��SS ����ȷ���������������Ǵӻ�
 * 
 * SPEN��SPI ʹ�ܿ���λ
 * SPEN=1��ʹ�� SPI ����
 * SPEN=0���ر� SPI ����
 * 
 * FirstBit��SPI ����λ����/���յ�˳��
 * FirstBit=1�� �ȷ���/�������ݵĵ�λ��LSB��
 * FirstBit=0�� �ȷ���/�������ݵĸ�λ��MSB��
 * 
 * MSTR��������/��ģʽѡ��λ
 *  ��������ģʽ��
 *      �� SSIG��0���� SS �ܽű���Ϊ�ߵ�ƽ������ MSTR Ϊ 1
 *      �� SSIG��1����ֻ��Ҫ���� MSTR Ϊ 1������ SS �ܽŵĵ�ƽ��
 *  ���ôӻ�ģʽ��
 *      �� SSIG��0���� SS �ܽű���Ϊ�͵�ƽ���� MSTR λ�޹أ�
 *      �� SSIG��1����ֻ��Ҫ���� MSTR Ϊ 0������ SS �ܽŵĵ�ƽ��
 * 
 * CPOL��SPI ʱ�Ӽ��Կ���
 * CPOL=1��SCLK ����ʱΪ�ߵ�ƽ��SCLK ��ǰʱ����Ϊ�½��أ���ʱ����Ϊ������
 * CPOL=0��SCLK ����ʱΪ�͵�ƽ��SCLK ��ǰʱ����Ϊ�����أ���ʱ����Ϊ�½���
 * 
 * CPHA��SPIʱ����λѡ��
 * CPHA=1�� ������ SCLK ��ǰʱ������������ʱ���ز���
 * CPHA=0�� ���� SS �ܽ�Ϊ�͵�ƽ������һλ���ݲ��� SCLK �ĺ�ʱ���ظı����ݣ�ǰʱ���ز������ݣ����� SSIG��0��
void SPI_Config()
{
    SPI_InitTypeDef SPIx; // ����ṹ�����

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
 * ���ܣ���ʼ����ʱ��0
 * ��������
 * ���أ���
*/
void Time0_Config()
{
	TIM_InitTypeDef TIME; // ����ṹ�����

	TIME.TIM_Mode = TIM_16BitAutoReload; 	// 16λ�Զ�����
	TIME.TIM_ClkSource = TIM_CLOCK_12T;		// ʱ��Դ			TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIME.TIM_ClkOut = DISABLE;				// �ɱ��ʱ�����,	ENABLE,DISABLE
	TIME.TIM_Value = 0xF830;				// װ�س�ֵ
	TIME.TIM_Run = DISABLE;					// �Ƿ�����			ENABLE,DISABLE

	Timer_Inilize(Timer0, &TIME);
}
/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
