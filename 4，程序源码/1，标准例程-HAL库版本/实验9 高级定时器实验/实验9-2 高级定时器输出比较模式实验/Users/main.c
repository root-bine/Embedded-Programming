/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    高级定时器输出比较模式实验
 * @license  Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ******************************************************************************
 * @attention
 * 
 * 实验平台:正点原子 M48Z-M3最小系统板STM32F103版
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 ******************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TIM/atim.h"


/**
 * @brief       显示实验信息
 * @param       无
 * @retval      无
 */
void stm32f103c8t6_show_mesg(void)
{
    /* 串口输出实验信息 */
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
    HAL_Init();                                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                         /* 设置时钟, 72Mhz */
    delay_init(72);                                             /* 延时初始化 */
//    usart_init(115200);                                         /* 初始化串口 */
    led_init();                                                 /* 初始化LED */
    key_init();                                                 /* 初始化按键 */
//    stm32f103c8t6_show_mesg();                                  /* 显示实验信息 */
    delay_ms(100);
    atim_timx_comp_pwm_init(1000 - 1, 72 - 1);                  /* 1Mhz的计数频率 1Khz的周期 */
    ATIM_TIMX_COMP_CH1_CCRX = 250 - 1;                          /* 通道1 相位25% */
    ATIM_TIMX_COMP_CH2_CCRX = 500 - 1;                          /* 通道2 相位50% */
    ATIM_TIMX_COMP_CH3_CCRX = 750 - 1;                          /* 通道3 相位75% */
    ATIM_TIMX_COMP_CH4_CCRX = 1000 - 1;                         /* 通道4 相位100% */

    while (1)
    {
        delay_ms(10);
    }
}
