/* Copyright 2016 Pedro Falcato

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
/**************************************************************************
 * 
 * 
 * File: tty.c
 * 
 * Description: Contains the text terminal initialization and manipulation code
 * 
 * Date: 30/1/2016
 * 
 * 
 **************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/portio.h>
#include <kernel/vga.h>
#include <kernel/tty.h>
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void TTY::Init(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void TTY::SetColor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
void terminal_scroll()
{
	for(unsigned int i = 0; i < VGA_HEIGHT; i++){
		for (unsigned int m = 0; m < VGA_WIDTH; m++){
			terminal_buffer[i * VGA_WIDTH + m] = terminal_buffer[(i + 1) * VGA_WIDTH + m];
		}
	}
}
void TTY::PutChar(char c)
{
	if(c == '\n'){
		terminal_column = 0;
		if(terminal_row +1 == VGA_HEIGHT)
			terminal_scroll();
		else
		terminal_row++;
		
		UpdateCursor();
		return;
	}
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if ( ++terminal_column == VGA_WIDTH )
	{
		terminal_column = 0;
		if ( ++terminal_row == VGA_HEIGHT )
		{
			terminal_scroll();
		}
	}
	UpdateCursor();
}
void TTY::UpdateCursor()
{
	uint16_t position=(terminal_row*80) + terminal_column;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(position&0xFF));//Cursor low
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char)((position>>8)&0xFF));
}
void TTY::Write(const char* data, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		PutChar(data[i]);
}

void TTY::WriteString(const char* data)
{
	Write(data, strlen(data));
}