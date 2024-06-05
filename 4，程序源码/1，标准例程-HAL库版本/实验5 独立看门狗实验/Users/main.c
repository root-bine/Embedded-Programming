/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    独立看门狗实验
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
#include "./BSP/IWDG/iwdg.h"


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
    printf("IWDG Test\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("KEY_UP:Feed a dog\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}

int main(void)
{
    HAL_Init();                                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                         /* 设置时钟, 72Mhz */
    delay_init(72);                                             /* 延时初始化 */
    usart_init(115200);                                         /* 初始化串口 */
    led_init();                                                 /* 初始化LED */
    key_init();                                                 /* 初始化按键 */
    delay_ms(100);                                              /* 延时100ms */
    iwdg_init(IWDG_PRESCALER_64, 625);                          /* 预分频数为64,重载值为625,溢出时间约为1s */
    LED0(0);                                                    /* 先点亮LED0 */
    stm32f103c8t6_show_mesg();                                  /* 实验信息 */
    
    while (1)
    {
        if (key_scan(0) == WKUP_PRES)                           /* 如果WK_UP按下，则喂狗 */
        {
            iwdg_feed();                                        /* 喂狗 */
        }
        
        delay_ms(10);
    }
}
