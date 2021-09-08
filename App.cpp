#include "App.h"
#include "Math/Math.h"
#include "GraphicsDiscriptor.h"
#include <string>
#include <iomanip>

App::App()
	: wnd(800, 600, "Main Window"), camera(800, 600, Math::deg2rad(45), (float)800 / 600, 0.1, 100)
{	}

void App::Initial()
{
	// 设置显示选项
	GraphicsDiscriptor disc;
	disc.texwrap = TextureWrapMode::Repeat;
	//disc.sample = SampleMode::Bilinear;
	//disc.MSAA4x = true;
	wnd.Gfx().set_discriptor(disc);
	
	//导入模型

	//objects = std::make_unique<Object>("obj/spot.obj", "obj/spot_texture256.bmp");
	objects = std::make_unique<Object>("obj/helmet.obj", "obj/helmet_basecolor.bmp");


	// 设定初始的变换矩阵
	objects->transform.set_model( // 初步来看需要遵循zyx的顺序 ！否则效果不对，，，
		//Transform3::rotate_z(Math::deg2rad(45)) *
		//Transform3::rotate_y(Math::deg2rad(45)) * 
		Transform3::rotate_x(Math::deg2rad(90))
	);
	objects->transform.set_view(
		camera.get_view()
	);
	objects->transform.set_projection(
		camera.get_persp()
	);
	objects->transform.gen_mvp();

	// 设定shader
	objects->vertex_shader = [](const Vertex& in, const ShadingContext& m, V2F& out) -> Vec4f {
		// 一个基本的vertex shader
		out.vtx_mvp = m.mvp * in.vtx.to_vec4();
		out.rhw = 1.0f / out.vtx_mvp.w;  // 为了插值进行预处理

		out.vtx_view = (m.mv * in.vtx.to_vec4()).to_vec3();
		out.normal = (m.it_mv * in.normal.to_vec4(0)).to_vec3();
		out.texcoord = in.texcoord;
		
		return out.vtx_mvp;
	};
	objects->pixel_shader = [](const V2F& in) -> Vec3f {
		//Vec2f uv = in.texcoord * 10;
		//float x = std::floor(uv.u);
		//float y = std::floor(uv.v);
		//float c = (x + y) / 2;
		//float frac = c - (int)c;
		//return Vec3f(frac, frac, frac) * 2;


		Vec3f tex_color = in.color;

		Vec3f ka = Vec3f(0.005, 0.005, 0.005);
		Vec3f kd = tex_color;
		Vec3f ks = Vec3f(0.7937, 0.7937, 0.7937);

		struct Light {
			Vec3f pos;
			float intensity;
		};

		static Light l1 = Light{ {20, 20, 20}, 500 };
		static Light l2 = Light{ {-20, 20, 0}, 500 };

		static std::vector<Light> lights{ l1, l2 };
		Vec3f amb_light_intensity{ 10, 10, 10 };
		Vec3f eye_pos{ 0, 0, 10 };

		int p = 150;

		Vec3f color = tex_color;
		Vec3f point = in.vtx_view;
		Vec3f normal = in.normal;

		Vec3f result_color = { 0, 0, 0 };
		for (auto& light : lights)
		{
			float r2 = (light.pos - point).length_squared();
			Vec3f I = (light.pos - point).normalize();
			Vec3f V = (eye_pos - point).normalize();

			Vec3f diffuse = kd * (light.intensity / r2) * std::max(0.0f, I.dot(normal));
			Vec3f specular = ks * (light.intensity / r2) * Math::qpow(std::max(0.0f, normal.dot((I + V).normalize())), p);
			Vec3f ambient = ka * amb_light_intensity.x;

			result_color +=
				diffuse
				+ specular
				+ ambient;

		}

		return result_color;
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
	//wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(0, 0, 0)));

	//for (const auto& p : objects)
	//	wnd.Gfx().draw_object(*p);

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

	wnd.Gfx().update_by(wnd.kbd);

	// 鼠标控制摄像机
	camera.update(wnd.mouse, wnd.kbd);

	objects->transform.view = camera.get_view();
	objects->transform.gen_mvp();
}
