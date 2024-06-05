/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    高级定时器输出指定个数PWM实验
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
    printf("KEY0:Output PWM pulses\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

int main(void)
{
    uint8_t key = 0;
    uint8_t t = 0;
    
    HAL_Init();                                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                         /* 设置时钟, 72Mhz */
    delay_init(72);                                             /* 延时初始化 */
    usart_init(115200);                                         /* 初始化串口 */
    led_init();                                                 /* 初始化LED */
    key_init();                                                 /* 初始化按键 */
	stm32f103c8t6_show_mesg();									/* 显示实验信息 */
    atim_timx_npwm_chy_init(5000 - 1, 7200 - 1);                /* 10Khz的计数频率,2hz的PWM频率. */

    ATIM_TIMX_NPWM_CHY_CCRX = 2500;                             /* 设置PWM占空比,50%,这样可以控制每一个PWM周期,LED0(RED)
                                                                 * 有一半时间是亮的,一半时间是灭的,LED0亮灭一次,表示一个PWM波
                                                                 */
    atim_timx_npwm_chy_set(5);                                  /* 输出5个PWM波(控制LED0(RED)闪烁5次) */

    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)                                   /* KEY0按下 */
        {
            atim_timx_npwm_chy_set(5);                          /* 输出5个PWM波(控制TIM1_CH1, 即PA8输出5个脉冲) */
        }

        t++;
        delay_ms(10);

        if (t > 20)                                             /* 控制LED1闪烁, 提示程序运行状态 */
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}
