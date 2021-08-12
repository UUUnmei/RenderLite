#pragma once
#ifndef APP_H
#define APP_H

#include "Window.h"
#include "Timer.h"

class App {
public:
	App();

	//����ÿһ֡��message loop
	int Go();

private:
	void DoFrame();
	void update_fps();
	Window wnd;
	Timer timer;
};

#endif // !APP_H
