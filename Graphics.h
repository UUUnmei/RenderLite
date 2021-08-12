#pragma once
#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include "ChiliWin.h"

class Graphics {
public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void set_pixel(int x, int y, unsigned int color);
	void clear_buffer(unsigned int color);
	void draw(void);
	//void DrawTriangle(float angle);
	//void DrawCube(float angle, float x, float y);

private:
	
	int width;
	int height;
	HWND screen_handle;		// 主窗口 HWND
	HDC screen_dc;			// 配套的 HDC
	HBITMAP screen_hb;		// DIB
	HBITMAP screen_ob;		// 老的 BITMAP
	unsigned int* framebuffer;		// frame buffer

};

#endif // !GRAPHICS_H

