/**
 ****************************************************************************************************
 * @file        atim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       �߼���ʱ�� ��������
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


#include "./BSP/TIM/atim.h"
#include "./BSP/LED/led.h"


TIM_HandleTypeDef g_timx_pwmin_chy_handle;   /* ��ʱ��x��� */

/* PWM����״̬(g_timxchy_cap_sta)
 * 0,û�гɹ�����.
 * 1,�Ѿ��ɹ�������
 */
uint8_t g_timxchy_pwmin_sta  = 0;   /* PWM����״̬ */
uint16_t g_timxchy_pwmin_psc  = 0;  /* PWM�����Ƶϵ�� */
uint32_t g_timxchy_pwmin_hval = 0;  /* PWM�ĸߵ�ƽ���� */
uint32_t g_timxchy_pwmin_cval = 0;  /* PWM�����ڿ�� */

/**
 * @brief       ��ʱ��TIMX ͨ��Y PWM����ģʽ ��ʼ������
 * @note
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 72Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 *              ��������ʼ����ʱ��: ʹ��psc=0, arr�̶�Ϊ65535. �õ�����ʱ��Ƶ��Ϊ72Mhz,����Լ13.8ns
 *
 * @param       ��
 * @retval      ��
 */
void atim_timx_pwmin_chy_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_SlaveConfigTypeDef slave_config = {0};
    TIM_IC_InitTypeDef tim_ic_pwmin_chy = {0};

    ATIM_TIMX_PWMIN_CHY_CLK_ENABLE();
    ATIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE();
//    __HAL_RCC_AFIO_CLK_ENABLE();
//    AFIO_REMAP_PARTIAL(AFIO_EVCR_PORT_PC, AFIO_EVCR_PIN_PX6);       /* ����TIM1_CH1N/PA7 */

    gpio_init_struct.Pin = ATIM_TIMX_PWMIN_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP; 
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    HAL_GPIO_Init(ATIM_TIMX_PWMIN_CHY_GPIO_PORT, &gpio_init_struct);

    g_timx_pwmin_chy_handle.Instance = ATIM_TIMX_PWMIN;             /* ��ʱ��1 */
    g_timx_pwmin_chy_handle.Init.Prescaler = 0;                     /* ��ʱ��Ԥ��Ƶϵ�� */
    g_timx_pwmin_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;  /* ��������ģʽ */
    g_timx_pwmin_chy_handle.Init.Period = 65535;                    /* �Զ���װ��ֵ */
    HAL_TIM_IC_Init(&g_timx_pwmin_chy_handle);
    
    /* ��ģʽ���ã�IT1�������� */
    slave_config.SlaveMode = TIM_SLAVEMODE_RESET;                   /* ��ģʽ����λģʽ */
    slave_config.InputTrigger = TIM_TS_TI1FP1;                      /* ��ʱ�����봥��Դ��TI1FP1 */
    slave_config.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING; /* �����ؼ�� */
    slave_config.TriggerFilter = 0;                                 /* ���˲� */
    HAL_TIM_SlaveConfigSynchro(&g_timx_pwmin_chy_handle, &slave_config);

    /* IC1���������ش���TI1FP1 */
    tim_ic_pwmin_chy.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING; /* �����ؼ�� */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;       /* ѡ�������IC1ӳ�䵽TI1 */
    tim_ic_pwmin_chy.ICPrescaler = TIM_ICPSC_DIV1;                 /* ����Ƶ */
    tim_ic_pwmin_chy.ICFilter = 0;                                 /* ���˲� */
    HAL_TIM_IC_ConfigChannel( &g_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, TIM_CHANNEL_1 );
    
    /* IC2���������ش���TI1FP2 */
    tim_ic_pwmin_chy.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING; /* �½��ؼ�� */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI;      /* ѡ�������IC2ӳ�䵽TI1 */
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle,&tim_ic_pwmin_chy,TIM_CHANNEL_2);
    
    HAL_NVIC_SetPriority(ATIM_TIMX_PWMIN_IRQn, 1, 3);               /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ( ATIM_TIMX_PWMIN_IRQn );                     /* ����TIMx�ж� */
    
    /* TIM1/TIM8�ж��������벶���жϷ����� */
    if ( ATIM_TIMX_PWMIN == TIM1 || ATIM_TIMX_PWMIN == TIM8)
    {
        HAL_NVIC_SetPriority(ATIM_TIMX_PWMIN_CC_IRQn, 1, 3);        /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
        HAL_NVIC_EnableIRQ(ATIM_TIMX_PWMIN_CC_IRQn);                /* ����TIMx�ж� */
    }

    __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE);
    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2);
}

/**
 * @brief       ��ʱ��TIMX PWM����ģʽ ������������
 * @param       ��
 * @retval      ��
 */
void atim_timx_pwmin_chy_restart(void)
{
    sys_intx_disable();                                             /* �ر��ж� */

    g_timxchy_pwmin_sta = 0;                                        /* ����״̬,���¿�ʼ��� */
    g_timxchy_pwmin_psc = 0;                                        /* ��Ƶϵ������ */

    __HAL_TIM_SET_PRESCALER(&g_timx_pwmin_chy_handle, 0);           /* �����ļ���Ƶ�ʲɼ�,�Եõ���õľ��� */
    __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0);             /* ���������� */
    
    __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC1);      /* ʹ��ͨ��1�����ж� */
    __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE);   /* ʹ������ж� */
    __HAL_TIM_ENABLE(&g_timx_pwmin_chy_handle);                     /* ʹ�ܶ�ʱ��TIMX */

    ATIM_TIMX_PWMIN->SR = 0;                                        /* ��������жϱ�־λ */

    sys_intx_enable();                                              /* ���ж� */
}

/**
 * @brief       ��ʱ��TIMX ͨ��Y PWM����ģʽ �жϴ�����
 * @note
 *              ��ΪTIM1/TIM8���ж���жϷ�����,��TIM2~5/TIM12/TIM15����ͨ��ʱ��ֻ��1���жϷ���
 *              ����,Ϊ�˸��õļ���,���Ƕ��жϴ���ͳһ�ŵ�atim_timx_pwin_chy_process����������д���
 *
 * @param       ��
 * @retval      ��
 */
static void atim_timx_pwmin_chy_process(void)
{
    static uint8_t sflag = 0;               /* ����PWMIN�������־ */

    if (g_timxchy_pwmin_sta)
    {
        g_timxchy_pwmin_psc = 0;
        ATIM_TIMX_PWMIN->SR = 0;                                        /* ��������жϱ�־λ */
        __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0);             /* ���������� */
        return ;
    }

    if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE))         /* ����������ж� */
    { 
        __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE);       /* �������жϱ�� */

        if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1) == 0)   /* û�з������ڲ����ж�,�Ҳ���δ��� */
        {
            sflag = 0;
            if (g_timxchy_pwmin_psc == 0)   /* ��0 �� 1 */
            {
                g_timxchy_pwmin_psc ++;
            }
            else
            {
                if (g_timxchy_pwmin_psc == 65535)               /* �Ѿ������,������������״̬ */
                {
                    g_timxchy_pwmin_psc = 0;                    /* ���»ָ�����Ƶ */
                }
                else if (g_timxchy_pwmin_psc > 32767)           /* ���ܱ����� */
                {
                    g_timxchy_pwmin_psc = 65535;                /* ֱ�ӵ�������Ƶϵ�� */
                }
                else
                {
                    g_timxchy_pwmin_psc += g_timxchy_pwmin_psc; /* ���� */
                }
            }

            __HAL_TIM_SET_PRESCALER(&g_timx_pwmin_chy_handle, g_timxchy_pwmin_psc); /* ���ö�ʱ��Ԥ��Ƶϵ�� */
            __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0);                     /* ���������� */
            ATIM_TIMX_PWMIN->SR = 0;                                                /* ��������жϱ�־λ */
            return ;
        }
    }

    if (sflag == 0)   /* ��һ�βɼ��������ж� */
    {
        if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1))         /* ��⵽�˵�һ�����ڲ����ж� */
        { 
            sflag = 1;  /* ��ǵ�һ�������Ѿ�����, �ڶ������ڲ�����Կ�ʼ�� */
        }
        
        ATIM_TIMX_PWMIN->SR = 0;                                                /* ��������жϱ�־λ */
        return ;                    /* ��ɴ˴β��� */
    }

    if (g_timxchy_pwmin_sta == 0)   /* ��û�гɹ����� */
    {
        if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1))         /* ��⵽�����ڲ����ж� */
        {
            g_timxchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2) + 1; /* �߶�ƽ������ֵ */
            g_timxchy_pwmin_cval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1) + 1; /* ���ڲ���ֵ */

            if (g_timxchy_pwmin_hval < g_timxchy_pwmin_cval)    /* �ߵ�ƽ����ض�С�����ڳ��� */
            {
                g_timxchy_pwmin_sta = 1;                        /* ��ǲ���ɹ� */

                g_timxchy_pwmin_psc = ATIM_TIMX_PWMIN->PSC;     /* ��ȡPWM�����Ƶϵ�� */
                
                if (g_timxchy_pwmin_psc == 0)                   /* ��Ƶϵ��Ϊ0��ʱ��, ������ȡ���� */
                {
                    g_timxchy_pwmin_hval++;                     /* ����ϵ��Ϊ1, ��1 */
                    g_timxchy_pwmin_cval++;                     /* ����ϵ��Ϊ1, ��1 */
                }

                sflag = 0;
                /* ÿ�β���PWM����ɹ���, ֹͣ����,����Ƶ���ж�Ӱ��ϵͳ������������ */
                ATIM_TIMX_PWMIN->CR1  &= ~(1 << 0);                             /* �رն�ʱ��TIMX */
                __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC1);     /* �ر�ͨ��1�����ж� */
                __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC2);     /* �ر�ͨ��2�����ж� */
                __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE);  /* �ر�����ж� */

                ATIM_TIMX_PWMIN->SR = 0;                                        /* ��������жϱ�־λ */
            }
            else
            {
                atim_timx_pwmin_chy_restart();
            }
        }
    }
    
    ATIM_TIMX_PWMIN->SR = 0;                                      	            /* ��������жϱ�־λ */
}

/**
 * @brief       ��ʱ��TIMX ����/��� �жϷ�����
 *   @note      TIM1/TIM8��������������ڸ���/����жϷ���,����������һ������!
 *              ������ͨ��ʱ�������/���/����,��������������洦��!
 * @param       ��
 * @retval      ��
 */
void ATIM_TIMX_PWMIN_IRQHandler(void)
{
    atim_timx_pwmin_chy_process();
}

/**
 * @brief       ��ʱ��TIMX ���벶�� �жϷ�����
 *   @note      ��TIM1/TIM8���������,������ͨ��ʱ��û������жϷ�����!
 * @param       ��
 * @retval      ��
 */
void ATIM_TIMX_PWMIN_CC_IRQHandler(void)
{
    atim_timx_pwmin_chy_process();
}
