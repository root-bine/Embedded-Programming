/**
 ******************************************************************************
 * @file     main.c
 * @author   ����ԭ���Ŷ�(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    FreeRTOS��ֲʵ��
 * @license  Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ******************************************************************************
 * @attention
 * 
 * ʵ��ƽ̨:����ԭ�� M48Z-M3��Сϵͳ��STM32F103��
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 ******************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "FreeRTOS.h"
#include "task.h"


/* START_TASK��������
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1                                   /* �������ȼ� */
#define START_STK_SIZE  128                                 /* �����ջ��С */
TaskHandle_t StartTask_Handler;                             /* ������ */
void start_task(void *pvParameters);                        /* ������ */

/* LED0��������
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED0_PRIO      2                                    /* �������ȼ� */
#define LED0_STK_SIZE  128                                  /* �����ջ��С */
TaskHandle_t LED0Task_Handler;                              /* ������ */
void led0_task(void *pvParameters);                         /* ������ */

/* PRINTF��������
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define PRINTF_PRIO      3                                  /* �������ȼ� */
#define PRINTF_STK_SIZE  128                                /* �����ջ��С */
TaskHandle_t PRINTFTask_Handler;                            /* ������ */
void printf_task(void *pvParameters);                       /* ������ */


int main(void)
{
    HAL_Init();                                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);                     /* ����ʱ��, 72Mhz */
    delay_init(72);                                         /* ��ʱ��ʼ�� */
    usart_init(115200);                                     /* ��ʼ������ */
    led_init();                                             /* ��ʼ��LED */
    
    xTaskCreate((TaskFunction_t )start_task,                /* ������ */
                (const char*    )"start_task",              /* �������� */
                (uint16_t       )START_STK_SIZE,            /* �����ջ��С */
                (void*          )NULL,                      /* ������������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,           /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);       /* ������ */
                
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters: �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    ( void ) pvParameters;
    taskENTER_CRITICAL();                                   /* �����ٽ��� */
    
    /* ����LED0���� */
    xTaskCreate((TaskFunction_t )led0_task,
                (const char*    )"led0_task",
                (uint16_t       )LED0_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED0_PRIO,
                (TaskHandle_t*  )&LED0Task_Handler);
                
    /* ����LED1���� */
    xTaskCreate((TaskFunction_t )printf_task,
                (const char*    )"printf_task",
                (uint16_t       )PRINTF_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )PRINTF_PRIO,
                (TaskHandle_t*  )&PRINTFTask_Handler);
                
    vTaskDelete(StartTask_Handler);                     /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();                                /* �˳��ٽ��� */
}

/**
 * @brief       LED0����
 * @param       pvParameters: �������(δ�õ�)
 * @retval      ��
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
 * @brief       printf����
 * @param       pvParameters: �������(δ�õ�)
 * @retval      ��
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

