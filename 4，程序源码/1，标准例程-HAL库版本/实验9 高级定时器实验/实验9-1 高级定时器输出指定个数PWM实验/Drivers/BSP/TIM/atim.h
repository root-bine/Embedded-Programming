/**
 ****************************************************************************************************
 * @file        atim.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       高级定时器 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 M48Z-M3最小系统板STM32F103版
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __ATIM_H
#define __ATIM_H

#include "./SYSTEM/sys/sys.h"


/* 高级定时器输出指定个数PWM定义 */
#define ATIM_TIMX_NPWM_CHY_GPIO_PORT            GPIOA
#define ATIM_TIMX_NPWM_CHY_GPIO_PIN             GPIO_PIN_11
#define ATIM_TIMX_NPWM_CHY_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define ATIM_TIMX_NPWM                          TIM1
#define ATIM_TIMX_NPWM_IRQn                     TIM1_UP_IRQn
#define ATIM_TIMX_NPWM_IRQHandler               TIM1_UP_IRQHandler
#define ATIM_TIMX_NPWM_CHY                      TIM_CHANNEL_4                               /* 通道Y,  1<= Y <=4 */
#define ATIM_TIMX_NPWM_CHY_CCRX                 ATIM_TIMX_NPWM->CCR4                        /* 通道Y的输出比较寄存器 */
#define ATIM_TIMX_NPWM_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)  /* TIM1 时钟使能 */

/* 函数声明 */
void atim_timx_npwm_chy_init(uint16_t arr, uint16_t psc);   /* 高级定时器 输出指定个数PWM初始化函数 */
void atim_timx_npwm_chy_set(uint32_t npwm);                 /* 高级定时器 设置输出PWM的个数 */

#endif

















