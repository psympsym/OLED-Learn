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

/* ---------------------------------- ����ͷ�ļ� ---------------------------------- */
#include "OLED.h"
#include "OLEDFont.h" // �ֿ�

/* ---------------------------------- ˽�к궨�� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�����Ͷ��� ---------------------------------- */
uint8 xdata Cache[8][128]; // ������

/* ---------------------------------- ˽�б��� ---------------------------------- */
/* None. */

/* ---------------------------------- ��չ���� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�к���ԭ�� ---------------------------------- */
void OLED_WRT_BYTE(uint8 dat, uint8 cmd);
void OLED_Set_Cursor(uint8 x, uint8 y);

/* ---------------------------------- ������ ---------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                ��һ�� OLED��������                          */
/* -------------------------------------------------------------------------- */
/**
 * @brief: OLED��ʼ��
 * @param: None
 * @return: None
 * @note: None
 */
void OLED_Init()
{
    // OLED��λ /* ������RST��������RST����λOLED */
    OLED_RES_SET(HIGHT);
    delay_ms(20);
    OLED_RES_SET(LOW);
    delay_ms(50);
    OLED_RES_SET(HIGHT);
    delay_ms(20);

    OLED_WRT_BYTE(0xAE, OLED_CMD); // �ر�OLED��ʾ����
    OLED_WRT_BYTE(0x00, OLED_CMD); // ���õ��е�ַ��
    OLED_WRT_BYTE(0x10, OLED_CMD); // ���ø��е�ַ��
    OLED_WRT_BYTE(0x40, OLED_CMD); // ������ʼ�е�ַ��
    OLED_WRT_BYTE(0x81, OLED_CMD); // ���öԱȶȿ��ƼĴ�����
    OLED_WRT_BYTE(0xCF, OLED_CMD); // ����SEG����������ȡ�
    OLED_WRT_BYTE(0xC8, OLED_CMD); // ����COM/��ɨ�跽��
    OLED_WRT_BYTE(0xA1, OLED_CMD); // ����SEG/��ӳ�䡣
    OLED_WRT_BYTE(0xA6, OLED_CMD); // ����������ʾ��
    OLED_WRT_BYTE(0xA8, OLED_CMD); // ���ö�·���ñ��ʡ�
    OLED_WRT_BYTE(0x3f, OLED_CMD); // ����1/64��Duty��
    OLED_WRT_BYTE(0xD3, OLED_CMD); // ������ʾƫ�ơ�
    OLED_WRT_BYTE(0x00, OLED_CMD); // ��ƫ�ơ�
    OLED_WRT_BYTE(0xd5, OLED_CMD); // ������ʾʱ�ӷ�Ƶ��/����Ƶ�ʡ�
    OLED_WRT_BYTE(0x80, OLED_CMD); // ���÷�Ƶ�ȣ���ʱ������Ϊ100֡/�롣
    OLED_WRT_BYTE(0xD9, OLED_CMD); // ����Ԥ������ڡ�
    OLED_WRT_BYTE(0xF1, OLED_CMD); // ����Ԥ���ʱ�����ͷŵ�ʱ������
    OLED_WRT_BYTE(0xDA, OLED_CMD); // ����COM����Ӳ�����á�
    OLED_WRT_BYTE(0x12, OLED_CMD); // �����е�ַ����λ��
    OLED_WRT_BYTE(0xDB, OLED_CMD); // ����VCOMH��
    OLED_WRT_BYTE(0x40, OLED_CMD); // ����VCOMȡ����ƽ��
    OLED_WRT_BYTE(0x20, OLED_CMD); // ����ҳѰַģʽ��
    OLED_WRT_BYTE(0x02, OLED_CMD); // �����е�ַ����λ��
    OLED_WRT_BYTE(0x8D, OLED_CMD); // ���ó�������/���á�
    OLED_WRT_BYTE(0x14, OLED_CMD); // ���ó��ý��á�
    OLED_WRT_BYTE(0xA4, OLED_CMD); // �ر�������ʾ����
    OLED_WRT_BYTE(0xA6, OLED_CMD); // �رշ�����ʾ��
    OLED_WRT_BYTE(0xAF, OLED_CMD); // ��OLED��ʾ����

    OLED_Clear(0x00);      // �����ʾ����
    OLED_Set_Cursor(0, 0); // ���ù��λ��Ϊ(0,0)��
}

/**
 * @brief: OLEDд1�ֽ�����/����
 * @param: cmd ����
 * @return: None
 */
void OLED_WRT_BYTE(uint8 dat, uint8 cmd)
{
    uint8 i;

    // ��������
    OLED_DC_SET(cmd);
    // ����Ƭѡ
    OLED_CS_SET(LOW);

    // ��λ��������
    /*��ʱ�����ŵĵ�ƽΪ�͵�ƽʱ�����������ϵ����ݱ��ֲ��䣻��ʱ�����ŵĵ�ƽΪ�ߵ�ƽʱ�����������ϵ����ݷ����仯*/
    for (i = 0; i < 8; i++)
    {
        // �Ƚ�ʱ�����ŵĵ�ƽ���ͣ���ʾ���ݴ��俪ʼ
        OLED_CLK_SET(LOW);
        // �������λ����
        OLED_MOSI_SET(dat & 0x80);
        // ��ʱ�����ŵĵ�ƽ���ߣ���ʾ���ݴ������
        OLED_CLK_SET(HIGHT);
        // ��������һλ
        dat <<= 1;
    }

    // ����Ƭѡ�����ݴ������
    OLED_CS_SET(HIGHT);
    // �����������ţ���ʾ�����������������ʾ���ݶ�����������
    OLED_DC_SET(OLED_DAT);
}

/**
 * @brief ���OLED��ʾ��
 * @param: mode ���ģʽ 0x00 ȫ�� 0xFF ȫ��
 * @return: None
 */
void OLED_Clear(uint8 mode)
{
    uint8 i, j;

    for (i = 0; i < MAX_PAGE; i++)
    {
        OLED_Set_Cursor(0, i); // ���ù��λ��

        // ѭ��д�������
        for (j = 0; j < MAX_COLUMN; j++)
        {
            OLED_WRT_BYTE(mode, OLED_DAT);
            Cache[i][j] = mode;
        }
    }
}

/**
 * @brief ���ù��λ��
 * @param: x ������ y ҳ����
 * @return: None
 */
void OLED_Set_Cursor(uint8 x, uint8 y)
{
    OLED_WRT_BYTE(0xb0 + y, OLED_CMD);
    OLED_WRT_BYTE(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WRT_BYTE((x & 0x0f) | 0x01, OLED_CMD);
}

/**
 * @brief: OLED��ʾ��������
 * @param: x ������ y ҳ���� CCindex ���ֿ��е�λ��
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
 * @bried: OLED��ʾ�ַ���
 * @param: x ������ y ҳ���� str �ַ���
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
 * @brief: OLED��ʾ�ַ�
 * @param: x ������ y ҳ���� chr �ַ�
 * @return: None
 */
void OLED_ShowChar(uint8 x, uint8 y, uint8 chr, uint8 width)
{
    uint8 i = 0;
    // ������Χ���ؿ�
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
 * @brief: OLED��ʾ1���ص�
 * @param: x ������ y ������
 * @return: None
 */
void OLED_ShowPoint(uint8 x, uint8 y)
{
    // תΪ�ڼ�ҳ�ڼ���
    uint8 page = y >> 3;
    uint8 row = y % 8;

    OLED_WRT_Cache(x, page, Cache[page][x] | 0x01 << row);
}

/**
 * @brief: ����OLED��ʾ
 * @param: None
 * @return: None
 */
void OLED_ON()
{
    OLED_WRT_BYTE(0X8D, OLED_CMD); // SET DCDC����
    OLED_WRT_BYTE(0X14, OLED_CMD); // DCDC ON
    OLED_WRT_BYTE(0XAF, OLED_CMD); // DISPLAY ON
}

/**
 * @brief: �ر�OLED��ʾ
 * @param: None
 * @return: None
 */
void OLED_OFF()
{
    OLED_WRT_BYTE(0X8D, OLED_CMD); // SET DCDC����
    OLED_WRT_BYTE(0X10, OLED_CMD); // DCDC OFF
    OLED_WRT_BYTE(0XAE, OLED_CMD); // DISPLAY OFF
}

/* -------------------------------------------------------------------------- */
/*                                �ڶ��� OLED���溯��                          */
/* -------------------------------------------------------------------------- */
// /**
//  * @brief: ������д�뻺����
//  * @param: x �е�ַ y ҳ��ַ dat ����
//  * @return: None
//  */
// void OLED_WRT_Cache(uint8 x, uint8 y, uint8 dat)
// {
//     EAXSFR();
//     Cache[y][x] = dat;
//     EAXRAM();
// }

/**
 * @brief: ������д�뻺����
 * @param: x �е�ַ y �е�ַ dat ����
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
 * @brief: ���ͻ��������ݵ�OLED
 * @param: None
 * @return: None
 */
void OLED_Push_Cache()
{
    uint8 x, y;

    EAXSFR();

    for (y = 0; y < MAX_PAGE; y++)
    {
        OLED_Set_Cursor(0, y); // ���ù��λ��

        // ������������д��OLED
        for (x = 0; x < MAX_COLUMN; x++)
            OLED_WRT_BYTE(Cache[y][x], OLED_DAT);
    }

    EAXRAM();
}

/* -------------------------------------------------------------------------- */
/*                                ������ OLED��չ����                          */
/* -------------------------------------------------------------------------- */
/**
 * @brief: ���������Բ������
 * @param: x ���ĵ������ y ���ĵ������� r Բ�뾶
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
