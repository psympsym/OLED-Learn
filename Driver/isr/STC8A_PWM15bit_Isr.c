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

#include	"STC8A_PWM15bit.h"

//========================================================================
// 函数: PWM_ISR_Handler
// 描述: 15位增强型PWM中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
void	PWM_ISR_Handler (void) interrupt PWM_VECTOR
{
	char store;
	store = P_SW2;
	P_SW2 |= 0x80;

	if(PWMCFG & 0x08)		//PWM计数器中断
	{
		PWMCFG &= ~0x08;		//清PWM计数器中断标志
		// TODO: 在此处添加用户代码
	}
	if(PWMIF)		//PWM0匹配中断
	{
		PWMIF = 0;
		// TODO: 在此处添加用户代码
	}
	P_SW2 = store;
}
