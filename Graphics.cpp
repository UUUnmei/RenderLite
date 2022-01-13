// OPENMP 使用 https://openmp.org/wp-content/uploads/openmp-examples-4.5.0.pdf   https://zhuanlan.zhihu.com/p/61857547
// 然而vs才支持2.0。。。相关说明见 https://devblogs.microsoft.com/cppblog/improved-openmp-support-for-cpp-in-visual-studio/
//#include <omp.h>

#include "Timer.h"
#include "Graphics.h"
#include "Exception.h"
#include "ShaderBase.h"
#include <iostream>
#include <cassert>


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
	//bmpinfo.bmiColors 还不太清楚。。

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
		for (int j = 0; j < width; ++j) 
			framebuffer[i * width + j] = color;

	if (disc.MSAA4x) {
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) {
				int idx = i * width + j;
				for (int k = 0; k < 4; ++k)
					depthbuffer[k + idx * 4] = 1.0f;
			}
	}
	else {
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				depthbuffer[i * width + j] = 1.0f;
	}
				
}

void Graphics::draw(void)
{
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
}

// bresenham
// 学习算法http://www.idivecat.com/archives/652 
// 实现参考了这里https://stackoverflow.com/questions/10060046/drawing-lines-with-bresenhams-line-algorithm/16405254#16405254
// 但是误差项和上面说的不一样，实际效果的话主次对角线上覆盖不到，确实有问题。按照上面改了下
 
// update：2021.8.19 （已修复）
// 发现个问题：比如物体右侧出了视野范围，那么它会在窗口左侧的相应位置被显示出来，，
// 下方（简陋）示意图，物体#在右侧，应该只有右边那部分，但是它本应被遮挡的地方画到了左边去（不过有时候感觉这个效果有点用）
//	**************************
//	*                        *
//	*						#*
//	*#					   ##*		
//	*##					  ###*	
//	*                        *
//	**************************
// 基本确定问题是在于它的实现是直接算下标索引了，这导致比如(x,y)超了范围但y*w+x得到的索引没超的情况下
// 然后会用这个索引得到新的(x,y)，一画就是上面的效果。。。
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
	// 其实改的是这个，但这么写性能似乎比现在的写法慢一些
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


//static std::tuple<float, float, float>   //性能分析发现这个返回值太慢了
void computeBarycentric2D(float x, float y, const Vec3f* v,
	float &c1, float& c2, float& c3)		
{
	c1 = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
	c2 = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
	//float c3 = (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
	c3 = 1 - c1 - c2;
	//return std::tuple<float, float, float>(c1, c2, 1 - c1 - c2);
}


bool check_cvv_clip_any(const Vec4f& v) {
	float r = fabs(v.w), l = -r;
	return v.x >= l || v.x <= r
		|| v.y >= l || v.y <= r
		|| v.z >= l || v.z <= r;
}

bool check_cvv_clip_all(const Vec4f& v) {
	float r = fabs(v.w), l = -r;
	return v.x >= l && v.x <= r
		&& v.y >= l && v.y <= r
		&& v.z >= l && v.z <= r;
}




// 背面剔除，在mvp后和屏幕空间中都可以
// 这里在的屏幕空间中处理
// https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf  Chapter 3.5.1
//this is the same as(but more efficient than)
// vec3_t ab = vec3_sub(b, a);
// vec3_t ac = vec3_sub(c, a);
// return vec3_cross(ab, ac).z > 0;   面积
// 最后一句 等同于 dot(cross(ab,ac), eye_dir) > 0    eye_dir = (0,0,1) 这样就是看面的朝向了
// 这里顺着opengl认为逆时针为正向，如果为正向返回true
bool backface_culling(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3) {
	float a = v1.x * v2.y - v1.y * v2.x +
		v2.x * v3.y - v2.y * v3.x +
		v3.x * v1.y - v3.y * v1.x;
	return -a > 0;  // 逆时针时对于上面计算的要取反"Setting dir to CCW indicates that the sign of a should be reversed prior to use"
	// 关于取负的新理解，在2d部分多边形裁剪中提到的，由于是在屏幕空间坐标进行的，而屏幕坐标y轴向下，导致叉乘变号
}

const std::vector<Vec4f> clip_planes{
	{0,0,1,1}, //near
	{0,0,-1,1}, //far
	{1,0,0,1}, //left
	{0,1,0,1},  //top
	{-1,0,0,1}, //right
	{0,-1,0,1}  //bottom 
};


bool inside_test(const Vec4f& plane, const Vec4f& point) {
	return plane.dot(point) > -EPSILON;
}

V2F lerp_v2f(const V2F& p1, const V2F& p2, float t) {
	V2F ret;
	ret.vtx_model = Math::lerp(p1.vtx_model, p2.vtx_model, t);
	ret.vtx_view = Math::lerp(p1.vtx_view, p2.vtx_view, t);
	ret.vtx_mvp = Math::lerp(p1.vtx_mvp, p2.vtx_mvp, t);
	ret.normal = Math::lerp(p1.normal, p2.normal, t);
	ret.texcoord = Math::lerp(p1.texcoord, p2.texcoord, t);
	// 不能在这插值rhw，不线性
	return ret;
}

V2F get_intersection(const Vec4f& plane, const V2F& p1, const V2F& p2) {
	float d1 = plane.dot(p1.vtx_mvp);
	float d2 = plane.dot(p2.vtx_mvp);
	float t = d1 / (d1 - d2);
	return lerp_v2f(p1, p2, t);
}


// 齐次空间裁剪 
// https://zhuanlan.zhihu.com/p/162190576 其中还有相关参考
// https://github.com/SilverClawko/SoftRender/blob/newVersion/Clip.cpp
// 也类似2d部分的 Sutherland_Hodgeman多边形裁剪
std::vector<V2F> homogeneous_clip(const V2F& v1, const V2F& v2, const V2F& v3) {
	std::vector<V2F> ret{ v1, v2, v3 };

	// 加个判断，如果实际不需裁剪的话就返回
	// 不然下面循环里完全是pushback分配内存，性能暴跌
	if (check_cvv_clip_all(v1.vtx_mvp) && check_cvv_clip_all(v2.vtx_mvp) && check_cvv_clip_all(v3.vtx_mvp))
		return ret;

	for (int k = 0; k < clip_planes.size(); ++k) {
		std::vector<V2F> cur;
		for (int i = 0; i < ret.size(); ++i) {
			int j = (i + 1) % ret.size();

			bool f1 = inside_test(clip_planes[k], ret[i].vtx_mvp); //为真表示该点在要保留的范围
			bool f2 = inside_test(clip_planes[k], ret[j].vtx_mvp);  

			if (f1 && f2) {
				// 两点都可见 要第二个点
				cur.push_back(ret[j]);
			}
			else if (f1) {
				// 只有第一个点可见， 要与边界的交点
				cur.push_back(get_intersection(clip_planes[k], ret[i], ret[j]));
			}
			else if (f2) {
				// 只有第二个点可见，要交点和第二个点
				cur.push_back(get_intersection(clip_planes[k], ret[i], ret[j]));
				cur.push_back(ret[j]);
			}
			else {
				// 都不可见，弃之
			}
		}
		ret = cur;
	}
	return ret;
}

// shader版
void Graphics::draw_object(Object& obj)
{

	int tri_n = obj.indices.size() / 3;

	//#pragma omp parallel for
	for (int k = 0; k < tri_n; ++k) {
		// 取一个三角形
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

		// 调用vertex_shader
		obj.vertex_shader(v1, obj.transform, v2f[0]);
		obj.vertex_shader(v2, obj.transform, v2f[1]);
		obj.vertex_shader(v3, obj.transform, v2f[2]);


		// cvv 裁剪 
		// 之前是只要有一个顶点不在就剔除，
		// 现在为了配合齐次裁剪，只有完全不在的才剔除
		// 脑补一下魔方，cvv是最里面那个立方体，这一步只会裁掉位于魔方顶点的8个位置，剩下 27 - 8 = 19个都留着
		if (!check_cvv_clip_any(v2f[0].vtx_mvp) && !check_cvv_clip_any(v2f[1].vtx_mvp) && !check_cvv_clip_any(v2f[2].vtx_mvp))
			continue;	

		// 齐次裁剪 用mvp后的坐标
		// 对于处理摄像机在模型内部的情况非常重要！
		std::vector<V2F> cliped_vtx = homogeneous_clip(v2f[0], v2f[1], v2f[2]);
		//std::vector<V2F> cliped_vtx = { v2f[0], v2f[1], v2f[2] };

		for (V2F& x : cliped_vtx) {
			// 求屏幕坐标
			x.vtx_wnd = x.vtx_mvp;
			x.rhw = 1.0f / x.vtx_mvp.w;  // 为了插值进行预处理
			auto& vv = x.vtx_wnd;
			//// 齐次化
			vv = vv.homogenize();
			//std::stringstream ss;
			//ss << vv.x << ' ' << vv.y << ' ' << vv.z << ' ' << vv.w << '\n';
			//OutputDebugString(ss.str().c_str());
			// 视口变换
			vv.x = (1.0f + vv.x) * width * 0.5f;
			vv.y = (1.0f - vv.y) * height * 0.5f;
			//vv.z = (1.0f + vv.z) * 0.5f;
			
		}

		int sz = cliped_vtx.size() - 2;
		for (int z = 0; z < sz; ++z) {
			// 取裁剪后的一个三角形
			V2F vf1 = cliped_vtx[0];   // 注意顺序
			V2F vf2 = cliped_vtx[z + 1];
			V2F vf3 = cliped_vtx[z + 2];
			Vec3f v_screen[] = {
				vf1.vtx_wnd.to_vec3(),
				vf2.vtx_wnd.to_vec3(),
				vf3.vtx_wnd.to_vec3()
			};

			if (disc.display == RenderMode::WireFrame) {
				auto c = Math::vec_to_color(Vec3f(1, 1, 1));
				draw_line(v_screen[0], v_screen[1], c);
				draw_line(v_screen[1], v_screen[2], c);
				draw_line(v_screen[2], v_screen[0], c);
				continue;
			}

			// mode == Graphics::RenderMode::FILLEDTRIANGLE

			if ((disc.fc_mode == FaceCullMode::Back && !backface_culling(v_screen[0], v_screen[1], v_screen[2]))
				|| (disc.fc_mode == FaceCullMode::Front && backface_culling(v_screen[0], v_screen[1], v_screen[2])))
				continue;


			int minx = std::floor(std::min({ v_screen[0].x, v_screen[1].x ,v_screen[2].x }));
			int maxx = std::ceil(std::max({ v_screen[0].x, v_screen[1].x ,v_screen[2].x }));
			int miny = std::floor(std::min({ v_screen[0].y, v_screen[1].y ,v_screen[2].y }));
			int maxy = std::ceil(std::max({ v_screen[0].y, v_screen[1].y ,v_screen[2].y }));
			minx = std::max(0, minx - 1);
			maxx = std::min(width - 1, maxx + 1);
			miny = std::max(0, miny);
			maxy = std::min(height - 1, maxy + 1);

			if (disc.MSAA4x == false) {
				for (int j = miny; j <= maxy; ++j) {
					for (int i = minx; i <= maxx; ++i) {
						float x = i + 0.5;
						float y = j + 0.5;
						float alpha, beta, gamma;
						computeBarycentric2D(x, y, v_screen, alpha, beta, gamma);
						if (alpha > -EPSILON && beta > -EPSILON && gamma > -EPSILON) {  // 直接用重心坐标
							// 各属性插值
							// 深度测试也可以用1/w代替z
							float Z = alpha * v_screen[0].z + beta * v_screen[1].z + gamma * v_screen[2].z;
							// 透视矫正
							alpha *= vf1.rhw, beta *= vf2.rhw, gamma *= vf3.rhw;
							float inv = 1.0 / (alpha + beta + gamma);
							alpha *= inv, beta *= inv, gamma *= inv;
							//深度测试
							if (Z <= depthbuffer[j * width + i]) {
								depthbuffer[j * width + i] = Z;
								// 法线插值
								Vec3f n = vf1.normal * alpha + vf2.normal * beta + vf3.normal * gamma;
								// viewspace中坐标插值
								Vec3f coord = vf1.vtx_view * alpha + vf2.vtx_view * beta + vf3.vtx_view * gamma;
								// 纹理坐标
								Vec2f texcoord = vf1.texcoord * alpha + vf2.texcoord * beta + vf3.texcoord * gamma;


								Vec3f wrd = vf1.vtx_model * alpha + vf2.vtx_model * beta + vf3.vtx_model * gamma;
								wrd = wrd.normalize();
								// 调用pixelshader
								V2F param{
									wrd,
									coord,
									{0,0,0,0},
									{0,0,0,0},
									n.normalize(),
									texcoord,
									0,
									obj.type == 0 ? obj.get_tex(texcoord.u, texcoord.v) : obj.get_tex(wrd.x, wrd.y, wrd.z)
								};
								Vec3f color = obj.pixel_shader(param);
								set_pixel_unsafe(i, j, Math::vec_to_color(color));
							}
						}
					}
				}
			}

		}


		


	}
}



//// shader版
//void Graphics::draw_object(Object& obj)
//{
//
//	int tri_n = obj.indices.size() / 3;
//
////#pragma omp parallel for
//	for (int k = 0; k < tri_n; ++k) {
//		// 取一个三角形
//		Vertex v1{
//			obj.vertices[obj.indices[3 * k]],
//			obj.normals[obj.indices[3 * k]],
//			obj.texcoords[obj.indices[3 * k]]
//		};
//		Vertex v2{
//			obj.vertices[obj.indices[3 * k + 1]],
//			obj.normals[obj.indices[3 * k + 1]],
//			obj.texcoords[obj.indices[3 * k + 1]]
//		};
//		Vertex v3{
//			obj.vertices[obj.indices[3 * k + 2]],
//			obj.normals[obj.indices[3 * k + 2]],
//			obj.texcoords[obj.indices[3 * k + 2]]
//		};
//		V2F v2f[3];
//
//		// 调用vertex_shader
//		Vec4f vv1 = obj.vertex_shader(v1, obj.transform, v2f[0]);
//		Vec4f vv2 = obj.vertex_shader(v2, obj.transform, v2f[1]);
//		Vec4f vv3 = obj.vertex_shader(v3, obj.transform, v2f[2]);
//
//
//		// cvv 裁剪  整个放弃超过cvv的三角形
//		if (!check_cvv_clip(vv1) || !check_cvv_clip(vv2) || !check_cvv_clip(vv3))
//			continue;
//
//		// TODO: 齐次裁剪
//		//std::vector<V2F> cliped_vtx = homogeneous_clip();
//		//std::vector<V2F> cliped_vtx = { v2f[0], v2f[1], v2f[2] };
//	https://zhuanlan.zhihu.com/p/162190576
//
//
//
//
//		// 齐次化
//		vv1 = vv1.homogenize();
//		vv2 = vv2.homogenize();
//		vv3 = vv3.homogenize();
//
//		// 视口变换
//		// 由于计算比较简单，直接手动展开（不用viewport那个矩阵了先）
//		// [-1,1]^3 --> [0,w]*[0,h]*[0,1]
//		vv1.x = (1.0f + vv1.x) * width * 0.5f;
//		vv1.y = (1.0f - vv1.y) * height * 0.5f;
//		vv1.z = (1.0f + vv1.z) * 0.5f;
//		vv2.x = (1.0f + vv2.x) * width * 0.5f;
//		vv2.y = (1.0f - vv2.y) * height * 0.5f;
//		vv2.z = (1.0f + vv2.z) * 0.5f;
//		vv3.x = (1.0f + vv3.x) * width * 0.5f;
//		vv3.y = (1.0f - vv3.y) * height * 0.5f;
//		vv3.z = (1.0f + vv3.z) * 0.5f;
//
//
//		Vec3f v_screen[] = {
//			vv1.to_vec3(),
//			vv2.to_vec3(),
//			vv3.to_vec3()
//		};
//
//		if (disc.display == RenderMode::WireFrame) {
//			auto c = Math::vec_to_color(Vec3f(1, 1, 1));
//			draw_line(v_screen[0], v_screen[1], c);
//			draw_line(v_screen[1], v_screen[2], c);
//			draw_line(v_screen[2], v_screen[0], c);
//			continue;
//		}
//
//		// mode == Graphics::RenderMode::FILLEDTRIANGLE
//
//		// 背面剔除， 认为顺时针排列为正面
//		// 若为背面，则忽略
//		if ((disc.fc_mode == FaceCullMode::Back && !backface_culling(v_screen[0], v_screen[1], v_screen[2]))
//			|| (disc.fc_mode == FaceCullMode::Front && backface_culling(v_screen[0], v_screen[1], v_screen[2])))
//			continue;
//
//		int minx = std::floor(std::min({ v_screen[0].x, v_screen[1].x ,v_screen[2].x }));
//		int maxx = std::ceil(std::max({ v_screen[0].x, v_screen[1].x ,v_screen[2].x }));
//		int miny = std::floor(std::min({ v_screen[0].y, v_screen[1].y ,v_screen[2].y }));
//		int maxy = std::ceil(std::max({ v_screen[0].y, v_screen[1].y ,v_screen[2].y }));
//		minx = std::max(0, minx - 1);
//		maxx = std::min(width - 1, maxx + 1);
//		miny = std::max(0, miny);
//		maxy = std::min(height - 1, maxy + 1);	
//
//		if (disc.MSAA4x == false) {
//			for (int j = miny; j <= maxy; ++j) {
//				for (int i = minx; i <= maxx; ++i) {
//					float x = i + 0.5;
//					float y = j + 0.5;
//					float alpha, beta, gamma;
//					computeBarycentric2D(x, y, v_screen, alpha, beta, gamma);
//					if (alpha > -EPSILON && beta > -EPSILON && gamma > -EPSILON) {  // 直接用重心坐标
//						// 各属性插值
//						// 深度测试也可以用1/w代替z
//						float Z = alpha * v_screen[0].z + beta * v_screen[1].z + gamma * v_screen[2].z;
//						// 透视矫正
//						alpha *= v2f[0].rhw, beta *= v2f[1].rhw, gamma *= v2f[2].rhw;
//						float inv = 1.0 / (alpha + beta + gamma);
//						alpha *= inv, beta *= inv, gamma *= inv;
//						//深度测试
//						if (Z >= depthbuffer[j * width + i]) { 
//							depthbuffer[j * width + i] = Z;
//							// 法线插值
//							Vec3f n = v2f[0].normal * alpha + v2f[1].normal * beta + v2f[2].normal * gamma;
//							// viewspace中坐标插值
//							Vec3f coord = v2f[0].vtx_view * alpha + v2f[1].vtx_view * beta + v2f[2].vtx_view * gamma;
//							// 纹理坐标
//							Vec2f texcoord = v2f[0].texcoord * alpha + v2f[1].texcoord * beta + v2f[2].texcoord * gamma;
//							
//
//							Vec3f wrd = v2f[0].vtx_model * alpha + v2f[1].vtx_model * beta + v2f[2].vtx_model * gamma;
//							wrd = wrd.normalize();
//							// 调用pixelshader
//							V2F param{
//								wrd,
//								coord,
//								{0,0,0,0},
//								{0,0,0,0},
//								n.normalize(),
//								texcoord,
//								0,
//								obj.type == 0 ? obj.get_tex(texcoord.u, texcoord.v) : obj.get_tex(wrd.x, wrd.y, wrd.z)
//							};
//							Vec3f color = obj.pixel_shader(param);
//							set_pixel_unsafe(i, j, Math::vec_to_color(color));
//						}
//					}
//				}
//			}
//		}
//		else {
//			//// 开启 MSAA 4x
//			//for (int j = miny; j <= maxy; ++j) {
//			//	for (int i = minx; i <= maxx; ++i) {
//			//		int idx = j * width + i;
//			//		int cnt = 0, cnt2 = 0;
//			//		float d = 0.5;
//			//		Vec3f n, coord, color, wrd;
//			//		Vec2f texcoord;
//			//		// multi sampling
//			//		for (int a = 0; a < 2; ++a) {
//			//			for (int b = 0; b < 2; ++b) {
//			//				float x = i + a * 0.5 + 0.25;
//			//				float y = j + b * 0.5 + 0.25;
//			//				float alpha, beta, gamma;
//			//				computeBarycentric2D(x, y, v_screen, alpha, beta, gamma);
//			//				if (alpha > -EPSILON && beta > -EPSILON && gamma > -EPSILON) {
//			//					cnt++;
//			//					float Z = alpha * v_screen[0].z + beta * v_screen[1].z + gamma * v_screen[2].z;
//			//					// 透视矫正
//			//					alpha *= v2f[0].rhw, beta *= v2f[1].rhw, gamma *= v2f[2].rhw;
//			//					float inv = 1.0 / (alpha + beta + gamma);
//			//					alpha *= inv, beta *= inv, gamma *= inv;
//
//			//					int t = idx * 4 + a * 2 + b;
//			//					if (Z > depthbuffer[t]) {
//			//						depthbuffer[t] = Z;
//			//						cnt2++;
//			//						// 法线插值
//			//						n += (v2f[0].normal * alpha + v2f[1].normal * beta + v2f[2].normal * gamma).normalize();
//			//						// viewspace中坐标插值
//			//						coord += v2f[0].vtx_view * alpha + v2f[1].vtx_view * beta + v2f[2].vtx_view * gamma;
//			//						// 纹理坐标
//			//						texcoord += v2f[0].texcoord * alpha + v2f[1].texcoord * beta + v2f[2].texcoord * gamma;		
//
//			//						wrd = v2f[0].vtx_world * alpha + v2f[1].vtx_world * beta + v2f[2].vtx_world * gamma;
//			//						wrd = wrd.normalize();
//			//					}
//			//				}
//			//			}
//			//		}
//			//		if (cnt2 > 0) {
//			//			
//			//			coord /= (float)cnt2;
//			//			n /= (float)cnt2;
//			//			texcoord /= (float)cnt2;
//
//			//			V2F param{
//			//				wrd,
//			//				coord,
//			//				{0,0,0,0},
//			//				{0,0,0,0},
//			//				n.normalize(),
//			//				texcoord,
//			//				0,
//			//				obj.get_tex(texcoord.u, texcoord.v)
//			//			};
//			//			Vec3f color = obj.pixel_shader(param);// *cnt / 4.0 + framebuffer[idx] * (1 - cnt / 4.0);
//			//			set_pixel_unsafe(i, j, Math::vec_to_color(color));
//			//		}
//			//	}
//			//}
//		}
//		
//	}
//}

void Graphics::save_as_bmp_file(const char* filename)
{
	int m = 0;
	for (int j = 0; j < height; ++j)
		for (int i = 0; i < width; ++i)
			bmp_manager.SetPixel(i, j, framebuffer[m++]);
	bmp_manager.SaveFile(filename);
}

void Graphics::set_discriptor(GraphicsDiscriptor& d)
{
	disc = d;
	if (d.MSAA4x == true) {
		depthbuffer.reset();
		depthbuffer = std::make_unique<float[]>(4 * width * height);
	}
}

void Graphics::update_by(Keyboard& k)
{
	if (k.KeyIsPressed(VK_SPACE)) {
		// 切换显示模式，不是很好使
		if (disc.display == RenderMode::WireFrame)
			disc.display = RenderMode::FilledTriangle;
		else
			disc.display = RenderMode::WireFrame;
	}
	else if (k.KeyIsPressed(VK_RETURN)) {
		// 按回车键截图
		save_as_bmp_file();
	}
}

void Graphics::line_DDA(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0, dy = y1 - y0;
	float xf = x0, yf = y0;
	int eps = (abs(dx) > abs(dy) ? abs(dx) : abs(dy));
	float xi = (float)dx / eps;
	float yi = (float)dy / eps;
	for (int i = 0; i <= eps; ++i) {
		set_pixel(int(xf + 0.5f), int(yf + 0.5f), Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
		xf += xi;
		yf += yi;
	}

}

void Graphics::line_Bresenham(int x0, int y0, int x1, int y1)
{

	int dx = x1 - x0, dy = y1 - y0;
	int ux = dx > 0 ? 1 : -1;
	int uy = dy > 0 ? 1 : -1;
	dx = abs(dx), dy = abs(dy);
	if (dx > dy) {
		int d = dx - 2 * dy;
		int up = 2 * dx - 2 * dy;
		int down = -2 * dy;
		for (int x = x0, y = y0, i = 0; i <= dx; ++i) {
			set_pixel(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
			x += ux;
			if (d < 0) {
				// 中点在线下方，选上面的点
				y += uy;
				d += up;
			}
			else {
				d += down;
			}
		}
	}
	else {
		// y为长轴
		int d = dy - 2 * dx;
		int up = 2 * dy - 2 * dx;
		int down = -2 * dx;
		for (int x = x0, y = y0, i = 0; i <= dy; ++i) {
			set_pixel(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
			y += uy;
			if (d < 0) {
				x += ux;
				d += up;
			}
			else {
				d += down;
			}
		}
	}
	
}

void Graphics::line_Improved_Bresenham(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0, dy = y1 - y0;
	int ux = dx > 0 ? 1 : -1;
	int uy = dy > 0 ? 1 : -1;
	dx = abs(dx), dy = abs(dy);
	if (dx > dy) {
		int e = -dx;
		for (int x = x0, y = y0, i = 0; i <= dx; ++i) {
			set_pixel(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
			x += ux;
			e += 2 * dy;
			if (e > 0) {
				y += uy;
				e -= 2 * dx;
			}
		}
	}
	else {
		// y为长轴
		int e = -dy;
		for (int x = x0, y = y0, i = 0; i <= dy; ++i) {
			set_pixel(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
			y += uy;
			e += 2 * dx;
			if (e > 0) {
				x += ux;
				e -= 2 * dy;
			}
		}
	}
}

void Graphics::circle(int ox, int oy, int r)
{
	auto circle_point = [&](int x, int y, uint32_t color) {
		// 需要调整 原本是以原点为圆心
		set_pixel(x + ox, y + oy, color);
		set_pixel(x + ox, -y + oy, color);
		set_pixel(-x + ox, y + oy, color);
		set_pixel(-x + ox, -y + oy, color);
		set_pixel(y + ox, x + oy, color);
		set_pixel(y + ox, -x + oy, color);
		set_pixel(-y + ox, x + oy, color);
		set_pixel(-y + ox, -x + oy, color);
	};

	int x = 0, y = r, d = 1 - r;
	while (x <= y) {
		circle_point(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
		if (d < 0) {
			d += 2 * x + 3;
		}
		else {
			d += 2 * x - 2 * y + 5;
			y--;
		}
		x++;
	}
}

void Graphics::ellipse(int ox, int oy, int a, int b)
{
	auto ellipse_point = [&](int x, int y, uint32_t color) {
		// 需要调整
		set_pixel(x + ox, y + oy, color);
		set_pixel(x + ox, -y + oy, color);
		set_pixel(-x + ox, y + oy, color);
		set_pixel(-x + ox, -y + oy, color);
	};
	int x = 0, y = b;
	ellipse_point(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
	
	// 上
	float d1 = b * b + a * a * (-b + 0.25); 
	a *= a;
	b *= b;	
	while (b * (x + 1) < a * (y - 0.5)) {
		if (d1 <= 0) {
			d1 += b * (2 * x + 3);
			x++;
		}
		else {
			d1 += b * (2 * x + 3) + a * (-2 * y + 2);
			x++;
			y--;
		}
		ellipse_point(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
	}
	// 下
	//float d2 = b * (x + 0.5) * (x + 0.5) + a * (y - 1) * (y - 1) - a * b; //椭圆较大时，两侧会变竖直的
	float d2 = b * 0.25 * x + a * (1 - 2 * y);  // 更好用
	while (y > 0) {
		if (d2 <= 0) {
			d2 += b * (2 * x + 2) + a * (-2 * y + 3);
			x++;
			y--;
		}
		else {
			d2 += a * (-2 * y + 3);
			y--;
		}
		ellipse_point(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
	}
}

// 双纽线
void Graphics::lemniscate(int ox, int oy, int a)
{
	auto lemniscate_point = [&](int x, int y, uint32_t color) {
		// 需要调整
		set_pixel(x + ox, y + oy, color);
		set_pixel(x + ox, -y + oy, color);
		set_pixel(-x + ox, y + oy, color);
		set_pixel(-x + ox, -y + oy, color);
	};
	
	// F(x, y) = (x^2 + y^2)^2 - 2 * a^2 * (x^2 - y^2)

	int x = 0, y = 0;
	int aa = a * a;
	lemniscate_point(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));

	// 0 < k < 1 段
	float d1 = 1.5625 - 1.5 * aa; // F(1, 0.5)
	while (
		// 法向量 x分量 <= 0
		std::pow(x + 1, 3) + (y + 0.5) * (y + 0.5) * (x + 1) - aa * (x + 1) < 0
		) {
		if (d1 <= 0) {
			x++;
			y++;
			// 赞美 Mathematica ！
			// 不过其实到这个程度的话我觉得还真不如直接算这个点的误差了，这迭代也太麻烦了
			// F(x + 2, y + 1.5) - F(x + 1, y + 0.5)
			d1 += 37.5f - 2 * aa + 45 * x - 4 * aa * x + 22 * x * x
				+ 4 * x * x * x + 35 * y + 4 * aa * y + 20 * x * y 
				+ 4 * x * x * y + 18 * y * y + 4 * x * y * y + 4 * y * y * y;
		}
		else {
			x++;
			// F(x + 2, y + 0.5) - F(x + 1, y + 0.5)
			d1 += 16.5f - 6 * aa + 29 * x - 4 * aa * x + 18 * x * x
				+ 4 * x * x * x + 6 * y + 4 * x * y + 6 * y * y + 4 * x * y * y;
		}
		lemniscate_point(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
	}

	auto square = [](float x) {
		return x * x;
	};
	auto cube = [](float x) {
		return x * x * x;
	};

	// -1 < k < 0 段
	float d2 = square(square(x + 1) + square(y - 0.5)) - 2 * aa * (square(x + 1) - square(y - 0.5));
	while (
		// 法向量y分量大于x分量
		cube(x + 1) + square(y - 0.5) * (x + 1) - aa * (x + 1)
		< cube(y - 0.5) + square(x + 1) * (y - 0.5) + aa * (y - 0.5)
		) {
		if (d2 <= 0) {
			x++;
			// F(x + 2, y - 0.5) - F(x + 1, y - 0.5)
			d2 += 16.5f - 6 * aa + 29 * x - 4 * aa * x + 18 * x * x
				+ 4 * x * x * x - 6 * y - 4 * x * y + 6 * y * y + 4 * x * y * y;
		}
		else {
			x++;
			y--;
			// F(x + 2, y - 1.5) - F(x + 1, y - 0.5)
			d2 += 37.5f - 2 * aa + 45 * x - 4 * aa * x + 22 * x * x
				+ 4 * x * x * x - 35 * y - 4 * aa * y - 20 * x * y
				- 4 * x * x * y + 18 * y * y + 4 * x * y * y - 4 * y * y * y;
		}
		lemniscate_point(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
	}

	// k < -1 段
	// y为最大位移方向
	float d3 = square(square(x + 0.5) + square(y - 1)) - 2 * aa * (square(x + 0.5) - square(y - 1));
	while (y > 0) {
		if (d3 <= 0) {
			y--;
			x++;
			// F(x + 1.5, y - 2) - F(x + 0.5, y - 1)
			d3 += 37.5f + 2 * aa + 35 * x - 4 * aa * x - 18 * x * x
				+ 4 * x * x * x - 45 * y - 4 * aa * y - 20 * x * y
				- 4 * x * x * y + 22 * y * y + 4 * x * y * y - 4 * y * y * y;
		}
		else {
			y--;
			// F(x + 0.5, y - 2) - F(x + 0.5, y - 1)
			d3 += 16.5f + 6 * aa + 6 * x - 29 * y - 4 * aa * y - 4 * x * y
				- 4 * x * x * y + 18 * y * y - 4 * y * y * y;
		}
		lemniscate_point(x, y, Math::vec_to_color(Vec3f(0.0f, 0.0f, 0.0f)));
	}

}
