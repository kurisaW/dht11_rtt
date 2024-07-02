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

#include <rtdevice.h>
#include <rthw.h>
#include "board.h"
#include <stdint.h>
#include "rt_hw_dht11.h"

#define SENSOR_DEBUG
#define DBG_TAG               "sensor.dht11"

#ifdef SENSOR_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_ERROR
#endif /* SENSOR_DEBUG */
#include <rtdbg.h>

#ifndef RT_USING_PIN
#error "Please enable RT_USING_PIN"
#endif

#ifndef rt_hw_us_delay
rt_weak void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t delta;

    us = us * (SysTick->LOAD / (1000000 / RT_TICK_PER_SECOND));
    delta = SysTick->VAL;

    while (delta - SysTick->VAL < us) continue;
}
#endif

static void dht11_reset(rt_base_t pin)
{
    rt_pin_mode(pin, PIN_MODE_OUTPUT);

    rt_pin_write(pin, PIN_LOW);
    rt_thread_mdelay(20);               /* 20ms */

    rt_pin_write(pin, PIN_HIGH);
    rt_hw_us_delay(30);                 /* 30us*/
}

static uint8_t dht11_check(rt_base_t pin)
{
    uint8_t retry = 0;
    rt_pin_mode(pin, PIN_MODE_INPUT);

    while (rt_pin_read(pin) && retry < 100)
    {
        retry++;
        rt_hw_us_delay(1);
    }

    if(retry >= 100)
    {
        return CONNECT_FAILED;
    }

    retry = 0;
    while (!rt_pin_read(pin) && retry < 100)
    {
        retry++;
        rt_hw_us_delay(1);
    };

    if(retry >= 100)
    {
        return CONNECT_FAILED;
    }

    return CONNECT_SUCCESS;
}

static uint8_t dht11_read_bit(rt_base_t pin)
{
	uint8_t retry = 0;
	while (rt_pin_read(pin) && retry < 100)
    {
        retry++;
        rt_hw_us_delay(1);
    }
	retry = 0;

	while (!rt_pin_read(pin) && retry < 100)
    {
        retry++;
        rt_hw_us_delay(1);
    }

	rt_hw_us_delay(40);
	if(rt_pin_read(pin))
		return 1;
    return 0;
}

static uint8_t dht11_read_byte(rt_base_t pin)
{
    uint8_t i, dat = 0;

    for (i = 1; i <= 8; i++)
    {
        dat <<= 1;
		dat |= dht11_read_bit(pin);
    }

    return dat;
}

uint8_t dht11_read_Data(rt_base_t pin,uint8_t *temp,uint8_t *humi)
{
	uint8_t i, buf[5];
	dht11_reset(pin);

	if(dht11_check(pin) == 0)
	{
		for(i=0; i<5; i++) /* read 40 bits */
		{
			buf[i] = dht11_read_byte(pin);
		}

		if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
		{
			*humi = buf[0];
			*temp = buf[2];
		}
	}
    else
    {
        return 1;
    }

	return 0;	
}

uint8_t dht11_init(rt_base_t pin)
{
    uint8_t ret = 0;

    dht11_reset(pin);
    ret = dht11_check(pin);
    if (ret != 0)
    {
        dht11_reset(pin);
        ret = dht11_check(pin);
    }

    return ret;
}

int32_t dht11_get_temperature(rt_base_t pin)
{
    static int32_t temOLD = 0;
    uint8_t humi=0, temp = 0;
    int32_t temNEW;

    dht11_read_Data(pin, &temp, &humi);

    temNEW = (humi << 16)|(temp<<0);

    if((temNEW != temOLD) && (temNEW !=0))
    {
        temOLD = temNEW;
    }
    return temOLD;
}