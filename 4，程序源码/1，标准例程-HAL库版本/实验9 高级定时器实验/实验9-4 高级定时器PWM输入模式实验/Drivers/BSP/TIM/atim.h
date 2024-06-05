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


/* 高级定时器输出比较模式定义 */
#define ATIM_TIMX_PWMIN_CHY_GPIO_PORT           GPIOA
#define ATIM_TIMX_PWMIN_CHY_GPIO_PIN            GPIO_PIN_8
#define ATIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) /* PA口时钟使能 */

#define ATIM_TIMX_PWMIN                         TIM1                       
#define ATIM_TIMX_PWMIN_IRQn                    TIM1_UP_IRQn
#define ATIM_TIMX_PWMIN_IRQHandler              TIM1_UP_IRQHandler
#define ATIM_TIMX_PWMIN_CHY                     TIM_CHANNEL_1
#define ATIM_TIMX_PWMIN_CHY_CLK_ENABLE()        do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)  /* TIM1 时钟使能 */

 /* TIM1 / TIM8 有独立的捕获中断服务函数,需要单独定义,对于TIM2~5等,则不需要以下定义 */
#define ATIM_TIMX_PWMIN_CC_IRQn                 TIM1_CC_IRQn
#define ATIM_TIMX_PWMIN_CC_IRQHandler           TIM1_CC_IRQHandler

/* 函数声明 */
void atim_timx_pwmin_chy_init(void);                                                        /* 初始化高级定时器PWM输入模式 */

#endif

















