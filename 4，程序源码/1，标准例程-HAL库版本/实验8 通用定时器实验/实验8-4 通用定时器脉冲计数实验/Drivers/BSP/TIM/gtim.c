/**
 ****************************************************************************************************
 * @file        gtim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       ͨ�ö�ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� M48Z-M3��Сϵͳ��STM32F103��
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./BSP/TIM/gtim.h"
#include "./BSP/LED/led.h"


TIM_HandleTypeDef g_timx_cnt_chy_handle;        /* ��ʱ��x��� */

/* ��¼��ʱ�����������������, ���������������� */
uint32_t g_timxchy_cnt_ofcnt = 0 ;              /* ����������� */

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ������� ��ʼ������
 * @note
 *              ������ѡ��ͨ�ö�ʱ����ʱ��ѡ��: �ⲿʱ��Դģʽ1(SMS[2:0] = 111)
 *              ����CNT�ļ���ʱ��Դ������ TIMX_CH1/CH2, ����ʵ���ⲿ�������(�������CH1/CH2)
 *
 *              ʱ�ӷ�Ƶ�� = psc, һ������Ϊ0, ��ʾÿһ��ʱ�Ӷ������һ��, ����߾���.
 *              ͨ����ȡCNT���������, �����򵥼���, ���Եõ���ǰ�ļ���ֵ, �Ӷ�ʵ���������
 *
 * @param       arr: �Զ���װֵ 
 * @retval      ��
 */
void gtim_timx_cnt_chy_init(uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_SlaveConfigTypeDef tim_slave_config = {0};
    GTIM_TIMX_CNT_CHY_CLK_ENABLE();                                          /* ʹ��TIMxʱ�� */
    GTIM_TIMX_CNT_CHY_GPIO_CLK_ENABLE();                                     /* ����GPIOAʱ�� */
    
    g_timx_cnt_chy_handle.Instance = GTIM_TIMX_CNT;                          /* ��ʱ��x */
    g_timx_cnt_chy_handle.Init.Prescaler = psc;                              /* ��ʱ����Ƶ */
    g_timx_cnt_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* ��������ģʽ */
    g_timx_cnt_chy_handle.Init.Period = 65535;                               /* �Զ���װ��ֵ */
    HAL_TIM_IC_Init(&g_timx_cnt_chy_handle);

    gpio_init_struct.Pin = GTIM_TIMX_CNT_CHY_GPIO_PIN;                       /* ���벶���GPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                                 /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                                   /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                           /* ���� */
    HAL_GPIO_Init(GTIM_TIMX_CNT_CHY_GPIO_PORT, &gpio_init_struct);

    /* ��ģʽ���ⲿ����ģʽ1 */
    tim_slave_config.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;                    /* ��ģʽ���ⲿ����ģʽ1 */
    tim_slave_config.InputTrigger = TIM_TS_TI1FP1;                           /* ���봥����ѡ�� TI1FP1(TIMX_CH1) ��Ϊ����Դ */
    tim_slave_config.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;           /* �������ԣ������� */
    tim_slave_config.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;           /* ����Ԥ��Ƶ���� */
    tim_slave_config.TriggerFilter = 0x0;                                    /* �˲��������в���Ҫ�κ��˲� */
    HAL_TIM_SlaveConfigSynchro(&g_timx_cnt_chy_handle, &tim_slave_config);

    HAL_NVIC_SetPriority(GTIM_TIMX_CNT_IRQn, 1, 3);                          /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_CNT_IRQn);

    __HAL_TIM_ENABLE_IT(&g_timx_cnt_chy_handle, TIM_IT_UPDATE);              /* ʹ�ܸ����ж� */
    HAL_TIM_IC_Start(&g_timx_cnt_chy_handle, GTIM_TIMX_CNT_CHY);             /* ��ʼ����TIMx��ͨ��y */
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ��ȡ��ǰ����ֵ 
 * @param       ��
 * @retval      ��ǰ����ֵ
 */
uint32_t gtim_timx_cnt_chy_get_count(void)
{
    uint32_t count = 0;
    count = g_timxchy_cnt_ofcnt * 65536;                     /* �������������Ӧ�ļ���ֵ */
    count += __HAL_TIM_GET_COUNTER(&g_timx_cnt_chy_handle);  /* ���ϵ�ǰCNT��ֵ */
    return count;
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ����������
 * @param       ��
 * @retval      ��ǰ����ֵ
 */
void gtim_timx_cnt_chy_restart(void)
{
    __HAL_TIM_DISABLE(&g_timx_cnt_chy_handle);        /* �رն�ʱ��TIMX */
    g_timxchy_cnt_ofcnt = 0;                          /* �ۼ������� */
    __HAL_TIM_SET_COUNTER(&g_timx_cnt_chy_handle, 0); /* ���������� */
    __HAL_TIM_ENABLE(&g_timx_cnt_chy_handle);         /* ʹ�ܶ�ʱ��TIMX */
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ������� �����жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_CNT_IRQHandler(void)
{
    /* ���´���û��ʹ�ö�ʱ��HAL�⹲�ô���������������ֱ��ͨ���ж��жϱ�־λ�ķ�ʽ */
    if(__HAL_TIM_GET_FLAG(&g_timx_cnt_chy_handle, TIM_FLAG_UPDATE) != RESET)
    {
        g_timxchy_cnt_ofcnt++;          /* �ۼ�������� */
    }

      __HAL_TIM_CLEAR_IT(&g_timx_cnt_chy_handle, TIM_IT_UPDATE);
}
