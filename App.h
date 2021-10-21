#pragma once
#ifndef APP_H
#define APP_H

#include "Window.h"
#include "Timer.h"
#include "OrbitCamera.h"
#include "Object.h"
#include "SkyBox.h"

#include <memory>
class App {
public:
	App();

	void Initial();

	//管理每一帧和message loop
	int Go();

	// 添加object的辅助函数
	void add_object(const std::string& filename, const Mat4f& model_mat, Texture2D& tex = Texture2D{});

private:

	std::vector<std::unique_ptr<Object>> objects;
	//std::unique_ptr<Object> objects;
	OrbitCamera camera;
	SkyBox sky;

public:
	void DoFrame();
	void update_fps();
	void handle_kbd_mouse();
	Window wnd;
	Timer timer;
	
};

#endif // !APP_H
