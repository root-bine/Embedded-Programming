/**
 ******************************************************************************
 * @file     main.c
 * @author   ����ԭ���Ŷ�(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    �������Ź�ʵ��
 * @license  Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ******************************************************************************
 * @attention
 * 
 * ʵ��ƽ̨:����ԭ�� M48Z-M3��Сϵͳ��STM32F103��
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 ******************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/IWDG/iwdg.h"


/**
 * @brief       ��ʾʵ����Ϣ
 * @param       ��
 * @retval      ��
 */
void stm32f103c8t6_show_mesg(void)
{
    /* �������ʵ����Ϣ */
    printf("\n");
    printf("********************************\r\n");
    printf("STM32\r\n");
    printf("IWDG Test\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("KEY_UP:Feed a dog\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

int main(void)
{
    HAL_Init();                                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);                         /* ����ʱ��, 72Mhz */
    delay_init(72);                                             /* ��ʱ��ʼ�� */
    usart_init(115200);                                         /* ��ʼ������ */
    led_init();                                                 /* ��ʼ��LED */
    key_init();                                                 /* ��ʼ������ */
    delay_ms(100);                                              /* ��ʱ100ms */
    iwdg_init(IWDG_PRESCALER_64, 625);                          /* Ԥ��Ƶ��Ϊ64,����ֵΪ625,���ʱ��ԼΪ1s */
    LED0(0);                                                    /* �ȵ���LED0 */
    stm32f103c8t6_show_mesg();                                  /* ʵ����Ϣ */
    
    while (1)
    {
        if (key_scan(0) == WKUP_PRES)                           /* ���WK_UP���£���ι�� */
        {
            iwdg_feed();                                        /* ι�� */
        }
        
        delay_ms(10);
    }
}
