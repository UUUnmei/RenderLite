#pragma once
#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include "ChiliWin.h"
#include "Math/Math.h"
#include "bmp.h"
#include <memory>

#include "Object.h"
#include "GraphicsDiscriptor.h"


class Graphics {
public:

	GraphicsDiscriptor disc;

public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

public:
	void set_pixel(int x, int y, uint32_t color);
	void set_pixel_unsafe(int x, int y, uint32_t color);
	void set_pixel(int idx, uint32_t color);
	void clear_buffer(uint32_t color);
	void draw(void);
	void draw_line(const Vec2i& start, const Vec2i& end, uint32_t color);
	template<typename T>
	void draw_line(const Vec3<T>& start, const Vec3<T>& end, uint32_t color);
	void DrawTriangle(float angle);
	//void DrawCube(float angle, float x, float y);
	void draw_object(Object& obj);

	void save_as_bmp_file(const char* filename = "output.bmp");



private:
	
	Bitmap bmp_manager;

	int width;
	int height;
	HWND screen_handle;		// 主窗口 HWND
	HDC screen_dc;			// 配套的 HDC
	HBITMAP screen_hb;		// DIB
	HBITMAP screen_ob;		// 老的 BITMAP
	uint32_t* framebuffer;		// frame buffer
	std::unique_ptr<float[]> depthbuffer; 

};

#endif // !GRAPHICS_H

