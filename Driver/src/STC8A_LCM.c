/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#include "STC8A_LCM.h"

//========================================================================
// 函数: void LCM_Inilize(LCM_InitTypeDef *LCM)
// 描述: LCM 初始化程序.
// 参数: LCM: 结构参数,请参考LCM.h里的定义.
// 返回: none.
// 版本: V1.0, 2021-06-02
//========================================================================
void LCM_Inilize(LCM_InitTypeDef *LCM)
{
	EAXSFR();		/* MOVX A,@DPTR/MOVX @DPTR,A指令的操作对象为扩展SFR(XSFR) */

	LCMIFSTA = 0x00;
	if(LCM->LCM_Mode == MODE_M6800)		LCMIFCFG |= MODE_M6800;	//LCM接口模式：M6800
	else LCMIFCFG &= ~MODE_M6800;	//LCM接口模式：I8080
	
	if(LCM->LCM_Bit_Wide == BIT_WIDE_16)		LCMIFCFG |= BIT_WIDE_16;	//LCM数据宽度：16位
	else LCMIFCFG &= ~BIT_WIDE_16;	//LCM数据宽度：8位
	
	if(LCM->LCM_Setup_Time <= 7) LCMIFCFG2 = (LCMIFCFG2 & ~0x1c) | (LCM->LCM_Setup_Time << 2);	//LCM通信数据建立时间：0~7
	if(LCM->LCM_Hold_Time <= 3) LCMIFCFG2 = (LCMIFCFG2 & ~0x03) | LCM->LCM_Hold_Time;	//LCM通信数据建立时间：0~7
	
	if(LCM->LCM_Enable == ENABLE)		LCMIFCR |= 0x80;	//使能LCM接口功能
	else LCMIFCR &= ~0x80;	//禁止LCM接口功能

	EAXRAM();		/* MOVX A,@DPTR/MOVX @DPTR,A指令的操作对象为扩展RAM(XRAM) */
}
