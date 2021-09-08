// OPENMP ʹ�� https://openmp.org/wp-content/uploads/openmp-examples-4.5.0.pdf   https://zhuanlan.zhihu.com/p/61857547
// Ȼ��vs��֧��2.0���������˵���� https://devblogs.microsoft.com/cppblog/improved-openmp-support-for-cpp-in-visual-studio/
//#include <omp.h>

#include "Timer.h"
#include "Graphics.h"
#include "Exception.h"
#include "ShaderBase.h"
#include <iostream>


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
			depthbuffer[i * width + j] = 0;// -std::numeric_limits<float>::infinity();
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


// �ڰ�����
Vec3f gen_grid(Vec2f uv) {
	uv *= 10;
	float x = std::floor(uv.u);
	float y = std::floor(uv.v);
	float c = (x + y) / 2;
	float frac = c - (int)c;
	return Vec3f(frac, frac, frac) * 2;
}

// �ڰ�ͬ��Բ�����..) 
Vec3f gen_noise(Vec2f uv) {
	
	uv = uv - Vec2f(0.5f, 0.5f);
	float c = std::sqrt(uv.u * uv.u + uv.v * uv.v) * 10;
	float frac = c - (int)c;
	return Vec3f(frac, frac, frac);
}

// shader��
void Graphics::draw_object(Object& obj)
{

	int tri_n = obj.indices.size() / 3;

//#pragma omp parallel for
	for (int k = 0; k < tri_n; ++k) {
		// ȡһ��������
		Vertex v1{
			obj.vertices[obj.indices[3 * k]],
			obj.normals[obj.indices[3 * k]],
			obj.texcoords[obj.indices[3 * k]]
		};
		Vertex v2{
			obj.vertices[obj.indices[3 * k + 1]],
			obj.normals[obj.indices[3 * k + 1]],
			obj.texcoords[obj.indices[3 * k + 1]]
		};
		Vertex v3{
			obj.vertices[obj.indices[3 * k + 2]],
			obj.normals[obj.indices[3 * k + 2]],
			obj.texcoords[obj.indices[3 * k + 2]]
		};
		V2F v2f[3];

		// ����vertex_shader
		Vec4f vv1 = obj.vertex_shader(v1, obj.transform, v2f[0]);
		Vec4f vv2 = obj.vertex_shader(v2, obj.transform, v2f[1]);
		Vec4f vv3 = obj.vertex_shader(v3, obj.transform, v2f[2]);

		// cvv �ü�  ������������cvv��������
		if (!check_cvv_clip(vv1) || !check_cvv_clip(vv2) || !check_cvv_clip(vv3))
			continue;

		// ��λ�
		vv1 = vv1.homogenize();
		vv2 = vv2.homogenize();
		vv3 = vv3.homogenize();

		// �ӿڱ任
		// ���ڼ���Ƚϼ򵥣�ֱ���ֶ�չ��������viewport�Ǹ��������ȣ�
		// [0,1]^3 --> [0,w]*[0,h]*[0,1]
		vv1.x = (1.0f + vv1.x) * width * 0.5f;
		vv1.y = (1.0f - vv1.y) * height * 0.5f;
		vv1.z = (1.0f + vv1.z) * 0.5f;
		vv2.x = (1.0f + vv2.x) * width * 0.5f;
		vv2.y = (1.0f - vv2.y) * height * 0.5f;
		vv2.z = (1.0f + vv2.z) * 0.5f;
		vv3.x = (1.0f + vv3.x) * width * 0.5f;
		vv3.y = (1.0f - vv3.y) * height * 0.5f;
		vv3.z = (1.0f + vv3.z) * 0.5f;
		

		Vec3f v_screen[] = {
			vv1.to_vec3(),
			vv2.to_vec3(),
			vv3.to_vec3()
		};

		if (disc.display == RenderMode::WireFrame) {
			auto c = Math::vec_to_color(Vec3f(1, 1, 1));
			draw_line(v_screen[0], v_screen[1], c);
			draw_line(v_screen[1], v_screen[2], c);
			draw_line(v_screen[2], v_screen[0], c);
			continue;
		}

		// mode == Graphics::RenderMode::FILLEDTRIANGLE

		// �����޳��� ��Ϊ˳ʱ������Ϊ����
		// ��Ϊ���棬�����
		if (!backface_culling(v_screen[0], v_screen[1], v_screen[2])) 
			continue;

		int minx = std::floor(std::min({ v_screen[0].x, v_screen[1].x ,v_screen[2].x }));
		int maxx = std::ceil(std::max({ v_screen[0].x, v_screen[1].x ,v_screen[2].x }));
		int miny = std::floor(std::min({ v_screen[0].y, v_screen[1].y ,v_screen[2].y }));
		int maxy = std::ceil(std::max({ v_screen[0].y, v_screen[1].y ,v_screen[2].y }));
		minx = std::max(0, minx - 1);
		maxx = std::min(width - 1, maxx + 1);
		miny = std::max(0, miny);
		maxy = std::min(height - 1, maxy + 1);	

		for (int j = miny; j < maxy; ++j) {
			for (int i = minx; i <= maxx; ++i) {
				float x = i + 0.5;
				float y = j + 0.5;
				float alpha, beta, gamma;
				computeBarycentric2D(x, y, v_screen, alpha, beta, gamma);
				if (alpha > -EPSILON && beta > -EPSILON && gamma > -EPSILON) {  // ֱ������������
					// �����Բ�ֵ
					// ��Ȳ���Ҳ������1/w����z
					float Z = alpha * v_screen[0].z + beta * v_screen[1].z + gamma * v_screen[2].z;

					// ͸�ӽ���
					alpha *= v2f[0].rhw, beta *= v2f[1].rhw, gamma *= v2f[2].rhw;
					float inv = 1.0 / (alpha + beta + gamma);
					alpha *= inv, beta *= inv, gamma *= inv;

					if (Z > depthbuffer[j * width + i]) { //��Ȳ���
						depthbuffer[j * width + i] = Z;

						// ���߲�ֵ
						Vec3f n = v2f[0].normal * alpha + v2f[1].normal * beta + v2f[2].normal * gamma;

						// viewspace�������ֵ
						Vec3f coord = v2f[0].vtx_view * alpha + v2f[1].vtx_view * beta + v2f[2].vtx_view * gamma;

						// ��������
						Vec2f texcoord = v2f[0].texcoord * alpha + v2f[1].texcoord * beta + v2f[2].texcoord * gamma;

						
	
						// ����pixelshader
						V2F param{
							coord,
							{0,0,0,0},
							n.normalize(),
							texcoord,
							0,
							obj.get_tex(texcoord.u, texcoord.v, disc.sample, disc.texwrap)
						};
						Vec3f color = obj.pixel_shader(param);
						set_pixel_unsafe(i, j, Math::vec_to_color(color));
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
