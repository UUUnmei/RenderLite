#pragma once
#ifndef DRAW_2D_H
#define DRAW_2D_H

#include "Window.h"
#include <deque>

class Draw2d
{
public:
	Draw2d();

	void Initial();

	//管理每一帧和message loop
	int Go();

private:

	void DoFrame();
	void handle_kbd_mouse();

	void show_draw_line();
	void show_draw_circle(); // 比较友好的交互式画圆
	void show_draw_ellipse(); 
	void show_draw_lemniscate(); // 双纽线

	void show_polygon_clip();
	// 实现多边形裁剪
	void draw_base_polygon(std::vector<Vec2i>&, uint32_t color);  
	void Sutherland_Hodgeman(std::vector<Vec2i> &polygon, std::vector<Vec2i> &rect);

	void show_perspective2();  // 二点透视


	Window wnd;

	std::deque<Vec2i> q;
};

#endif // DRAW_2D_H