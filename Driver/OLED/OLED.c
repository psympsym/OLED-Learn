/*-------------------------------------------------------------------------------
/*       (C) Copyright 2023, PSYM. All Rights Reserved
/*-------------------------------------------------------------------------------
/* File name   : OLED.c
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
#include "OLED.h"
#include "OLEDFont.h" // 字库

/* ---------------------------------- 私有宏定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有类型定义 ---------------------------------- */
uint8 xdata Cache[8][128]; // 缓冲区

/* ---------------------------------- 私有变量 ---------------------------------- */
/* None. */

/* ---------------------------------- 扩展变量 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有函数原型 ---------------------------------- */
void OLED_WRT_BYTE(uint8 dat, uint8 cmd);
void OLED_Set_Cursor(uint8 x, uint8 y);

/* ---------------------------------- 函数体 ---------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                第一区 OLED基础函数                          */
/* -------------------------------------------------------------------------- */
/**
 * @brief: OLED初始化
 * @param: None
 * @return: None
 * @note: None
 */
void OLED_Init()
{
    // OLED复位 /* 先拉低RST，再拉高RST，复位OLED */
    OLED_RES_SET(HIGHT);
    delay_ms(20);
    OLED_RES_SET(LOW);
    delay_ms(50);
    OLED_RES_SET(HIGHT);
    delay_ms(20);

    OLED_WRT_BYTE(0xAE, OLED_CMD); // 关闭OLED显示屏。
    OLED_WRT_BYTE(0x00, OLED_CMD); // 设置低列地址。
    OLED_WRT_BYTE(0x10, OLED_CMD); // 设置高列地址。
    OLED_WRT_BYTE(0x40, OLED_CMD); // 设置起始行地址。
    OLED_WRT_BYTE(0x81, OLED_CMD); // 设置对比度控制寄存器。
    OLED_WRT_BYTE(0xCF, OLED_CMD); // 设置SEG输出电流亮度。
    OLED_WRT_BYTE(0xC8, OLED_CMD); // 设置COM/行扫描方向。
    OLED_WRT_BYTE(0xA1, OLED_CMD); // 设置SEG/列映射。
    OLED_WRT_BYTE(0xA6, OLED_CMD); // 设置正常显示。
    OLED_WRT_BYTE(0xA8, OLED_CMD); // 设置多路复用比率。
    OLED_WRT_BYTE(0x3f, OLED_CMD); // 设置1/64的Duty。
    OLED_WRT_BYTE(0xD3, OLED_CMD); // 设置显示偏移。
    OLED_WRT_BYTE(0x00, OLED_CMD); // 不偏移。
    OLED_WRT_BYTE(0xd5, OLED_CMD); // 设置显示时钟分频比/振荡器频率。
    OLED_WRT_BYTE(0x80, OLED_CMD); // 设置分频比，将时钟设置为100帧/秒。
    OLED_WRT_BYTE(0xD9, OLED_CMD); // 设置预充电周期。
    OLED_WRT_BYTE(0xF1, OLED_CMD); // 设置预充电时钟数和放电时钟数。
    OLED_WRT_BYTE(0xDA, OLED_CMD); // 设置COM引脚硬件配置。
    OLED_WRT_BYTE(0x12, OLED_CMD); // 设置列地址高四位。
    OLED_WRT_BYTE(0xDB, OLED_CMD); // 设置VCOMH。
    OLED_WRT_BYTE(0x40, OLED_CMD); // 设置VCOM取消电平。
    OLED_WRT_BYTE(0x20, OLED_CMD); // 设置页寻址模式。
    OLED_WRT_BYTE(0x02, OLED_CMD); // 设置列地址低四位。
    OLED_WRT_BYTE(0x8D, OLED_CMD); // 设置充电泵启用/禁用。
    OLED_WRT_BYTE(0x14, OLED_CMD); // 设置充电泵禁用。
    OLED_WRT_BYTE(0xA4, OLED_CMD); // 关闭整个显示屏。
    OLED_WRT_BYTE(0xA6, OLED_CMD); // 关闭反显显示。
    OLED_WRT_BYTE(0xAF, OLED_CMD); // 打开OLED显示屏。

    OLED_Clear(0x00);      // 清除显示屏。
    OLED_Set_Cursor(0, 0); // 设置光标位置为(0,0)。
}

/**
 * @brief: OLED写1字节命令/数据
 * @param: cmd 命令
 * @return: None
 */
void OLED_WRT_BYTE(uint8 dat, uint8 cmd)
{
    uint8 i;

    // 设置命令
    OLED_DC_SET(cmd);
    // 设置片选
    OLED_CS_SET(LOW);

    // 按位发送命令
    /*当时钟引脚的电平为低电平时，数据引脚上的数据保持不变；当时钟引脚的电平为高电平时，数据引脚上的数据发生变化*/
    for (i = 0; i < 8; i++)
    {
        // 先将时钟引脚的电平拉低，表示数据传输开始
        OLED_CLK_SET(LOW);
        // 发送最高位数据
        OLED_MOSI_SET(dat & 0x80);
        // 将时钟引脚的电平拉高，表示数据传输完成
        OLED_CLK_SET(HIGHT);
        // 数据左移一位
        dat <<= 1;
    }

    // 拉高片选，数据传输结束
    OLED_CS_SET(HIGHT);
    // 拉高数据引脚，表示后续传输的数据是显示数据而非命令数据
    OLED_DC_SET(OLED_DAT);
}

/**
 * @brief 清空OLED显示屏
 * @param: mode 清空模式 0x00 全黑 0xFF 全白
 * @return: None
 */
void OLED_Clear(uint8 mode)
{
    uint8 i, j;

    for (i = 0; i < MAX_PAGE; i++)
    {
        OLED_Set_Cursor(0, i); // 设置光标位置

        // 循环写入空数据
        for (j = 0; j < MAX_COLUMN; j++)
        {
            OLED_WRT_BYTE(mode, OLED_DAT);
            Cache[i][j] = mode;
        }
    }
}

/**
 * @brief 设置光标位置
 * @param: x 横坐标 y 页坐标
 * @return: None
 */
void OLED_Set_Cursor(uint8 x, uint8 y)
{
    OLED_WRT_BYTE(0xb0 + y, OLED_CMD);
    OLED_WRT_BYTE(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WRT_BYTE((x & 0x0f) | 0x01, OLED_CMD);
}

/**
 * @brief: OLED显示单个汉字
 * @param: x 横坐标 y 页坐标 CCindex 汉字库中的位置
 */
void OLED_ShowCCE(uint8 x, uint8 y, uint8 CCindex)
{
    uint8 i;

    for (i = 0; i < 16; i++)
    {
        OLED_WRT_Cache(x + i, y, HZK[CCindex * 2][i]);
        OLED_WRT_Cache(x + i, y + 1, HZK[CCindex * 2 + 1][i]);
    }
}

/**
 * @bried: OLED显示字符串
 * @param: x 横坐标 y 页坐标 str 字符串
 * @return: None
 */
void OLED_ShowString(uint8 x, uint8 y, uint8 *str, uint8 width)
{
    while (*str != '\0')
    {
        OLED_ShowChar(x, y, *str, width);
        if ((x += 8) > 120)
        {
            x = 0;
            y += 2;
        }
        str++;
    }
}

/**
 * @brief: OLED显示字符
 * @param: x 横坐标 y 页坐标 chr 字符
 * @return: None
 */
void OLED_ShowChar(uint8 x, uint8 y, uint8 chr, uint8 width)
{
    uint8 i = 0;
    // 超出范围返回空
    if (x > MAX_COLUMN || x < 0)
        return;
    switch (width)
    {
    case 6:
        for (i = 0; i < width; i++)
        {
            OLED_WRT_Cache(x + i, y, ASCII68[chr - 32][i]);
        }
        break;

    case 8:
        for (i = 0; i < width; i++)
        {
            OLED_WRT_Cache(x + i, y, ASCII816[chr - 32][i]);
            OLED_WRT_Cache(x + i, y + 1, ASCII816[chr - 32][i + 8]);
        }
        break;
    }
}

/**
 * @brief: OLED显示1像素点
 * @param: x 横坐标 y 纵坐标
 * @return: None
 */
void OLED_ShowPoint(uint8 x, uint8 y)
{
    // 转为第几页第几行
    uint8 page = y >> 3;
    uint8 row = y % 8;

    OLED_WRT_Cache(x, page, Cache[page][x] | 0x01 << row);
}

/**
 * @brief: 开启OLED显示
 * @param: None
 * @return: None
 */
void OLED_ON()
{
    OLED_WRT_BYTE(0X8D, OLED_CMD); // SET DCDC命令
    OLED_WRT_BYTE(0X14, OLED_CMD); // DCDC ON
    OLED_WRT_BYTE(0XAF, OLED_CMD); // DISPLAY ON
}

/**
 * @brief: 关闭OLED显示
 * @param: None
 * @return: None
 */
void OLED_OFF()
{
    OLED_WRT_BYTE(0X8D, OLED_CMD); // SET DCDC命令
    OLED_WRT_BYTE(0X10, OLED_CMD); // DCDC OFF
    OLED_WRT_BYTE(0XAE, OLED_CMD); // DISPLAY OFF
}

/* -------------------------------------------------------------------------- */
/*                                第二区 OLED缓存函数                          */
/* -------------------------------------------------------------------------- */
// /**
//  * @brief: 将数据写入缓存区
//  * @param: x 列地址 y 页地址 dat 数据
//  * @return: None
//  */
// void OLED_WRT_Cache(uint8 x, uint8 y, uint8 dat)
// {
//     EAXSFR();
//     Cache[y][x] = dat;
//     EAXRAM();
// }

/**
 * @brief: 将数据写入缓存区
 * @param: x 列地址 y 行地址 dat 数据
 * @return: None
 */
void OLED_WRT_Cache(uint8 x, uint8 y, uint8 dat)
{
    uint8 page = y >> 3;
    uint8 row = y % 8;

    EAXSFR();
    Cache[page][x] |= (dat << row) & 0xFF;
    Cache[page + 1][x] |= (dat >> (8 - row)) & 0xFF;
    EAXRAM();
}

/**
 * @brief: 推送缓存区数据到OLED
 * @param: None
 * @return: None
 */
void OLED_Push_Cache()
{
    uint8 x, y;

    EAXSFR();

    for (y = 0; y < MAX_PAGE; y++)
    {
        OLED_Set_Cursor(0, y); // 设置光标位置

        // 将缓存区数据写入OLED
        for (x = 0; x < MAX_COLUMN; x++)
            OLED_WRT_BYTE(Cache[y][x], OLED_DAT);
    }

    EAXRAM();
}

/* -------------------------------------------------------------------------- */
/*                                第三区 OLED扩展函数                          */
/* -------------------------------------------------------------------------- */
/**
 * @brief: 绘制无填充圆形轮廓
 * @param: x 中心点横坐标 y 中心点纵坐标 r 圆半径
 * @return: None
 */
void OLED_Draw_Circle(uint8 x, uint8 y, uint8 r)
{
    uint16 i;
    uint8 cx, cy;

    for (i = 0; i <= 360; i++)
    {
        cx = x + r * cos(i * PI / 180);
        cy = y + r * sin(i * PI / 180);
        OLED_ShowPoint(cx, cy);
    }
}

/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
