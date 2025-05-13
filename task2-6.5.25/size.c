#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

#define ABS(x) (((x) > 0) ? (x) : -(x))

#define RESOLUTION_X 320
#define RESOLUTION_Y 240

#define BOX_LEN 2
#define NUM_BOXES 8

#define FALSE 0
#define TRUE 1

#pragma GCC optimize("Os")
	
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
	
void draw_line(int x0, int y0, int x1, int y1, short int linecolor);
void clear_screen();
void plot_pixel(int x, int y, short int line_color);
void swap (int x, int y);
void wait_for_vsync();
void draw_box(int x, int y, short int color);

#define erase_box(x, y) 				draw_box(x, y, 0x0000)
#define erase_line(x0, y0, x1, y1)		draw_line(x0, y0, x1, y1, 0x0000)

volatile int pixel_buffer_start; 

short int colors[] = {WHITE, YELLOW, RED, GREEN, BLUE, CYAN, MAGENTA, GREY, PINK, ORANGE};
short int lineColor [8];
short int boxColor [8];
int x_coords [8];
int y_coords [8];
int x_offset [8];
int y_offset [8];

int main(void)
{
	
	int i;
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
	for (i = 0; i < 8; ++i)
	{
		x_coords[i] = rand() % RESOLUTION_X;
		y_coords[i] = rand() % RESOLUTION_Y;
		lineColor[i] = colors[rand() % 10];
		x_offset[i] = (rand() % 2) * 2 - 1;
		y_offset[i] = (rand() % 2) * 2 - 1;
	} 
    *(pixel_ctrl_ptr + 1) = 0xC8000000; 
    wait_for_vsync();

    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); 
    
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); 
    clear_screen(); 

    while (1)
    {
        for (i = 0; i < 8; i++)
		{
			draw_box((x_coords[i]-x_offset[i]), (y_coords[i]-y_offset[i]), 0x0000);
			draw_line((x_coords[i]-x_offset[i]), (y_coords[i]-y_offset[i]), (x_coords[(i+1)%8]-x_offset[(i+1)%8]), (y_coords[(i+1)%8]-y_offset[(i+1)%8]), 0);
		}
					   
		for (i = 0; i < 8; i++)
		{
			if (x_coords[i] == 0)
			{
				x_offset[i] = 1;
			}
			if (x_coords[i] == RESOLUTION_X)
			{
				x_offset[i] = -1;
			}
			if (y_coords[i] == 0)
			{
				y_offset[i] = 1;
			}
			if (y_coords[i] == RESOLUTION_Y)
			{
				y_offset[i] = -1;
			}
			
			x_coords[i] = x_coords[i] + x_offset[i];
			y_coords[i] = y_coords[i] + y_offset[i];
		
		} 
	    for(i = 0; i < 8; i++)
		{
            draw_box(x_coords[i], y_coords[i], YELLOW);
            draw_line(x_coords[i], y_coords[i], x_coords[(i+1)%8], y_coords[(i+1)%8], lineColor[i]);
        }		   
        wait_for_vsync(); 
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); 
    }
}

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void swap (int x, int y)
{
    int temp;
    temp = x;
    x = y;
    y = temp;
}

void draw_line(int x0, int y0, int x1, int y1, short int linecolor) 
{
    int y_step;

    bool is_steep = (abs(y1-y0)) > (abs(x1-x0));
    if (is_steep)
    {
        swap(x0, y0);
        swap(x1, y1);
    }
    if (x0>x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    int deltaX = x1 - x0;
    int deltaY = abs(y1 - y0);
    int error = -(deltaX / 2);
    int y = y0;
    if (y0 < y1)
    {
        y_step = 1;
    }
    else
    {
        y_step = -1;
    }
	int x;
    for (x = x0; x < x1; x++)
    {
        if (is_steep)
        {
            plot_pixel(y, x, linecolor);
        }
        else
        {
            plot_pixel(x, y, linecolor);
        }
        error = error + deltaY;
        if (error > 0)
        {
            y = y + y_step;
            error = error - deltaX;
        }

    } 

} 

void clear_screen()
{
    int x, y;
    for (x = 0; x < RESOLUTION_X; x++)       
    {
        for (y = 0; y < RESOLUTION_Y; y++)  
        {
            plot_pixel(x, y, 0x0000);        
        }
    }
}

void wait_for_vsync()	
{
	volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    register int status;
	*pixel_ctrl_ptr = 1;
	status = *(pixel_ctrl_ptr + 3);
	while ((status &1) != 0)
	{
		status = *(pixel_ctrl_ptr + 3);
	}
	return;
} 


void draw_box(int x, int y, short int color)
{
	int i, j;
	for (i = (x-1); (i < (x+1)) && (i < RESOLUTION_X) && (i > 0); i++)
	{
		for (j = (y-1); (j < (y+1)) && (j < RESOLUTION_Y) && (j>0); j++)
		{
			plot_pixel(x, y, color);
		} 
	} 
} 
