#include "App.h"
#include "Math/Math.h"
#include <string>
#include <iomanip>

App::App()
	: wnd(800, 600, "Main Window")
{	}

void App::Initial()
{
	wnd.Gfx().mode = Graphics::RenderMode::FILLEDTRIANGLE;
	
	//objects.emplace_back(std::make_unique<Object>("obj/diablo3_pose.obj"));
	//objects.emplace_back(std::make_unique<Object>("obj/african_head.obj"));
	objects.emplace_back(std::make_unique<Object>("obj/spot.obj"));

}

int App::Go() {
	
	Initial();

	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		
		// execute the game logic
		DoFrame();
		
		update_fps();

		if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
			// 切换显示模式，
			// 在线框模式下，可能由于帧数较高，每帧过短，按下一次空格可能被识别多次，导致切换不成功或者是有闪烁。。。。
			if (wnd.Gfx().mode == Graphics::RenderMode::WIREFRAME)
				wnd.Gfx().mode = Graphics::RenderMode::FILLEDTRIANGLE;
			else
				wnd.Gfx().mode = Graphics::RenderMode::WIREFRAME;
		}
		else if (wnd.kbd.KeyIsPressed(VK_RETURN)) {
			// 按回车键截图
			wnd.Gfx().save_as_bmp_file();
		}
	}
}

void App::DoFrame()
{

	wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(1, 1, 1)));

	for (const auto& p : objects)
		wnd.Gfx().draw_object(*p);

	wnd.Gfx().draw();

}

void App::update_fps() {
	static float last = 0;
	float now = timer.Peek();

	wnd.SetTitle(std::to_string(1.0 / (now - last)));

	last = now;
}