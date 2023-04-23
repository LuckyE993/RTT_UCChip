/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-5      SummerGift   first version
 */

#ifndef __DRV_SPI_H_
#define __DRV_SPI_H_

#include <rtthread.h>
#include "rtdevice.h"
#include <rthw.h>
#include "uc_pulpino.h"

rt_err_t rt_hw_spi_device_attach(const char* bus_name, const char* device_name, GPIO_TypeDef* cs_gpiox, uint16_t cs_gpio_pin);

#endif /*__DRV_SPI_H_ */
