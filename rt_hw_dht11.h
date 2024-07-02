/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author          Notes
 * 2019-08-01     LuoGong         the first version.
 * 2019-08-15     MurphyZhao      add lock and modify code style
 * 2024-07-02     KurisaW         sensor mode separation
 *
 */

#ifndef __RT_HW_DHT11_H__
#define __RT_HW_DHT11_H__

#include <rtthread.h>

#define CONNECT_SUCCESS  0
#define CONNECT_FAILED   1

uint8_t dht11_init(rt_base_t pin);
uint8_t dht11_read_Data(rt_base_t pin,uint8_t *temp,uint8_t *humi);
int32_t dht11_get_temperature(rt_base_t pin);

#endif /* __RT_HW_DHT11_H__ */