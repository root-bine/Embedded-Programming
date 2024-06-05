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
#define ATIM_TIMX_COMP_CH1_GPIO_PORT            GPIOA
#define ATIM_TIMX_COMP_CH1_GPIO_PIN             GPIO_PIN_8
#define ATIM_TIMX_COMP_CH1_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA口时钟使能 */

#define ATIM_TIMX_COMP_CH2_GPIO_PORT            GPIOA
#define ATIM_TIMX_COMP_CH2_GPIO_PIN             GPIO_PIN_9
#define ATIM_TIMX_COMP_CH2_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA口时钟使能 */

#define ATIM_TIMX_COMP_CH3_GPIO_PORT            GPIOA
#define ATIM_TIMX_COMP_CH3_GPIO_PIN             GPIO_PIN_10
#define ATIM_TIMX_COMP_CH3_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA口时钟使能 */

#define ATIM_TIMX_COMP_CH4_GPIO_PORT            GPIOA
#define ATIM_TIMX_COMP_CH4_GPIO_PIN             GPIO_PIN_11
#define ATIM_TIMX_COMP_CH4_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA口时钟使能 */

#define ATIM_TIMX_COMP                          TIM1            
#define ATIM_TIMX_COMP_CH1_CCRX                 ATIM_TIMX_COMP->CCR1                            /* 通道1的输出比较寄存器 */
#define ATIM_TIMX_COMP_CH2_CCRX                 ATIM_TIMX_COMP->CCR2                            /* 通道2的输出比较寄存器 */
#define ATIM_TIMX_COMP_CH3_CCRX                 ATIM_TIMX_COMP->CCR3                            /* 通道3的输出比较寄存器 */
#define ATIM_TIMX_COMP_CH4_CCRX                 ATIM_TIMX_COMP->CCR4                            /* 通道4的输出比较寄存器 */
#define ATIM_TIMX_COMP_CLK_ENABLE()             do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)      /* TIM1 时钟使能 */

/* 函数声明 */
void atim_timx_pwm_chy_init(uint16_t arr, uint16_t psc);                                        /* 高级定时器 PWM初始化函数 */
void atim_timx_npwm_chy_init(uint16_t arr, uint16_t psc);                                       /* 高级定时器 输出指定个数PWM初始化函数 */
void atim_timx_npwm_chy_set(uint32_t npwm);                                                     /* 高级定时器 设置输出PWM的个数 */
void atim_timx_comp_pwm_init(uint16_t arr, uint16_t psc);                                       /* 高级定时器 输出比较模式输出PWM 初始化函数 */

#endif

















