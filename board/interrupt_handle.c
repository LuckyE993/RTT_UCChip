
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "uc_utils.h"
#include "uc_event.h"
#include "uc_int.h"
#include "uc_timer.h"
__attribute__ ((weak))
void rtc_handler(void)
{
    return;
}

__attribute__ ((weak))
void cce_handler(void)
{
    return;
}

__attribute__ ((weak))
void pls_cnt_compare_handler(void)
{
    return;
}

__attribute__ ((weak))
void udma_over_handler(void)
{
    return;
}

__attribute__ ((weak))
void acc_den_handler(void)
{
    return;
}

__attribute__ ((weak))
void cnt32k_compare_handler(void)
{
    return;
}

__attribute__ ((weak))
void adc_handler(void)
{
    return;
}

__attribute__ ((weak))
void dac_handler(void)
{
    return;
}

__attribute__ ((weak))
void i2c_handler(void)
{
    return;
}

__attribute__ ((weak))
void uart0_handler(void)
{
    return;
}

__attribute__ ((weak))
void uart1_handler(void)
{
    return;
}

__attribute__ ((weak))
void gpio_handler(void)
{
    return;
}

__attribute__ ((weak))
void spim_watermask_handler(void)
{
    return;
}

__attribute__ ((weak))
void spim_over_handler(void)
{
    return;
}


void timer0_over_handler(void)
{
    timer_int_clear_pending(UC_TIMER0, TIMER_IT_OVF);
    rt_kprintf("timer0 overflow interrupt!!!\r\n");
    
    return;
}

__attribute__ ((weak))
void timer0_compare_handler(void)
{
    return;
}

__attribute__ ((weak))
void timer1_over_handler(void)
{
    return;
}

__attribute__ ((weak))
void timer1_compare_handler(void)
{
    return;
}

static void (*g_interrupt_vector[32])(void) =
{
    rtc_handler,    //RTC interrupt
    cce_handler,    //CCE interrupt
    pls_cnt_compare_handler,    //pulse counter compare interrupt
    udma_over_handler,    //udma over interrupt
    acc_den_handler,    //access denied interrupt
    cnt32k_compare_handler,    //counter 32k compare interrupt
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    RT_NULL,    //reserved
    adc_handler,    //ADC interrupt
    dac_handler,    //DAC interrupt
    i2c_handler,    //I2C interrupt
    uart0_handler,    //UART0 interrupt
    uart1_handler,    //UART1 interrupt
    gpio_handler,    //GPIO interrupt
    spim_watermask_handler,    //SPIM rd/wr watermark interrupt
    spim_over_handler,    //SPIM transport over interrupt
    timer0_over_handler,    //TIMER0 over interrupt
    timer0_compare_handler,    //TIMER0 compare interrupt
    timer1_over_handler,    //TIMER1 over interrupt
    timer1_compare_handler,    //TIMER1 compare interrupt
};

void default_int_handler(void)
{
    uint32_t mcause;
    void (*int_func)(void) = RT_NULL;

    csrr(mcause, mcause);
    mcause &= 0xff;

    if (mcause > 31)
    {
        return;
    }

    int_func = g_interrupt_vector[mcause];
    if (int_func != RT_NULL)
    {
        int_func();
    }

    ICP = (1 << mcause);
}
