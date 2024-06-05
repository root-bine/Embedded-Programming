/**
 ******************************************************************************
 * @file     main.c
 * @author   ����ԭ���Ŷ�(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    ���ڿ��Ź�ʵ��
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
#include "./BSP/WWDG/wwdg.h"


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
    printf("WWDG Test\r\n");
    printf("ATOM@ALIENTEK\r\n");
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
    LED0(0);                                                    /* LED0���� */
    delay_ms(300);
    wwdg_init(0X7F, 0X5F, WWDG_PRESCALER_8);                    /* ������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8 */
    stm32f103c8t6_show_mesg();                                  /* ʵ����ʾ��Ϣ */
    
    while (1)
    {
        LED0(1);                                                /* LED0Ϩ�� */
    }
}
