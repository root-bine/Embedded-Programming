/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    FreeRTOS移植实验
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
#include "FreeRTOS.h"
#include "task.h"


/* START_TASK任务配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                                   /* 任务优先级 */
#define START_STK_SIZE  128                                 /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;                             /* 任务句柄 */
void start_task(void *pvParameters);                        /* 任务函数 */

/* LED0任务配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED0_PRIO      2                                    /* 任务优先级 */
#define LED0_STK_SIZE  128                                  /* 任务堆栈大小 */
TaskHandle_t LED0Task_Handler;                              /* 任务句柄 */
void led0_task(void *pvParameters);                         /* 任务函数 */

/* PRINTF任务配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define PRINTF_PRIO      3                                  /* 任务优先级 */
#define PRINTF_STK_SIZE  128                                /* 任务堆栈大小 */
TaskHandle_t PRINTFTask_Handler;                            /* 任务句柄 */
void printf_task(void *pvParameters);                       /* 任务函数 */


int main(void)
{
    HAL_Init();                                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                     /* 设置时钟, 72Mhz */
    delay_init(72);                                         /* 延时初始化 */
    usart_init(115200);                                     /* 初始化串口 */
    led_init();                                             /* 初始化LED */
    
    xTaskCreate((TaskFunction_t )start_task,                /* 任务函数 */
                (const char*    )"start_task",              /* 任务名称 */
                (uint16_t       )START_STK_SIZE,            /* 任务堆栈大小 */
                (void*          )NULL,                      /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,           /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);       /* 任务句柄 */
                
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters: 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    ( void ) pvParameters;
    taskENTER_CRITICAL();                                   /* 进入临界区 */
    
    /* 创建LED0任务 */
    xTaskCreate((TaskFunction_t )led0_task,
                (const char*    )"led0_task",
                (uint16_t       )LED0_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED0_PRIO,
                (TaskHandle_t*  )&LED0Task_Handler);
                
    /* 创建LED1任务 */
    xTaskCreate((TaskFunction_t )printf_task,
                (const char*    )"printf_task",
                (uint16_t       )PRINTF_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )PRINTF_PRIO,
                (TaskHandle_t*  )&PRINTFTask_Handler);
                
    vTaskDelete(StartTask_Handler);                     /* 删除开始任务 */
    taskEXIT_CRITICAL();                                /* 退出临界区 */
}

/**
 * @brief       LED0任务
 * @param       pvParameters: 传入参数(未用到)
 * @retval      无
 */
void led0_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while(1)
    {
        LED0_TOGGLE();
        vTaskDelay(1000);
    }
}

/**
 * @brief       printf任务
 * @param       pvParameters: 传入参数(未用到)
 * @retval      无
 */
void printf_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while(1)
    {
        printf("STM32F103C8T6 Runing .....\r\n");
        
        vTaskDelay(1000);
    }
}

