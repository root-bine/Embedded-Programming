/**
 ******************************************************************************
 * @file     main.c
 * @author   ����ԭ���Ŷ�(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    ͨ�ö�ʱ��PWM���ʵ��
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
#include "./BSP/TIM/gtim.h"


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
    printf("GTIM PWM Test\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}


int main(void)
{
    uint32_t ledpwmval = 0;
    uint8_t dir = 1;
    
    HAL_Init();                                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);                         /* ����ʱ��, 72Mhz */
    delay_init(72);                                             /* ��ʱ��ʼ�� */
    usart_init(115200);                                         /* ��ʼ������ */
    gtim_timx_pwm_chy_init(500 - 1, 72 - 1);                    /* 1Mhz�ļ���Ƶ��,2Khz��PWM. */
    stm32f103c8t6_show_mesg();                                  /* ʵ����ʾ��Ϣ */

    while (1)
    {
       delay_ms(10);
        
        /* ���ݷ����޸�ledpwmval */
        if (dir == 1)
        {
            ledpwmval++;
        }
        else
        {
            ledpwmval--;
        }
        
        /* ��ledpwmval����300ʱ�������Ϊ�ݼ� */
        if (ledpwmval > 300)
        {
            dir = 0;
        }
        
        /* ��ledpwmval����0ʱ�������Ϊ���� */
        if (ledpwmval == 0)
        {
            dir = 1;
        }

        /* �޸ıȽ�ֵ����ռ�ձ� */
        __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, ledpwmval);
    }
}
