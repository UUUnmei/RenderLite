#include "App.h"
#include "Math/Math.h"
#include "GraphicsDiscriptor.h"
#include <string>
#include <iomanip>

App::App()
	: wnd(800, 600, "Main Window"), camera(800, 600, Math::deg2rad(75), (float)800 / 600, 0.1, 1000)
{	}

Vec4f vertex_shader_default(const Vertex& in, const ShadingContext& m, V2F& out) {
	// 一个基本的vertex shader
	out.vtx_mvp = m.mvp * in.vtx.to_vec4();

	out.vtx_view = (m.mv * in.vtx.to_vec4()).to_vec3();
	out.normal = (m.it_mv * in.normal.to_vec4(0)).to_vec3();
	out.texcoord = in.texcoord;

	return out.vtx_mvp;
}
Vec3f pixel_shader_basic_blinn_phong(const V2F& in) {
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
}
Vec3f pixel_shader_grid(const V2F& in) {
	Vec2f uv = in.texcoord * 40;
	float x = std::floor(uv.u);
	float y = std::floor(uv.v);
	float c = (x + y) / 2;
	float frac = c - (int)c;
	return Vec3f(frac, frac, frac) * 2;
}
Vec3f pixel_shader_grid2(const V2F& in) {
	float sgn = sin(10 * in.vtx_view.x) * sin(10 * in.vtx_view.y) * sin(10 * in.vtx_view.z);
	float c = sgn > 0.0f ? 1.0f : 0.0f;
	return Vec3f(c, c, c);
}
Vec3f pixel_shader_circle(const V2F& in) {
	Vec2f uv = in.texcoord - Vec2f(0.5f, 0.5f);
	float c = std::sqrt(uv.u * uv.u + uv.v * uv.v) * 10;
	float frac = c - (int)c;
	return Vec3f(frac, frac, frac);
}

Vec4f vertex_shader_skybox(const Vertex& in, const ShadingContext& m, V2F& out) {
	
	out.vtx_mvp = m.mvp * in.vtx.to_vec4();

	out.vtx_view = (m.mv * in.vtx.to_vec4()).to_vec3();
	out.normal = (m.it_mv * in.normal.to_vec4(0)).to_vec3();
	out.texcoord = in.texcoord;

	out.vtx_model = in.vtx;
	out.vtx_mvp.z = -out.vtx_mvp.w; // 让最后的深度为0（最远）
	return out.vtx_mvp;
}
Vec3f pixel_shader_skybox(const V2F& in) {

	return in.color;
}

void App::Initial()
{
	// 设置显示选项
	GraphicsDiscriptor disc;
	//disc.texwrap = TextureWrapMode::Repeat;
	//disc.sample = SampleMode::Bilinear;
	//disc.MSAA4x = true;
	wnd.Gfx().set_discriptor(disc);
	
	
	// 设定初始的变换矩阵
	// 初步来看旋转需要遵循zyx的顺序 ！否则效果可能不对，，，死锁吧大概
	//objects->transform.set_model( 
	//	Transform3::rotate_z(Math::deg2rad(45)) *
	//	Transform3::rotate_y(Math::deg2rad(45)) * 
	//	Transform3::rotate_x(Math::deg2rad(90))
	//);

	// 设定shader
	//objects->vertex_shader = [](const Vertex& in, const ShadingContext& m, V2F& out) -> Vec4f { ... }
	//objects->pixel_shader = [](const V2F& in) -> Vec3f { ... } 
	//objects.back()->vertex_shader = vertex_shader_default;
	//objects.back()->pixel_shader = pixel_shader_basic_blinn_phong;

	//导入模型
	Texture2D helmet_tex("obj/helmet_basecolor.bmp");
	add_object("obj/helmet.obj", Transform3::rotate_x(Math::deg2rad(90)), helmet_tex);
	add_object("obj/cube.obj", Transform3::translate(5.0f, 0.0f, 0.0f), helmet_tex);
	add_object("obj/plane.obj", Transform3::translate(0.0f, -3.0f, 0.0f) *
		Transform3::scale(10.0f, 10.f, 10.0f) *
		Transform3::rotate_x(Math::deg2rad(-90)));
	objects.back()->pixel_shader = pixel_shader_grid;


	//sky.transform.set_model(Transform3::scale(30.0f, 30.0f, 30.0f));
	//sky.transform.set_view(camera.get_view());
	//sky.transform.set_projection(camera.get_persp());
	//sky.transform.gen_mvp();
	//sky.vertex_shader = vertex_shader_skybox;
	//sky.pixel_shader = pixel_shader_skybox;
	//sky.setup("D:\\Visual Studio\\RenderLite\\obj\\skybox");
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

void App::add_object(const std::string& filename, const Mat4f& model_mat, Texture2D& tex)
{
	objects.emplace_back(std::make_unique<Object>(filename));
	objects.back()->withTexture2D(tex);
	objects.back()->transform.set_model(
		model_mat
	);
	objects.back()->transform.set_view(camera.get_view());
	objects.back()->transform.set_projection(camera.get_persp());
	objects.back()->transform.gen_mvp();
	objects.back()->vertex_shader = vertex_shader_default;   // 两个默认shader
	objects.back()->pixel_shader = pixel_shader_basic_blinn_phong;
}

void App::DoFrame()
{
	wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(0.0823, 0.8901, 0.9450)));
	//wnd.Gfx().clear_buffer(Math::vec_to_color(Vec3f(0, 0, 0)));

	for (const auto& obj : objects)
		wnd.Gfx().draw_object(*obj);

	wnd.Gfx().disc.fc_mode = FaceCullMode::Front;
	//wnd.Gfx().draw_object(sky);
	wnd.Gfx().disc.fc_mode = FaceCullMode::Back;


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

	for (const auto& obj : objects) {
		obj->transform.view = camera.get_view();
		obj->transform.gen_mvp();
	}


	//sky.transform.view = camera.get_view();
	//sky.transform.gen_mvp();
}
