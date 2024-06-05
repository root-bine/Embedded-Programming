/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    通用定时器脉冲计数实验
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
#include "./BSP/TIM/gtim.h"
#include "./BSP/KEY/key.h"


extern uint8_t  g_timxchy_cap_sta;  /* 输入捕获状态 */
extern uint16_t g_timxchy_cap_val;  /* 输入捕获值 */

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

    HAL_Init();                                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                         /* 设置时钟, 72Mhz */
    delay_init(72);                                             /* 延时初始化 */
    usart_init(115200);                                         /* 初始化串口 */
    led_init();                                                 /* 初始化LED */
    key_init();                                                 /* 初始化按键 */
    stm32f103c8t6_show_mesg();                                  /* 实验提示信息 */
    gtim_timx_cnt_chy_init(0);                                  /* 定时器计数初始化, 不分频 */
    gtim_timx_cnt_chy_restart();                                /* 重启计数 */

    while (1)
    {
        key = key_scan(0);                                      /* 扫描按键 */

        if (key == KEY0_PRES)                                   /* KEY0按键按下,重启计数 */
        {
            gtim_timx_cnt_chy_restart();                        /* 重新启动计数 */
        }

        curcnt = gtim_timx_cnt_chy_get_count();                 /* 获取计数值 */

        if (oldcnt != curcnt)
        {
            oldcnt = curcnt;
            printf("CNT:%d\r\n", oldcnt);                       /* 打印脉冲个数 */
        }

        t++;

        if (t > 20)                                             /* 200ms进入一次 */
        {
            t = 0;
            LED0_TOGGLE();                                      /* LED0闪烁 ,提示程序运行 */
        }

        delay_ms(10);
    }
}
