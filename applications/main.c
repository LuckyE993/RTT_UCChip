/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-26     RT-Thread    first version
 */
#include <rtthread.h>
#define RT_USING_HWTIMER
#define HWTIMER_DEV_NAME   "timer0"     /* 定时器名称 */

/* 定时器超时回调函数 */
static rt_err_t timeout_cb(rt_device_t dev, rt_size_t size)
{
    rt_kprintf("this is hwtimer timeout callback fucntion!\n");
    rt_kprintf("tick is :%d !\n", rt_tick_get());

    return 0;
}
    

#include "uc_timer.h"
#include "uc_int.h"
#ifdef _RT_THREAD_
#include <rtdevice.h>
#include <uc_gpio.h>
#endif

#ifdef _FPGA_
#include <board.h>
#include "uc_event.h"
#endif

#ifndef WIOTA_APP_DEMO
#ifdef UC8288_MODULE
#ifdef RT_USING_AT
#include "at.h"
#endif
#else
#include "test_wiota_api.h"
#endif
#else
#include "app_manager.h"
#endif

#ifdef _WATCHDOG_APP_
#include "uc_watchdog_app.h"
#endif

#ifdef _ROMFUNC_
#include "dll.h"
#endif

#ifdef _ADC_APP_
#include "uc_adc_app.h"
#endif

#ifdef _CAN_APP_
#include "uc_can_app.h"
#endif

#ifdef _DAC_APP_
#include "uc_dac_app.h"
#endif

#ifdef _IIC_APP_
#include "uc_iic_app.h"
#endif

#ifdef _PIN_APP_
#include "uc_pin_app.h"
#endif

#ifdef _PWM_APP_
#include "uc_pwm_app.h"
#endif

#ifdef _RTC_APP_
#include "uc_rtc_app.h"
#endif

#ifdef _SPI_FLASH_APP_
#include "uc_spi_flash_app.h"
#endif

#include "hello_wiota.h"
#include <stdio.h>


extern void uc_wiota_static_data_init(void);

#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
extern void at_handle_log_uart(int uart_number);
#endif
extern void at_wiota_manager(void);
//void task_callback(struct rt_thread* from, struct rt_thread* to)
//{
//    rt_kprintf("name = %s, 0x%x\n", from->name, from);
//}
//
//
//void init_statistical_task_info(void)
//{
//    rt_scheduler_sethook(task_callback);
//}

void app_test(void)
{
#ifdef _ADC_APP_
    adc_app_sample();
#endif

#ifdef _CAN_APP_
    can_app_sample();
#endif

#ifdef _DAC_APP_
    dac_app_sample();
#endif

#ifdef _IIC_APP_
    iic_app_sample();
#endif

#ifdef _PIN_APP_
    pin_app_sample();
#endif

#ifdef _PWM_APP_
    pwm_app_sample();
#endif

#ifdef _RTC_APP_
    rtc_app_sample();
//    alarm_app_sample();
#endif

#ifdef _SPI_FLASH_APP_
    spi_flash_app_sample();
#endif
}

#include "bsp_timer.h"
int main(void)
{

#ifdef _ROMFUNC_
    dll_open();
#endif

    uc_wiota_static_data_init();

#ifdef _WATCHDOG_APP_
    if (!watchdog_app_init())
        watchdog_app_enable();
#endif

    //app_test();

    hello();
    rt_kprintf("main is running1\n");
#ifndef WIOTA_APP_DEMO
#ifdef UC8288_MODULE
#ifdef RT_USING_AT
    at_server_init();
    at_wiota_manager();
#endif
#else
    app_task_init();
#endif
#else
    app_manager_enter();
#ifndef AT_WIOTA_GATEWAY
    app_wiota_gateway_api_demo();
#endif
#endif

#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
//    at_handle_log_uart(0);
#endif

    rt_err_t ret = RT_EOK;
    rt_hwtimerval_t timeout_s;      /* 定时器超时值 */
    rt_device_t hw_dev = RT_NULL;   /* 定时器设备句柄 */
    rt_hwtimer_mode_t mode;         /* 定时器模式 */
    rt_uint32_t freq = 10000;               /* 计数频率 */

    /* 查找定时器设备 */
    hw_dev = rt_device_find(HWTIMER_DEV_NAME);
    if (hw_dev == RT_NULL)
    {
        rt_kprintf("hwtimer sample run failed! can't find %s device!\n", HWTIMER_DEV_NAME);
        return RT_ERROR;
    }

    /* 以读写方式打开设备 */
    ret = rt_device_open(hw_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", HWTIMER_DEV_NAME);
        return ret;
    }

    /* 设置超时回调函数 */
    rt_device_set_rx_indicate(hw_dev, timeout_cb);

    /* 设置计数频率(若未设置该项，默认为1Mhz 或 支持的最小计数频率) */
    rt_device_control(hw_dev, HWTIMER_CTRL_FREQ_SET, &freq);
    /* 设置模式为周期性定时器（若未设置，默认是HWTIMER_MODE_ONESHOT）*/
    mode = HWTIMER_MODE_PERIOD;
    ret = rt_device_control(hw_dev, HWTIMER_CTRL_MODE_SET, &mode);
    if (ret != RT_EOK)
    {
        rt_kprintf("set mode failed! ret is :%d\n", ret);
        return ret;
    }

    /* 设置定时器超时值为5s并启动定时器 */
    timeout_s.sec = 5;      /* 秒 */
    timeout_s.usec = 0;     /* 微秒 */
    if (rt_device_write(hw_dev, 0, &timeout_s, sizeof(timeout_s)) != sizeof(timeout_s))
    {
        rt_kprintf("set timeout value failed\n");
        return RT_ERROR;
    }

    /* 延时3500ms */
    rt_thread_mdelay(3500);

    /* 读取定时器当前值 */
    rt_device_read(hw_dev, 0, &timeout_s, sizeof(timeout_s));
    rt_kprintf("Read: Sec = %d, Usec = %d\n", timeout_s.sec, timeout_s.usec);










    rt_kprintf("main is running2\n");
    hello();
    printf("---------timer ovf demo start---------\r\n");
    
    printf("-------------Timer Demo start--------------\n");
   
    

    // gpio_set_pin_direction(UC_GPIO,GPIO_PIN_2,GPIO_DIR_OUT);
    // gpio_set_pin_value(UC_GPIO,GPIO_PIN_2,1);
    
    // printf("GPIO2 OUT HIGH\n");

    // DHT11_GPIO_Config();
    // if(DHT11_Read(&DHT11_DATA) == SUCCESS)
    // {
    //     rt_kprintf("DHT11 Data\n");
    //     rt_kprintf("================================\n");
    //     rt_kprintf("Temperature:%d.%d\n",DHT11_DATA.temp_int,DHT11_DATA.temp_deci);
    //     rt_kprintf("Humilarity:%d.%d\n",DHT11_DATA.humi_int,DHT11_DATA.humi_deci);
    // }
    // else
    // {
    //     rt_kprintf("Wrong Answer\n");
    // }

    // DHT11_GPIO_Config();
    // if(DHT11_Read(&DHT11_DATA) == SUCCESS)
    // {
    //     rt_kprintf("DHT11 Data\n");
    //     rt_kprintf("================================\n");
    //     rt_kprintf("Temperature:%d.%d\n",DHT11_DATA.temp_int,DHT11_DATA.temp_deci);
    //     rt_kprintf("Humilarity:%d.%d\n",DHT11_DATA.humi_int,DHT11_DATA.humi_deci);
    // }
    
    // else
    // {
    //     rt_kprintf("Wrong Answer\n");
    // }
    
    // if(DHT11_Read(&DHT11_DATA)==SUCCESS)
	// {
	// 	printf("DHT11 Data\n");
    //     printf("================================");
    //     printf("Temperature:%d.%d",DHT11_DATA.temp_int,DHT11_DATA.temp_deci);	
	// }
    
    


    
    
    return 0;
}
