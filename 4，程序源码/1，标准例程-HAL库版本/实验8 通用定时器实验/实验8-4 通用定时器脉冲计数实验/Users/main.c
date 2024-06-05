/**
 ******************************************************************************
 * @file     main.c
 * @author   ����ԭ���Ŷ�(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    ͨ�ö�ʱ���������ʵ��
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
#include "./BSP/KEY/key.h"


extern uint8_t  g_timxchy_cap_sta;  /* ���벶��״̬ */
extern uint16_t g_timxchy_cap_val;  /* ���벶��ֵ */

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
    printf("GTIM Pulse Count Test\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("KEY0:Restart Universal Timer Count\r\n");
    printf("KEY_UP:Input pulse\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

int main(void)
{
    uint32_t curcnt = 0;
    uint32_t oldcnt = 0;
    uint8_t key = 0;
    uint8_t t = 0;

    HAL_Init();                                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);                         /* ����ʱ��, 72Mhz */
    delay_init(72);                                             /* ��ʱ��ʼ�� */
    usart_init(115200);                                         /* ��ʼ������ */
    led_init();                                                 /* ��ʼ��LED */
    key_init();                                                 /* ��ʼ������ */
    stm32f103c8t6_show_mesg();                                  /* ʵ����ʾ��Ϣ */
    gtim_timx_cnt_chy_init(0);                                  /* ��ʱ��������ʼ��, ����Ƶ */
    gtim_timx_cnt_chy_restart();                                /* �������� */

    while (1)
    {
        key = key_scan(0);                                      /* ɨ�谴�� */

        if (key == KEY0_PRES)                                   /* KEY0��������,�������� */
        {
            gtim_timx_cnt_chy_restart();                        /* ������������ */
        }

        curcnt = gtim_timx_cnt_chy_get_count();                 /* ��ȡ����ֵ */

        if (oldcnt != curcnt)
        {
            oldcnt = curcnt;
            printf("CNT:%d\r\n", oldcnt);                       /* ��ӡ������� */
        }

        t++;

        if (t > 20)                                             /* 200ms����һ�� */
        {
            t = 0;
            LED0_TOGGLE();                                      /* LED0��˸ ,��ʾ�������� */
        }

        delay_ms(10);
    }
}
