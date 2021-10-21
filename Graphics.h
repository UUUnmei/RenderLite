#pragma once
#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include "ChiliWin.h"
#include "Math/Math.h"
#include "bmp.h"
#include <memory>

#include "Object.h"
#include "GraphicsDiscriptor.h"
#include "Keyboard.h"
#include "SkyBox.h"

class Graphics {


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
	//void DrawTriangle(float angle);
	//void DrawCube(float angle, float x, float y);
	void draw_object(Object& obj);
	//void draw_sky(SkyBox& sky, Vec3f& eye_pos);

	void save_as_bmp_file(const char* filename = "output.bmp");
	void set_discriptor(GraphicsDiscriptor& d);
	void update_by(Keyboard& k);

	GraphicsDiscriptor disc;

public: //2d
	void line_DDA(int x0, int y0, int x1, int y1);
	void line_Bresenham(int x0, int y0, int x1, int y1);
	void line_Improved_Bresenham(int x0, int y0, int x1, int y1);
	// ���¾�����Bresenham�е㷨
	void circle(int ox, int oy, int r);
	void ellipse(int ox, int oy, int a, int b); // aΪ�볤�ᣬbΪ����ᣬ��ʱĬ�ϳ�����x����
	void lemniscate(int ox, int oy, int a);  // ͬ����������x����

private:
	
	Bitmap bmp_manager;

	int width;
	int height;
	HWND screen_handle;		// ������ HWND
	HDC screen_dc;			// ���׵� HDC
	HBITMAP screen_hb;		// DIB
	HBITMAP screen_ob;		// �ϵ� BITMAP
	uint32_t* framebuffer;		// frame buffer
	std::unique_ptr<float[]> depthbuffer; 


};

#endif // !GRAPHICS_H

