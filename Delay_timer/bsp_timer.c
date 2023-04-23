#include "bsp_timer.h"
void Delay_us(uint32_t us)
{
    TIMER_CFG_Type cfg;
    cfg.pre = 7;
    cfg.cnt = 0xffffffffU - ((SYSTEM_CLK/(cfg.pre+1))*us)/1000000;//
    cfg.cmp = 0;//It must be 0 in this mode.
    timer_init(UC_TIMER0, &cfg);
    timer_enable(UC_TIMER0);
    timer_int_enable(UC_TIMER0, TIMER_IT_OVF);
    int_enable();
}
