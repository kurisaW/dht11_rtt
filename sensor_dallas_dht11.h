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

#ifndef __SENSOR_DALLAS_DHT11_H__
#define __SENSOR_DALLAS_DHT11_H__

#include <rtthread.h>

#ifdef RT_USING_SENSOR

#if RT_VER_NUM >= 0x50000
    #include "drivers/sensor.h"
    #define rt_size_t rt_ssize_t
#else
    #include "sensor.h"
#endif

struct dht11_device
{
    rt_base_t pin;
    rt_mutex_t lock;
};
typedef struct dht11_device *dht11_device_t;

int rt_hw_dht11_init(const char *name, struct rt_sensor_config *cfg);

#endif

#endif /* __SENSOR_DALLAS_DHT11_H__ */
