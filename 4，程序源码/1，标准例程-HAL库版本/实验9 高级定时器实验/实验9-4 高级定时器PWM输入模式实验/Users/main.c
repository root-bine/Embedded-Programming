/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    高级定时器PWM输入模式实验
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
#include "./BSP/TIM/gtim.h"


extern uint16_t g_timxchy_pwmin_psc;    /* PWM输入状态 */
extern uint16_t g_timxchy_pwmin_sta;    /* PWM输入状态 */
extern uint32_t g_timxchy_pwmin_hval;   /* PWM的高电平脉宽 */
extern uint32_t g_timxchy_pwmin_cval;   /* PWM的周期宽度 */

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
    printf("ATIM PWM Input Test\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

int main(void)
{
    uint8_t t = 0;
    double ht, ct, f, tpsc;
    
    HAL_Init();                                                     /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                             /* 设置时钟, 72Mhz */
    delay_init(72);                                                 /* 延时初始化 */
    usart_init(115200);                                             /* 初始化串口 */
    key_init();                                                     /* 初始化按键 */
    led_init();                                                     /* 初始化LED */
    stm32f103c8t6_show_mesg();                                      /* 显示实验信息 */
    gtim_timx_pwm_chy_init(10 - 1, 72 - 1);                         /* 1Mhz的计数频率, 100Khz PWM */
    atim_timx_pwmin_chy_init();                                     /* 初始化PWM输入捕获 */

    GTIM_TIMX_PWM_CHY_CCRX = 2;                                     /* 低电平宽度20,高电平宽度80 */

    while (1)
    {
        delay_ms(10);
        t++;

        if (t >= 20)                                                /* 每200ms输出一次结果,并闪烁LED0,提示程序运行 */
        {
            if (g_timxchy_pwmin_sta)                                /* 捕获了一次数据 */
            {
                printf("\r\n");                                     /* 输出空,另起一行 */
                printf("PWM PSC  :%d\r\n", g_timxchy_pwmin_psc);    /* 打印分频系数 */
                printf("PWM Hight:%d\r\n", g_timxchy_pwmin_hval);   /* 打印高电平脉宽 */
                printf("PWM Cycle:%d\r\n", g_timxchy_pwmin_cval);   /* 打印周期 */
                tpsc = ((double)g_timxchy_pwmin_psc + 1) / 72;      /* 得到PWM采样时钟周期时间 */ 
                ht = g_timxchy_pwmin_hval * tpsc;                   /* 计算高电平时间 */
                ct = g_timxchy_pwmin_cval * tpsc;                   /* 计算周期长度 */
                f = (1 / ct) * 1000000;                             /* 计算频率 */
                printf("PWM Hight time:%.3fus\r\n", ht);            /* 打印高电平脉宽长度 */
                printf("PWM Cycle time:%.3fus\r\n", ct);            /* 打印周期时间长度 */
                printf("PWM Frequency :%.3fHz\r\n", f);             /* 打印频率 */ 
                
                atim_timx_pwmin_chy_restart();                      /* 重启PWM输入检测 */
            } 
            t = 0;
        }
    }
}
