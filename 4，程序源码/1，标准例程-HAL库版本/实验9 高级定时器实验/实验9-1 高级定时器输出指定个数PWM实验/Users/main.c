/**
 ******************************************************************************
 * @file     main.c
 * @author   ����ԭ���Ŷ�(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    �߼���ʱ�����ָ������PWMʵ��
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
    printf("KEY0:Output PWM pulses\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

int main(void)
{
    uint8_t key = 0;
    uint8_t t = 0;
    
    HAL_Init();                                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);                         /* ����ʱ��, 72Mhz */
    delay_init(72);                                             /* ��ʱ��ʼ�� */
    usart_init(115200);                                         /* ��ʼ������ */
    led_init();                                                 /* ��ʼ��LED */
    key_init();                                                 /* ��ʼ������ */
	stm32f103c8t6_show_mesg();									/* ��ʾʵ����Ϣ */
    atim_timx_npwm_chy_init(5000 - 1, 7200 - 1);                /* 10Khz�ļ���Ƶ��,2hz��PWMƵ��. */

    ATIM_TIMX_NPWM_CHY_CCRX = 2500;                             /* ����PWMռ�ձ�,50%,�������Կ���ÿһ��PWM����,LED0(RED)
                                                                 * ��һ��ʱ��������,һ��ʱ�������,LED0����һ��,��ʾһ��PWM��
                                                                 */
    atim_timx_npwm_chy_set(5);                                  /* ���5��PWM��(����LED0(RED)��˸5��) */

    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)                                   /* KEY0���� */
        {
            atim_timx_npwm_chy_set(5);                          /* ���5��PWM��(����TIM1_CH1, ��PA8���5������) */
        }

        t++;
        delay_ms(10);

        if (t > 20)                                             /* ����LED1��˸, ��ʾ��������״̬ */
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}
