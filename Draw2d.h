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

	//����ÿһ֡��message loop
	int Go();

private:

	void DoFrame();
	void handle_kbd_mouse();

	void show_draw_line();
	void show_draw_circle(); // �Ƚ��ѺõĽ���ʽ��Բ
	void show_draw_ellipse(); 
	void show_draw_lemniscate(); // ˫Ŧ��

	void show_polygon_clip();
	// ʵ�ֶ���βü�
	void draw_base_polygon(std::vector<Vec2i>&, uint32_t color);  
	void Sutherland_Hodgeman(std::vector<Vec2i> &polygon, std::vector<Vec2i> &rect);

	void show_perspective2();  // ����͸��


	Window wnd;

	std::deque<Vec2i> q;
};

#endif // DRAW_2D_H