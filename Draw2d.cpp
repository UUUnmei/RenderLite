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



struct Rect {
	// 按逆时针给出
	std::vector<Vec2i> points;
	Rect() {
		// 方便演示，不用整个窗口了，画个小矩形
		points.push_back(Vec2i(150, 150));
		points.push_back(Vec2i(150, 450));
		points.push_back(Vec2i(650, 450));
		points.push_back(Vec2i(650, 150));
	}
};

struct Poly {
	std::vector<Vec2i> points;
	Poly() {
		// 未裁剪的多边形，写死的
		points.push_back(Vec2i(41, 193));
		points.push_back(Vec2i(305, 56));
		points.push_back(Vec2i(328, 242));
		points.push_back(Vec2i(498, 91));
		points.push_back(Vec2i(722, 290));
		points.push_back(Vec2i(429, 538));
		points.push_back(Vec2i(425, 393));
		points.push_back(Vec2i(78, 501));
	}
};

Rect rect;
Poly polygon;


void Draw2d::DoFrame()
{
	static const uint32_t black = Math::vec_to_color(Vec3f(0, 0, 0));
	static const uint32_t red = Math::vec_to_color(Vec3f(1, 0, 0));
	//wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(1.0f, 1.0f, 1.0f)));

	// 初始值
	draw_base_polygon(rect.points, black);
	draw_base_polygon(polygon.points, black);

	//if (wnd.mouse.LeftIsPressed()) {
	//	auto p = wnd.mouse.GetPos();
	//	std::string s = std::to_string(p.first) + ", " + std::to_string(p.second) + "\n";
	//	OutputDebugString(s.c_str());
	//}
	// points
	//41, 193
	//305, 56
	//328, 242
	//498, 91
	//722, 290
	//429, 538
	//425, 393
	//78, 501

	// 裁剪后
	Sutherland_Hodgeman(polygon.points, rect.points);
	draw_base_polygon(polygon.points, red);

	
	wnd.Gfx().draw();
}

void Draw2d::handle_kbd_mouse()
{
	if (wnd.kbd.KeyIsPressed(VK_RETURN)) {
		// 按回车键截图
		wnd.Gfx().save_as_bmp_file();
	}

	// 获取点击的位置，突然觉得这个鼠标的封装不太好使。。。
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
	// 暂定长轴在x，短轴在y
	// 第一个点是椭圆中心，第二个确定长轴、短轴
	// 半长轴a取横坐标之差，半短轴b取纵坐标之差
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

void Draw2d::show_draw_lemniscate()
{
	if (q.size() == 1) {
		wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(1.0f, 1.0f, 1.0f)));
		auto p1 = q[0];
		auto p2 = Vec2i(wnd.mouse.GetPos());
		int a = (p1 - p2).length();
		wnd.Gfx().lemniscate(p1.x, p1.y, a);
	}

	if (q.size() >= 2) {
		auto p1 = q.front();
		q.pop_front();
		auto p2 = q.front();
		q.pop_front();

		int a = (p1 - p2).length();
		wnd.Gfx().lemniscate(p1.x, p1.y, a);
	}
}



void Draw2d::draw_base_polygon(std::vector<Vec2i>& polygon, uint32_t color)
{
	for (int i = 0; i < polygon.size(); ++i) 
		wnd.Gfx().draw_line(polygon[i], polygon[(i + 1) % polygon.size()], color);
}


Vec2i get_intersection(Vec2i a, Vec2i b, Vec2i c, Vec2i d) {
	// 直线ab与cd交点
	//L1(t) = a + (b - a) * t
	//L2(u) = c + (d - c) * u
	//令L1(t) = L2(u)  https://www.zhihu.com/question/19971072

	float t = (float)((c - a).cross_z(d - c)) / (b - a).cross_z(d - c);
	Vec2i ab = b - a;
	Vec2i ret(
		a.x + ab.x * t,
		a.y + ab.y * t
	);
	return  ret;
}

void clip(std::vector<Vec2i>& polygon, Vec2i r1, Vec2i r2) 
{
	std::vector<Vec2i> ret;

	for (int i = 0; i < polygon.size(); ++i) {
		// 对于多边形每条边
		int j = (i + 1) % polygon.size();

		// 用叉乘判断内外关系
		// rect边是逆时针
		bool f1 = (r2 - r1).cross_z(polygon[i] - r1) < 0;  //为真表示第一个点在可见侧  
		bool f2 = (r2 - r1).cross_z(polygon[j] - r1) < 0;  //为真表示第二个点在可见侧

		if (f1 && f2) {
			// 两点都可见 要第二个点
			ret.push_back(polygon[j]);
		}
		else if (f1) {
			// 只有第一个点可见， 要与边界的交点
			ret.push_back(get_intersection(polygon[i], polygon[j], r1, r2));
		}
		else if (f2) {
			// 只有第二个点可见，要交点和第二个点
			ret.push_back(get_intersection(polygon[i], polygon[j], r1, r2));
			ret.push_back(polygon[j]);
		}
		else {
			// 都不可见，弃之
		}

	}

	// 更新顶点集合
	polygon = ret;

}


void Draw2d::Sutherland_Hodgeman(std::vector<Vec2i> &polygon, std::vector<Vec2i> &rect)
{
	for (int i = 0; i < rect.size(); ++i) {
		int j = (i + 1) % rect.size();
		// rect 中的点符合左下右上的顺序
		clip(polygon, rect[i], rect[j]);
	}
}
