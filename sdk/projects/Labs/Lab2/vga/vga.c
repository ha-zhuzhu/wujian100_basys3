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
#include "picture_data.h"

// address
#define VGA_RAM_BASE (0x40020000UL) // 0x40020000

// vga parameters
#define PIXEL_PER_ROW 320
#define PIXEL_PER_COL 240
#define PIXEL_PER_BYTE 2
#define ADDR_OFFSET_PER_ROW (PIXEL_PER_ROW / PIXEL_PER_BYTE) // 160
#define BYTE_PER_PACK 4									   // 32-bit pack
#define PIXEL_PER_PACK (PIXEL_PER_BYTE *BYTE_PER_PACK)	   // 8

// Display your picture, parameters should be multiples of 8
void display_picture(int start_x_pos, int start_y_pos, int pic_width, int pic_height, uint32_t *pic_data)
{
	uint32_t start_pixel = start_x_pos + PIXEL_PER_ROW * start_y_pos;
	// static_assert(!(start_pixel % 8));
	// static_assert((start_x_pos + pic_width <= PIXEL_PER_ROW) && (start_y_pos+pic_height<=PIXEL_PER_COL));
	uint32_t addr_offset = start_pixel / PIXEL_PER_BYTE;
	int pixel_x = 0;
	int index = 0; // pic_data index
	while (index < pic_width * pic_height / PIXEL_PER_PACK)
	{
		if (pixel_x >= pic_height) // 换行
		{
			pixel_x = 0;
			addr_offset += ADDR_OFFSET_PER_ROW;
		}
		REG32(VGA_RAM_BASE + addr_offset + pixel_x / PIXEL_PER_BYTE) = pic_data[index];
		pixel_x += 8;
		index++;
	}
}

// Flush canvas with color
void flush_canvas(uint32_t color)
{
	for (int i=0;i<PIXEL_PER_ROW*PIXEL_PER_COL/PIXEL_PER_PACK;i++)
	{
		REG32(VGA_RAM_BASE+i*BYTE_PER_PACK)=color;
	}
}

int main(void)
{
	flush_canvas(0xffffffff);
	display_picture(64, 20, 200, 200, picture_data);
	return 0;
}
