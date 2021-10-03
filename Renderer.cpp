#include<math.h>
float render_scale = 0.01f;

void render_background() 
{
	unsigned int* pixel = (unsigned int*)render_info.buffer_memory;
	for (int y = 0; y < render_info.height * render_info.width; y++)
	{
		*pixel++ = 0xff0055;
	}
}

void clear_screen()
{
	unsigned int* pixel = (unsigned int*)render_info.buffer_memory;
	for (int y = 0; y < render_info.height * render_info.width; y++)
	{
		*pixel++ = 0xff0055;
	}
}

inline int clamp(int min, int val, int max)
{
	if (val < min)return min;
	if (val > max)return max;
	return val;
}

void draw_rectangle_in_pixels(int x0, int y0, int x1, int y1, unsigned int color) 
{
	x0 = clamp(0, x0, render_info.width);
	x1 = clamp(0, x1, render_info.width);
	y0 = clamp(0, y0, render_info.height);
	y1 = clamp(0, y1, render_info.height);
	for (int y = y0; y < y1; y++) 
	{
		unsigned int* pixel = (unsigned int*)render_info.buffer_memory + x0 + (static_cast <unsigned __int64>(y) * render_info.width);
		for (int x = x0; x < x1; x++) 
		{
			*pixel++ = color;
		}
	}
}

void draw_circle_in_pixels(int x, int y, int radius, unsigned int color)
{
	unsigned int* pixel = (unsigned int*)render_info.buffer_memory;
	for (int y1 = 0; y1 < render_info.height; y1++) 
	{
		for (int x1 = 0; x1 < render_info.width; x1++) 
		{
			double a = pow((double)((double)x - (double)x1), 2);
			double b = pow((double)((double)y - (double)y1), 2);
			if (sqrt(a + b) <= radius)
			{
				*(pixel) = color;
			}
			//C6269 - dereference ignored!	
			*pixel++;
		}
	}
}

void draw_circle(float x, float y, float radius, unsigned int color) 
{
	x *=  render_info.width * render_scale;
	y *=  render_info.height * render_scale;

	radius *= render_info.height * render_scale;


	x += render_info.width / 2.f;
	y += render_info.height / 2.f;

	draw_circle_in_pixels((int)x, (int)y, (int)radius, color);
}

void draw_rectangle(float x, float y, float half_size_x, float half_size_y, unsigned int color) 
{
	x *= render_info.width * render_scale;
	y *= render_info.height * render_scale;
	half_size_x *= render_info.height * render_scale;
	half_size_y *= render_info.height * render_scale;

	x += render_info.width / 2.f;
	y += render_info.height / 2.f;
	
	draw_rectangle_in_pixels((int)(x - half_size_x), (int)(y - half_size_y), (int)(x + half_size_x), (int)(y + half_size_y), color);
}