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

	Window wnd;

	std::deque<Vec2i> q;
};

#endif // DRAW_2D_H