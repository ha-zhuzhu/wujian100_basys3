/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     example_gpio.c
 * @brief    the main function for the GPIO driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#include <stdio.h>
#include "soc.h"
#include "drv_gpio.h"
#include "pin_name.h"
#include "pin.h"
#include "io.h"

#define MY_IO_BASE (0x40010000UL)	// 0x40010000
#define VGA_RAM_BASE (0x40020000UL) // 0x40020000
// #define
#define PIXEL_PER_ROW 320
#define PIXEL_PER_COL 240
#define PIXEL_PER_BYTE 2
#define ADDR_OFFSET_PER_ROW (PIXEL_PER_ROW / PIXEL_PER_BYTE) // 160
#define BYTE_PER_PACK 4									   // 32-bit pack
#define PIXEL_PER_PACK (PIXEL_PER_BYTE *BYTE_PER_PACK)	   // 8

volatile static bool int_flag = 1;

static void gpio_interrupt_handler(int32_t idx)
{
	int_flag = 0;
}

/*****************************************************************************
test_gpio: main function of the gpio test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
// int example_gpio(pin_name_e gpio_pin)
//{
//     gpio_falling_edge_interrupt(gpio_pin);
//     return 0;
// }
gpio_pin_handle_t pin[16];

// initialize 16 gpios to output mode
void example_pin_gpio_init(void)
{
	for (int i = 0; i < 16; i++)
	{
		drv_pinmux_config(i, EXAMPLE_GPIO_PIN_FUNC);
		pin[i] = csi_gpio_pin_initialize(i, gpio_interrupt_handler);
		csi_gpio_pin_config_mode(pin[i], GPIO_MODE_PULLNONE);
		csi_gpio_pin_config_direction(pin[i], GPIO_DIRECTION_OUTPUT);
	}
}

void example_write_gpio(short data)
{
	for (int i = 0; i < 16; i++)
	{
		csi_gpio_pin_write(pin[i], (data & (1 << i)) >> i);
	}
}

int example_gpio(pin_name_e gpio_pin)
{
	example_pin_gpio_init();
	short data = 0;

	volatile int *myio = (volatile int *)MY_IO_BASE;
	*myio = 0x00;
	*myio = 0x1;
	*myio = 0x2;
	*myio = 0x4;

	while (1)
	{
		example_write_gpio(data);
		mdelay(500);
		data++;
	}
	csi_gpio_pin_uninitialize(pin);
	printf("gpio falling_edge test passed!!!\n");
	printf("test gpio successfully\n");
	return 0;
}


int main(void)
{
	//    return example_gpio(EXAMPLE_GPIO_PIN);
	//	vgaram_write_test();
	flush_canvas(0xffffffff);
	display_picture(64, 20, 200, 200, picture_data);
//	vgaram_write_test();
	return 0;
}
