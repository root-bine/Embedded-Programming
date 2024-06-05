/**
 ****************************************************************************************************
 * @file        atim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       �߼���ʱ�� ��������
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

#ifndef __ATIM_H
#define __ATIM_H

#include "./SYSTEM/sys/sys.h"


/* �߼���ʱ������Ƚ�ģʽ���� */
#define ATIM_TIMX_PWMIN_CHY_GPIO_PORT           GPIOA
#define ATIM_TIMX_PWMIN_CHY_GPIO_PIN            GPIO_PIN_8
#define ATIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) /* PA��ʱ��ʹ�� */

#define ATIM_TIMX_PWMIN                         TIM1                       
#define ATIM_TIMX_PWMIN_IRQn                    TIM1_UP_IRQn
#define ATIM_TIMX_PWMIN_IRQHandler              TIM1_UP_IRQHandler
#define ATIM_TIMX_PWMIN_CHY                     TIM_CHANNEL_1
#define ATIM_TIMX_PWMIN_CHY_CLK_ENABLE()        do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)  /* TIM1 ʱ��ʹ�� */

 /* TIM1 / TIM8 �ж����Ĳ����жϷ�����,��Ҫ��������,����TIM2~5��,����Ҫ���¶��� */
#define ATIM_TIMX_PWMIN_CC_IRQn                 TIM1_CC_IRQn
#define ATIM_TIMX_PWMIN_CC_IRQHandler           TIM1_CC_IRQHandler

/* �������� */
void atim_timx_pwmin_chy_init(void);                                                        /* ��ʼ���߼���ʱ��PWM����ģʽ */

#endif

















