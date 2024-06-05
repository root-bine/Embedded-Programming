/**
 ******************************************************************************
 * @file     main.c
 * @author   ����ԭ���Ŷ�(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    �߼���ʱ������Ƚ�ģʽʵ��
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
#include "./BSP/TIM/atim.h"


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
    printf("ATIM PWM Count Test\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

int main(void)
{
    HAL_Init();                                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);                         /* ����ʱ��, 72Mhz */
    delay_init(72);                                             /* ��ʱ��ʼ�� */
//    usart_init(115200);                                         /* ��ʼ������ */
    led_init();                                                 /* ��ʼ��LED */
    key_init();                                                 /* ��ʼ������ */
//    stm32f103c8t6_show_mesg();                                  /* ��ʾʵ����Ϣ */
    delay_ms(100);
    atim_timx_comp_pwm_init(1000 - 1, 72 - 1);                  /* 1Mhz�ļ���Ƶ�� 1Khz������ */
    ATIM_TIMX_COMP_CH1_CCRX = 250 - 1;                          /* ͨ��1 ��λ25% */
    ATIM_TIMX_COMP_CH2_CCRX = 500 - 1;                          /* ͨ��2 ��λ50% */
    ATIM_TIMX_COMP_CH3_CCRX = 750 - 1;                          /* ͨ��3 ��λ75% */
    ATIM_TIMX_COMP_CH4_CCRX = 1000 - 1;                         /* ͨ��4 ��λ100% */

    while (1)
    {
        delay_ms(10);
    }
}
