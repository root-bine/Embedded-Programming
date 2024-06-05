/**
 ****************************************************************************************************
 * @file        atim.c
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


#include "./BSP/TIM/atim.h"
#include "./BSP/LED/led.h"


TIM_HandleTypeDef g_timx_pwmin_chy_handle;   /* 定时器x句柄 */

/* PWM输入状态(g_timxchy_cap_sta)
 * 0,没有成功捕获.
 * 1,已经成功捕获了
 */
uint8_t g_timxchy_pwmin_sta  = 0;   /* PWM输入状态 */
uint16_t g_timxchy_pwmin_psc  = 0;  /* PWM输入分频系数 */
uint32_t g_timxchy_pwmin_hval = 0;  /* PWM的高电平脉宽 */
uint32_t g_timxchy_pwmin_cval = 0;  /* PWM的周期宽度 */

/**
 * @brief       定时器TIMX 通道Y PWM输入模式 初始化函数
 * @note
 *              高级定时器的时钟来自APB2, 而PCLK2 = 72Mhz, 我们设置PPRE2不分频, 因此
 *              高级定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 *              本函数初始化的时候: 使用psc=0, arr固定为65535. 得到采样时钟频率为72Mhz,精度约13.8ns
 *
 * @param       无
 * @retval      无
 */
void atim_timx_pwmin_chy_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_SlaveConfigTypeDef slave_config = {0};
    TIM_IC_InitTypeDef tim_ic_pwmin_chy = {0};

    ATIM_TIMX_PWMIN_CHY_CLK_ENABLE();
    ATIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE();
//    __HAL_RCC_AFIO_CLK_ENABLE();
//    AFIO_REMAP_PARTIAL(AFIO_EVCR_PORT_PC, AFIO_EVCR_PIN_PX6);       /* 复用TIM1_CH1N/PA7 */

    gpio_init_struct.Pin = ATIM_TIMX_PWMIN_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP; 
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    HAL_GPIO_Init(ATIM_TIMX_PWMIN_CHY_GPIO_PORT, &gpio_init_struct);

    g_timx_pwmin_chy_handle.Instance = ATIM_TIMX_PWMIN;             /* 定时器1 */
    g_timx_pwmin_chy_handle.Init.Prescaler = 0;                     /* 定时器预分频系数 */
    g_timx_pwmin_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;  /* 递增计数模式 */
    g_timx_pwmin_chy_handle.Init.Period = 65535;                    /* 自动重装载值 */
    HAL_TIM_IC_Init(&g_timx_pwmin_chy_handle);
    
    /* 从模式配置，IT1触发更新 */
    slave_config.SlaveMode = TIM_SLAVEMODE_RESET;                   /* 从模式：复位模式 */
    slave_config.InputTrigger = TIM_TS_TI1FP1;                      /* 定时器输入触发源：TI1FP1 */
    slave_config.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING; /* 上升沿检测 */
    slave_config.TriggerFilter = 0;                                 /* 不滤波 */
    HAL_TIM_SlaveConfigSynchro(&g_timx_pwmin_chy_handle, &slave_config);

    /* IC1捕获：上升沿触发TI1FP1 */
    tim_ic_pwmin_chy.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING; /* 上升沿检测 */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;       /* 选择输入端IC1映射到TI1 */
    tim_ic_pwmin_chy.ICPrescaler = TIM_ICPSC_DIV1;                 /* 不分频 */
    tim_ic_pwmin_chy.ICFilter = 0;                                 /* 不滤波 */
    HAL_TIM_IC_ConfigChannel( &g_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, TIM_CHANNEL_1 );
    
    /* IC2捕获：上升沿触发TI1FP2 */
    tim_ic_pwmin_chy.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING; /* 下降沿检测 */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI;      /* 选择输入端IC2映射到TI1 */
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle,&tim_ic_pwmin_chy,TIM_CHANNEL_2);
    
    HAL_NVIC_SetPriority(ATIM_TIMX_PWMIN_IRQn, 1, 3);               /* 设置中断优先级，抢占优先级1，子优先级3 */
    HAL_NVIC_EnableIRQ( ATIM_TIMX_PWMIN_IRQn );                     /* 开启TIMx中断 */
    
    /* TIM1/TIM8有独立的输入捕获中断服务函数 */
    if ( ATIM_TIMX_PWMIN == TIM1 || ATIM_TIMX_PWMIN == TIM8)
    {
        HAL_NVIC_SetPriority(ATIM_TIMX_PWMIN_CC_IRQn, 1, 3);        /* 设置中断优先级，抢占优先级1，子优先级3 */
        HAL_NVIC_EnableIRQ(ATIM_TIMX_PWMIN_CC_IRQn);                /* 开启TIMx中断 */
    }

    __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE);
    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2);
}

/**
 * @brief       定时器TIMX PWM输入模式 重新启动捕获
 * @param       无
 * @retval      无
 */
void atim_timx_pwmin_chy_restart(void)
{
    sys_intx_disable();                                             /* 关闭中断 */

    g_timxchy_pwmin_sta = 0;                                        /* 清零状态,重新开始检测 */
    g_timxchy_pwmin_psc = 0;                                        /* 分频系数清零 */

    __HAL_TIM_SET_PRESCALER(&g_timx_pwmin_chy_handle, 0);           /* 以最大的计数频率采集,以得到最好的精度 */
    __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0);             /* 计数器清零 */
    
    __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC1);      /* 使能通道1捕获中断 */
    __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE);   /* 使能溢出中断 */
    __HAL_TIM_ENABLE(&g_timx_pwmin_chy_handle);                     /* 使能定时器TIMX */

    ATIM_TIMX_PWMIN->SR = 0;                                        /* 清除所有中断标志位 */

    sys_intx_enable();                                              /* 打开中断 */
}

/**
 * @brief       定时器TIMX 通道Y PWM输入模式 中断处理函数
 * @note
 *              因为TIM1/TIM8等有多个中断服务函数,而TIM2~5/TIM12/TIM15等普通定时器只有1个中断服务
 *              函数,为了更好的兼容,我们对中断处理统一放到atim_timx_pwin_chy_process函数里面进行处理
 *
 * @param       无
 * @retval      无
 */
static void atim_timx_pwmin_chy_process(void)
{
    static uint8_t sflag = 0;               /* 启动PWMIN输入检测标志 */

    if (g_timxchy_pwmin_sta)
    {
        g_timxchy_pwmin_psc = 0;
        ATIM_TIMX_PWMIN->SR = 0;                                        /* 清除所有中断标志位 */
        __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0);             /* 计数器清零 */
        return ;
    }

    if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE))         /* 发生了溢出中断 */
    { 
        __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE);       /* 清除溢出中断标记 */

        if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1) == 0)   /* 没有发生周期捕获中断,且捕获未完成 */
        {
            sflag = 0;
            if (g_timxchy_pwmin_psc == 0)   /* 从0 到 1 */
            {
                g_timxchy_pwmin_psc ++;
            }
            else
            {
                if (g_timxchy_pwmin_psc == 65535)               /* 已经最大了,可能是无输入状态 */
                {
                    g_timxchy_pwmin_psc = 0;                    /* 重新恢复不分频 */
                }
                else if (g_timxchy_pwmin_psc > 32767)           /* 不能倍增了 */
                {
                    g_timxchy_pwmin_psc = 65535;                /* 直接等于最大分频系数 */
                }
                else
                {
                    g_timxchy_pwmin_psc += g_timxchy_pwmin_psc; /* 倍增 */
                }
            }

            __HAL_TIM_SET_PRESCALER(&g_timx_pwmin_chy_handle, g_timxchy_pwmin_psc); /* 设置定时器预分频系数 */
            __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0);                     /* 计数器清零 */
            ATIM_TIMX_PWMIN->SR = 0;                                                /* 清除所有中断标志位 */
            return ;
        }
    }

    if (sflag == 0)   /* 第一次采集到捕获中断 */
    {
        if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1))         /* 检测到了第一次周期捕获中断 */
        { 
            sflag = 1;  /* 标记第一次周期已经捕获, 第二次周期捕获可以开始了 */
        }
        
        ATIM_TIMX_PWMIN->SR = 0;                                                /* 清除所有中断标志位 */
        return ;                    /* 完成此次操作 */
    }

    if (g_timxchy_pwmin_sta == 0)   /* 还没有成功捕获 */
    {
        if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1))         /* 检测到了周期捕获中断 */
        {
            g_timxchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2) + 1; /* 高定平脉宽捕获值 */
            g_timxchy_pwmin_cval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1) + 1; /* 周期捕获值 */

            if (g_timxchy_pwmin_hval < g_timxchy_pwmin_cval)    /* 高电平脉宽必定小于周期长度 */
            {
                g_timxchy_pwmin_sta = 1;                        /* 标记捕获成功 */

                g_timxchy_pwmin_psc = ATIM_TIMX_PWMIN->PSC;     /* 获取PWM输入分频系数 */
                
                if (g_timxchy_pwmin_psc == 0)                   /* 分频系数为0的时候, 修正读取数据 */
                {
                    g_timxchy_pwmin_hval++;                     /* 修正系数为1, 加1 */
                    g_timxchy_pwmin_cval++;                     /* 修正系数为1, 加1 */
                }

                sflag = 0;
                /* 每次捕获PWM输入成功后, 停止捕获,避免频繁中断影响系统正常代码运行 */
                ATIM_TIMX_PWMIN->CR1  &= ~(1 << 0);                             /* 关闭定时器TIMX */
                __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC1);     /* 关闭通道1捕获中断 */
                __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC2);     /* 关闭通道2捕获中断 */
                __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE);  /* 关闭溢出中断 */

                ATIM_TIMX_PWMIN->SR = 0;                                        /* 清除所有中断标志位 */
            }
            else
            {
                atim_timx_pwmin_chy_restart();
            }
        }
    }
    
    ATIM_TIMX_PWMIN->SR = 0;                                      	            /* 清除所有中断标志位 */
}

/**
 * @brief       定时器TIMX 更新/溢出 中断服务函数
 *   @note      TIM1/TIM8的这个函数仅用于更新/溢出中断服务,捕获在另外一个函数!
 *              其他普通定时器则更新/溢出/捕获,都在这个函数里面处理!
 * @param       无
 * @retval      无
 */
void ATIM_TIMX_PWMIN_IRQHandler(void)
{
    atim_timx_pwmin_chy_process();
}

/**
 * @brief       定时器TIMX 输入捕获 中断服务函数
 *   @note      仅TIM1/TIM8有这个函数,其他普通定时器没有这个中断服务函数!
 * @param       无
 * @retval      无
 */
void ATIM_TIMX_PWMIN_CC_IRQHandler(void)
{
    atim_timx_pwmin_chy_process();
}
