#include "App.h"
#include "Math/Math.h"
#include <string>
#include <iomanip>

App::App()
	: wnd(800, 600, "Main Window")
{	}

void App::Initial()
{
	// 设置显示选项
	wnd.Gfx().mode = Graphics::RenderMode::FILLEDTRIANGLE;
	
	//导入模型
	//objects.emplace_back(std::make_unique<Object>("obj/diablo3_pose.obj"));
	//objects.emplace_back(std::make_unique<Object>("obj/african_head.obj"));
	//objects.emplace_back(std::make_unique<Object>("obj/bunny/bunny.obj"));
	//objects.emplace_back(std::make_unique<Object>("obj/mori_knob/testObj.obj"));  // 主要是下标1、4的mesh
	//objects.emplace_back(std::make_unique<Object>("obj/bun_zipper.ply"));
	//objects.emplace_back(std::make_unique<Object>("obj/spot.obj", "obj/spot_texture.bmp"));
	//objects.emplace_back(std::make_unique<Object>("obj/rock.obj", "obj/rock_texture.bmp"));
	
	//objects = std::make_unique<Object>("obj/spot.obj");
	objects = std::make_unique<Object>("obj/plane.obj", "obj/spot_texture.bmp");


	// 设定初始的变换矩阵
	objects->transform.set_model(
		Transform3::scale(3.0f / 4.0f, 1.0f, 1.0f)
		* Transform3::rotate_x(Math::deg2rad(-45))
	);
	objects->transform.set_view(
		Transform3::view(Vec3f(0, 0, 10), Vec3f(0, 0, -1), Vec3f(0, 1, 0))
	);
	objects->transform.set_projection(
		Transform3::persp(Math::deg2rad(45), 1, 0.1, 100)
	);
	objects->transform.gen_mvp();

	// 设定shader
	objects->vertex_shader = [](const Vertex& in, const ShadingContext& m, V2F& out) -> Vec4f {
		// 一个基本的vertex shader
		out.vtx_mvp = m.mvp * in.vtx.to_vec4();
		out.rhw = 1.0f / out.vtx_mvp.w;  // 为了插值进行预处理

		out.vtx_view = (m.mv * in.vtx.to_vec4()).to_vec3();// *out.rhw;
		out.normal = (m.it_mv * in.normal.to_vec4(0)).to_vec3();// *out.rhw;
		out.texcoord = in.texcoord;// *out.rhw;
		
		return out.vtx_mvp;
	};
	objects->pixel_shader = [&](const V2F& in) -> Vec3f {
		Vec2f uv = in.texcoord * 10;
		float x = std::floor(uv.u);
		float y = std::floor(uv.v);
		float c = (x + y) / 2;
		float frac = c - (int)c;
		return Vec3f(frac, frac, frac) * 2;


		//Vec3f tex_color;
		//if (objects->texture) {
		//	tex_color = objects->get_tex(in.texcoord.x, in.texcoord.y);
		//}
		//else {
		//	tex_color = Vec3f(148.0 / 255, 121.0 / 255, 92.0 / 255);
		//}

		//Vec3f ka = Vec3f(0.005, 0.005, 0.005);
		//Vec3f kd = tex_color;
		//Vec3f ks = Vec3f(0.7937, 0.7937, 0.7937);

		//struct Light {
		//	Vec3f pos;
		//	float intensity;
		//};

		//static Light l1 = Light{ {20, 20, 20}, 500 };
		//static Light l2 = Light{ {-20, 20, 0}, 500 };

		//static std::vector<Light> lights{ l1, l2 };
		//Vec3f amb_light_intensity{ 10, 10, 10 };
		//Vec3f eye_pos{ 0, 0, 10 };

		//int p = 150;

		//Vec3f color = tex_color;
		//Vec3f point = in.vtx_view;
		//Vec3f normal = in.normal;

		//Vec3f result_color = { 0, 0, 0 };
		//for (auto& light : lights)
		//{
		//	float r2 = (light.pos - point).length_squared();
		//	Vec3f I = (light.pos - point).normalize();
		//	Vec3f V = (eye_pos - point).normalize();

		//	Vec3f diffuse = kd * (light.intensity / r2) * std::max(0.0f, I.dot(normal));
		//	Vec3f specular = ks * (light.intensity / r2) * Math::qpow(std::max(0.0f, normal.dot((I + V).normalize())), p);
		//	Vec3f ambient = ka * amb_light_intensity.x;

		//	result_color +=
		//		diffuse
		//		+ specular
		//		+ ambient;

		//}

		//return result_color;
	};

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
		
		handle_kbd_mouse();

		// execute the game logic
		DoFrame();
		
		update_fps();

	}
}

void App::DoFrame()
{
	wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(0.0823, 0.8901, 0.9450)));

	//for (const auto& p : objects)
	//	wnd.Gfx().draw_object(*p);

	// update
	objects->transform.model *= Transform3::rotate_z(Math::deg2rad(1));
	objects->transform.gen_mvp();

	// render
	wnd.Gfx().draw_object(*objects);

	wnd.Gfx().draw();

}

void App::update_fps() {
	static float last = 0;
	float now = timer.Peek();

	wnd.SetTitle(std::to_string(1.0 / (now - last)));

	last = now;
}

void App::handle_kbd_mouse()
{
	if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
		// 切换显示模式，不是很好使
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
