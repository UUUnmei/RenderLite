#pragma once
#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include "ChiliWin.h"
#include "Math/Math.h"

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
	HWND screen_handle;		// ������ HWND
	HDC screen_dc;			// ���׵� HDC
	HBITMAP screen_hb;		// DIB
	HBITMAP screen_ob;		// �ϵ� BITMAP
	uint32_t* framebuffer;		// frame buffer

};

#endif // !GRAPHICS_H

