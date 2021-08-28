


#include "Graphics.h"
#include "Exception.h"
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Graphics::Graphics(HWND hWnd, int width, int height)
	:width(width), height(height), screen_handle(hWnd), bmp_manager(width, height)
{
	HDC hDC = GetDC(hWnd);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(hWnd, hDC);

	BITMAPINFO bmpinfo;
	memset(&bmpinfo, 0, sizeof(BITMAPINFO));
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = width;
	bmpinfo.bmiHeader.biHeight = -height;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = height * width * 4;
	//bmpinfo.bmiColors ����̫�������

	screen_hb = CreateDIBSection(screen_dc, &bmpinfo, DIB_RGB_COLORS, (void**)&framebuffer, 0, 0);
	if (screen_hb == NULL) {
		THROW_ERROR("create dib section failed")
	}

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);

	memset(framebuffer, 0, width * height * sizeof(unsigned int));

	depthbuffer = std::make_unique<float[]>(width * height);

	model = std::make_unique<Model>("obj/diablo3_pose.obj");
	//model = std::make_unique<Model>("obj/african_head.obj");


	// Assimp Test
	Assimp::Importer imp;
	auto model = imp.ReadFile("obj/african_head.obj",
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);


}


Graphics::~Graphics()
{	
	SelectObject(screen_dc, screen_ob);
	DeleteDC(screen_dc);
	DeleteObject(screen_ob);
	DeleteObject(screen_hb);
}

void Graphics::set_pixel(int x, int y, uint32_t color)
{
	//std::cout << x << ' ' << y << ' ' << color << '\n';
	if ((unsigned)x >= width || (unsigned)y >= height) return;
	framebuffer[y * width + x] = color;
}

void Graphics::set_pixel_unsafe(int x, int y, uint32_t color)
{
	framebuffer[y * width + x] = color;
}


void Graphics::set_pixel(int idx, uint32_t color)
{
	static int limit = width * height;
	if ((unsigned)idx >= limit) return;
	framebuffer[idx] = color;
}

void Graphics::clear_buffer(uint32_t color)
{
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j) {
			framebuffer[i * width + j] = color;
			depthbuffer[i * width + j] = -std::numeric_limits<float>::infinity();
		}
			

}

void Graphics::draw(void)
{
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
}

// bresenham
// ѧϰ�㷨http://www.idivecat.com/archives/652 
// ʵ�ֲο�������https://stackoverflow.com/questions/10060046/drawing-lines-with-bresenhams-line-algorithm/16405254#16405254
// ��������������˵�Ĳ�һ����ʵ��Ч���Ļ����ζԽ����ϸ��ǲ�����ȷʵ�����⡣�������������
 
// update��2021.8.19 �����޸���
// ���ָ����⣺���������Ҳ������Ұ��Χ����ô�����ڴ���������Ӧλ�ñ���ʾ��������
// �·�����ª��ʾ��ͼ������#���Ҳ࣬Ӧ��ֻ���ұ��ǲ��֣���������Ӧ���ڵ��ĵط����������ȥ��������ʱ��о����Ч���е��ã�
//	**************************
//	*                        *
//	*						#*
//	*#					   ##*		
//	*##					  ###*	
//	*                        *
//	**************************
// ����ȷ����������������ʵ����ֱ�����±������ˣ��⵼�±���(x,y)���˷�Χ��y*w+x�õ�������û���������
// Ȼ�������������õ��µ�(x,y)��һ�����������Ч��������
void Graphics::draw_line(const Vec2i& start, const Vec2i& end, uint32_t color)
{

	// 2021.8.19 bug version
	//int dx = end.x - start.x;
	//int dy = end.y - start.y;
	//int dLong = abs(dx);
	//int dShort = abs(dy);
	//int offsetLong = dx > 0 ? 1 : -1;
	//int offsetShort = dy > 0 ? width : -width;
	//if (dLong < dShort)
	//{
	//	std::swap(dShort, dLong);
	//	std::swap(offsetShort, offsetLong);
	//}
	//int error = dShort - dLong / 2;
	//int index = start.y * width + start.x;
	//const int offset[] = { offsetLong, offsetLong + offsetShort };
	//const int abs_d[] = { dShort, dShort - dLong };
	//for (int i = 0; i <= dLong; ++i)
	//{
	//	int v = index;
	//	set_pixel(v % width, v / width, color);  // bug find test
	//	//set_pixel(index, color);
	//	const int errorIsTooBig = error >= 0;
	//	index += offset[errorIsTooBig];
	//	error += abs_d[errorIsTooBig];
	//}

	int dx = end.x - start.x;
	int dy = end.y - start.y;
	int dLong = abs(dx);
	int dShort = abs(dy);
	int offsetLong = dx > 0 ? 1 : -1;
	int offsetShort = dy > 0 ? 1 : -1;
	bool f = true;
	if (dLong < dShort)
	{
		f = false;
		std::swap(dShort, dLong);
		std::swap(offsetShort, offsetLong);
	}
	int error = dShort - dLong / 2;
	int nx = start.x, ny = start.y;
	const int offset[] = { 0, offsetShort };
	const int abs_d[] = { dShort, dShort - dLong };
	if (f) {
	// ��ʵ�ĵ������������ôд�����ƺ������ڵ�д����һЩ
	//	(f ? nx : ny) += offsetLong;
	//	(f ? ny : nx) += offset[errorIsTooBig];
		for (int i = 0; i <= dLong; ++i)
		{
			set_pixel(nx, ny, color);
			const int errorIsTooBig = error >= 0;
			nx += offsetLong;
			ny += offset[errorIsTooBig];
			error += abs_d[errorIsTooBig];
		}
	}
	else {
		for (int i = 0; i <= dLong; ++i)
		{
			set_pixel(nx, ny, color);
			const int errorIsTooBig = error >= 0;
			ny += offsetLong;
			nx += offset[errorIsTooBig];
			error += abs_d[errorIsTooBig];
		}
	}
}

template<typename T>
void Graphics::draw_line(const Vec3<T>& start, const Vec3<T>& end, uint32_t color)
{
	draw_line(Vec2i(start.x, start.y), Vec2i(end.x, end.y), color);
}


//static std::tuple<float, float, float>   //���ܷ��������������ֵ̫����
void computeBarycentric2D(float x, float y, const Vec3f* v,
	float &c1, float& c2, float& c3)		
{
	c1 = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
	c2 = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
	//float c3 = (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
	c3 = 1 - c1 - c2;
	//return std::tuple<float, float, float>(c1, c2, 1 - c1 - c2);
}


bool check_cvv_clip(const Vec4f& v) {
	float r = fabs(v.w), l = -r;
	return v.x >= l && v.x <= r
		&& v.y >= l && v.y <= r
		&& v.z >= l && v.z <= r;
}

// �����޳�����mvp�����Ļ�ռ��ж�����
// �����ڵ���Ļ�ռ��д���
// https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf  Chapter 3.5.1
//this is the same as(but more efficient than)
// vec3_t ab = vec3_sub(b, a);
// vec3_t ac = vec3_sub(c, a);
// return vec3_cross(ab, ac).z > 0;   ���
// ���һ�� ��ͬ�� dot(cross(ab,ac), eye_dir) > 0    eye_dir = (0,0,1) �������ǿ���ĳ�����
// ����˳��opengl��Ϊ��ʱ��Ϊ�������Ϊ���򷵻�true
bool backface_culling(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3) {
	float a = v1.x * v2.y - v1.y * v2.x +
		v2.x * v3.y - v2.y * v3.x +
		v3.x * v1.y - v3.y * v1.x;
	return -a > 0;  // ��ʱ��ʱ������������Ҫȡ��"Setting dir to CCW indicates that the sign of a should be reversed prior to use"
}


void Graphics::DrawTriangle(float angle)
{
	Vec3f vertices[] = {
		//{2, 0, -2},
		//{0, 2, -2},
		//{-2, 0, -2},
		//{0, -2, -10},
		//{3.5, -1, -5},
		//{2.5, 1.5, -5},
		//{-1, 0.5, 1},

		{-1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f},
		{-1.0f, 1.0f, -1.0f},
		{-1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, -1.0f},
		{1.0f, 1.0f, 1.0f}
	};
	Vec3f colors[] = {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
	};
	static int vertex_indices[] = {
		//0, 1, 2,
		//0, 2, 3,
		//4, 5, 6,

		0,2,1, 1,2,6,
		1,6,7, 7,4,1,
		4,7,5, 5,3,4,
		5,2,0, 3,5,0,
		7,6,2, 2,5,7,
		0,1,4, 0,4,3
	};

	static Mat4f vp = Transform3::viewport(width, height);
	Mat4f mvp =  //Transform3::rotate_z(angle) * Transform3::rotate_x(angle) * 
		Transform3::rotate_y(angle) *
		Transform3::scale(3 / 4.0f, 1.0f, 1.0f) 
		//* Transform3::translate(2.0f,0.0f,0.0f) * Transform3::rotate_y(angle)
		* Transform3::view(Vec3f(0, 0, 5), Vec3f(0, 0, -1), Vec3f(0, 1, 0))
		* Transform3::persp(Math::deg2rad(45), 1, 0.1, 100);


	int tri_n = sizeof(vertex_indices) / sizeof(int) / 3;
	//for (int i = 0; i < tri_n; ++i) {
	// 	// ȡһ��������
	//	Vec3f v1 = vertices[vertex_indices[3 * i]];
	//	Vec3f v2 = vertices[vertex_indices[3 * i + 1]];
	//	Vec3f v3 = vertices[vertex_indices[3 * i + 2]];
	for (int i = 0; i < model->nfaces(); ++i) {
		auto f = model->face(i);
		// ȡһ��������
		Vec3f v1 = model->vert(f[0]);
		Vec3f v2 = model->vert(f[1]);
		Vec3f v3 = model->vert(f[2]);

		//����任
		//v1 = ((v1.to_vec4() * mvp).homogenize() * vp).to_vec3();
		//v2 = ((v2.to_vec4() * mvp).homogenize() * vp).to_vec3();
		//v3 = ((v3.to_vec4() * mvp).homogenize() * vp).to_vec3();

		// ��һ��w�ĵ�����Ϊ��͸�ӽ���
		auto vv1 = v1.to_vec4() * mvp;
		auto vv2 = v2.to_vec4() * mvp;
		auto vv3 = v3.to_vec4() * mvp;

		// cvv �ü�  ������������cvv��������
		if (!check_cvv_clip(vv1)
			|| !check_cvv_clip(vv2)
			|| !check_cvv_clip(vv3))
			continue;

		float rhws[] = { 1.0f / vv1.w, 1.0f / vv2.w, 1.0f / vv3.w };
		// ��λ� & �ӿڱ任
		v1 = (vv1.homogenize() * vp).to_vec3();
		v2 = (vv2.homogenize() * vp).to_vec3();
		v3 = (vv3.homogenize() * vp).to_vec3();

		if (mode == Graphics::RenderMode::WIREFRAME) {
			auto c = Math::vec_to_color(Vec3f(0,0,0));
			draw_line(v1, v2, c);
			draw_line(v2, v3, c);
			draw_line(v3, v1, c);
			continue;
		}

		// mode == Graphics::RenderMode::FILLEDTRIANGLE
		 
		// �����޳��� ��Ϊ˳ʱ������Ϊ����
		// ��Ϊ���棬�����
		if (!backface_culling(v1, v2, v3)) continue;

		int minx = std::floor(std::min({ v1.x, v2.x ,v3.x }));
		int maxx = std::ceil(std::max({ v1.x, v2.x ,v3.x }));
		int miny = std::floor(std::min({ v1.y, v2.y ,v3.y }));
		int maxy = std::ceil(std::max({ v1.y, v2.y ,v3.y }));
		minx = std::max(0, minx - 1);
		maxx = std::min(width - 1, maxx + 1);
		miny = std::max(0, miny);
		maxy = std::min(height - 1, maxy + 1);

		Vec3f v_screen[] = { v1, v2, v3 };

		for (int j = miny; j < maxy; ++j){
			for (int i = minx; i <= maxx; ++i) {
				// basic rasterization
				float x = i + 0.5;
				float y = j + 0.5;
				float alpha, beta, gamma;
				computeBarycentric2D(x, y, v_screen, alpha, beta, gamma);
				if (alpha > -EPSILON && beta > -EPSILON && gamma > -EPSILON) {  // ֱ������������
					// �����Բ�ֵ
					// ͸�ӽ�������ϵ��
					alpha *= rhws[0], beta *= rhws[1], gamma *= rhws[2];
					float inv = 1.0 / (alpha + beta + gamma);

					float Z = rhws[0] * alpha + beta * rhws[1] + gamma * rhws[2];  // ��Ȳ��Կ�����1/w����z

					if (Z > depthbuffer[j * width + i]) { //��Ȳ���
						depthbuffer[j * width + i] = Z;

					
						Vec3f color = inv * (colors[0] * alpha + colors[1] * beta + colors[2] * gamma);
						
						set_pixel_unsafe(i, j,
							Math::vec_to_color(color));
							//Math::vec_to_color(colors[0] * alpha + colors[1] * beta + colors[2] * gamma));
					}
				}
			}
		}

	}

}

void Graphics::save_as_bmp_file(const char* filename)
{
	int m = 0;
	for (int j = 0; j < height; ++j)
		for (int i = 0; i < width; ++i)
			bmp_manager.SetPixel(i, j, framebuffer[m++]);
	bmp_manager.SaveFile(filename);
}
