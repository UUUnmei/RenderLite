#include "Draw2d.h"


Draw2d::Draw2d()
	: wnd(800, 600, "Main Window")
{ }

void Draw2d::Initial()
{
	wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(1.0f, 1.0f, 1.0f)));
}

int Draw2d::Go()
{
	Initial();

	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		handle_kbd_mouse();

		DoFrame();
	}
}

void Draw2d::DoFrame()
{
	//wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(1.0f, 1.0f, 1.0f)));
	
	// ����
	//show_draw_line();
	// ��Բ
	//show_draw_circle();
	// ��Բ
	show_draw_ellipse();


	wnd.Gfx().draw();
}

void Draw2d::handle_kbd_mouse()
{
	if (wnd.kbd.KeyIsPressed(VK_RETURN)) {
		// ���س�����ͼ
		wnd.Gfx().save_as_bmp_file();
	}

	// ��ȡ�����λ�ã�ͻȻ����������ķ�װ��̫��ʹ������
	while (!wnd.mouse.IsEmpty()) {
		const auto e = wnd.mouse.Read();
		if (!e.has_value()) continue;
		switch (e.value().GetType()) {
		case Mouse::Event::Type::LPress:
			q.push_back(Vec2i(e.value().GetPos()));
			break;
		}
	}

}

void Draw2d::show_draw_line()
{
	auto f = &Graphics::line_DDA;
	//auto f = &Graphics::line_Bresenham;
	//auto f = &Graphics::line_Improved_Bresenham;
	

	if (q.size() == 1) {
		wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(1.0f, 1.0f, 1.0f)));
		auto a = q[0];
		auto b = Vec2i(wnd.mouse.GetPos());
		
		(wnd.Gfx().*f)(a.x, a.y, b.x, b.y);
	}

	if (q.size() >= 2) {
		auto a = q.front();
		q.pop_front();
		auto b = q.front();
		q.pop_front();

		(wnd.Gfx().*f)(a.x, a.y, b.x, b.y);
	}
}

void Draw2d::show_draw_circle()
{
	if (q.size() == 1) {
		wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(1.0f, 1.0f, 1.0f)));
		auto a = q[0];
		auto b = Vec2i(wnd.mouse.GetPos());
		int r = (a - b).length();
		wnd.Gfx().circle(a.x, a.y, r);
	}

	if (q.size() >= 2) {
		auto a = q.front();
		q.pop_front();
		auto b = q.front();
		q.pop_front();

		int r = (a - b).length();
		wnd.Gfx().circle(a.x, a.y, r);
	}
}

void Draw2d::show_draw_ellipse()
{
	// �ݶ�������x��������y
	// ��һ��������Բ���ģ��ڶ���ȷ�����ᡢ����
	// �볤��aȡ������֮������bȡ������֮��
	if (q.size() == 1) {
		wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(1.0f, 1.0f, 1.0f)));
		auto p1 = q[0];
		auto p2 = Vec2i(wnd.mouse.GetPos());
		int a = abs(p1.x - p2.x);
		int b = abs(p1.y - p2.y);
		wnd.Gfx().ellipse(p1.x, p1.y, a, b);
	}

	if (q.size() >= 2) {
		auto p1 = q.front();
		q.pop_front();
		auto p2 = q.front();
		q.pop_front();
		int a = abs(p1.x - p2.x);
		int b = abs(p1.y - p2.y);
		wnd.Gfx().ellipse(p1.x, p1.y, a, b);
	}

}
