#pragma once
#ifndef APP_H
#define APP_H

#include "Window.h"
#include "Timer.h"

#include <memory>
#include "Object.h"

class App {
public:
	App();

	void Initial();

	//����ÿһ֡��message loop
	int Go();

private:

	//std::vector<std::unique_ptr<Object>> objects;
	std::unique_ptr<Object> objects;

	void DoFrame();
	void update_fps();
	void handle_kbd_mouse();
	Window wnd;
	Timer timer;
};

#endif // !APP_H
