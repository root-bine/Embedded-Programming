/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       ͨ�ö�ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� M48Z-M3��Сϵͳ��STM32F103��
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"


/* ͨ�ö�ʱ������ */
#define GTIM_TIMX_CAP_CHY_GPIO_PORT            GPIOA
#define GTIM_TIMX_CAP_CHY_GPIO_PIN             GPIO_PIN_0
#define GTIM_TIMX_CAP_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define GTIM_TIMX_CAP                          TIM2                       
#define GTIM_TIMX_CAP_IRQn                     TIM2_IRQn
#define GTIM_TIMX_CAP_IRQHandler               TIM2_IRQHandler
#define GTIM_TIMX_CAP_CHY                      TIM_CHANNEL_1                                 /* ͨ��Y,  1<= Y <=4 */
#define GTIM_TIMX_CAP_CHY_CCRX                 TIM2->CCR1                                    /* ͨ��Y������ȽϼĴ��� */
#define GTIM_TIMX_CAP_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)    /* TIM2 ʱ��ʹ�� */

/* �������� */
void gtim_timx_cap_chy_init(uint16_t arr, uint16_t psc);                                    /* ͨ�ö�ʱ�� ���벶���ʼ������ */


#endif

