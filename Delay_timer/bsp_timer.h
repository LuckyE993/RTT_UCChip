#ifndef _BSP_TIMER_H_
#define _BSP_TIMER_H_

#include "uc_timer.h"
#include "uc_int.h"
#define Delay_ms(x) Delay_us(1000*x)
void Delay_us(uint32_t us);

#endif // _BSP_TIMER_H_