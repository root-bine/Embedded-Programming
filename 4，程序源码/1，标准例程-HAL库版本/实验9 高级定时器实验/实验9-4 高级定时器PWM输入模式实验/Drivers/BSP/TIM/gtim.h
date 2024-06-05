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


/*********************************������ͨ�ö�ʱ��PWM���ʵ����غ궨��*************************************/

/* TIMX PWM������� 
 * ���������PWM����LED0(RED)������ (��Ҫһ���Ű������� PA1 �� PA8 !!!)
 * Ĭ�������TIM2~TIM4
 * ע��: 
 * 1, ͨ���޸��⼸���궨��,����֧��TIM1~TIM8����һ����ʱ��,����һ��IO�����PWM
 * 1, TIM2_CH1ͨ��PA0���PWM, �������ǵ�LED0����PA8��, ������Ҫ�öŰ������� PA0 �� PA8, �Ӷ�����LED0����
 */
#define GTIM_TIMX_PWM_CHY_GPIO_PORT         GPIOA
#define GTIM_TIMX_PWM_CHY_GPIO_PIN          GPIO_PIN_1
#define GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) /* PA��ʱ��ʹ�� */

#define GTIM_TIMX_PWM                       TIM2 
#define GTIM_TIMX_PWM_CHY                   TIM_CHANNEL_2                               /* ͨ��Y,  1<= Y <=4 */
#define GTIM_TIMX_PWM_CHY_CCRX              GTIM_TIMX_PWM->CCR2                         /* ͨ��Y������ȽϼĴ��� */
#define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)  /* TIM2 ʱ��ʹ�� */


/******************************************************************************************/

extern TIM_HandleTypeDef g_timx_pwm_chy_handle;                                         /* ��ʱ��x��� */
void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc);                                /* ͨ�ö�ʱ�� PWM��ʼ������ */
void atim_timx_pwmin_chy_restart(void);                                                 /* �߼���ʱ�� ����PWM����ģʽ��� */

#endif

