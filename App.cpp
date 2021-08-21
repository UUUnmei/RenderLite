#include "App.h"
#include "Math/Math.h"
#include <string>
#include <iomanip>

App::App()
	: wnd(800, 600, "Main Window")
{
	//wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(1, 1, 1)));
	wnd.Gfx().mode = Graphics::RenderMode::FILLEDTRIANGLE;
}

int App::Go() {

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
	}
}

void App::DoFrame()
{
	
	//float d = std::sin(timer.Peek()) / 2 + 0.5f;
	//int c = d *  255;
	//wnd.Gfx().clear_buffer(c<<16 |c << 8 | 255);

	wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(1, 1, 1)));


	//wnd.Gfx().draw_line(
	//	Vec2i(wnd.GetWidth() / 2, wnd.GetHeight() / 2),
	//	Vec2i(wnd.mouse.GetPos()),
	//	Math::vec_to_color(Vec3f(0, 0, 0)));

	wnd.Gfx().DrawTriangle(timer.Peek());
	

	wnd.Gfx().draw();

}

void App::update_fps() {
	static float last = 0;
	float now = timer.Peek();

	wnd.SetTitle(std::to_string(1.0 / (now - last)));

	last = now;
}