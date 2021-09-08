#pragma once
#ifndef PRE_DECLARATION_H
#define PRE_DECLARATION_H

#include <cmath>
#include <algorithm>
#include <random>

#include <cstdint>

template<typename T> struct Vec2;
template<typename T> struct Vec3;
template<typename T> struct Vec4;

template<typename T> struct Mat4;


struct Transform3;



static const constexpr float EPSILON = 1e-5;
static const constexpr double PI = 3.141592653589793238462643383;

struct Math {
	template<typename T>
	static bool is_close_z_enough(T x) {
		return fabs(x) < EPSILON;
	}

	template<typename T>
	static bool is_close_enough(T x, T y) {
		return is_close_z_enough(x - y);
	}

	static double rad2deg(double rad) noexcept {
		return rad * 180.0 / PI;
	}

	static double deg2rad(double deg) noexcept {
		return deg / 180.0 * PI;
	}


	template<typename T>
	static T clamp(T val, T min_v, T max_v) noexcept {
		return std::min(std::max(val, min_v), max_v);
	}

	template<typename T>
	static T lerp(const T& a, const T& b, float t) noexcept {
		return a + (b - a) * t;
	}




	// 矢量转整数颜色
	template<typename T>
	static uint32_t vec_to_color(const Vec4<T>& color) noexcept  {

		//uint32_t r = (uint32_t)clamp((int)(color.r * 255.0f), 0, 255);
		//uint32_t g = (uint32_t)clamp((int)(color.g * 255.0f), 0, 255);
		//uint32_t b = (uint32_t)clamp((int)(color.b * 255.0f), 0, 255);
		//uint32_t a = (uint32_t)clamp((int)(color.a * 255.0f), 0, 255);

		uint32_t r = std::min((uint32_t)(color.r * 255.0f), 255u);
		uint32_t g = std::min((uint32_t)(color.g * 255.0f), 255u);
		uint32_t b = std::min((uint32_t)(color.b * 255.0f), 255u);
		uint32_t a = std::min((uint32_t)(color.a * 255.0f), 255u);

		return (r << 16) | (g << 8) | b | (a << 24);
	}

	// 矢量转换整数颜色
	template<typename T>
	static uint32_t vec_to_color(const Vec3<T>& color) noexcept  {
		//uint32_t r = (uint32_t)clamp((int)(color.r * 255.0f), 0, 255);
		//uint32_t g = (uint32_t)clamp((int)(color.g * 255.0f), 0, 255);
		//uint32_t b = (uint32_t)clamp((int)(color.b * 255.0f), 0, 255);
		uint32_t r = std::min((uint32_t)(color.r * 255.0f), 255u);
		uint32_t g = std::min((uint32_t)(color.g * 255.0f), 255u);
		uint32_t b = std::min((uint32_t)(color.b * 255.0f), 255u);
		return (r << 16) | (g << 8) | b | (255 << 24);
	}

	// //这里还不能用Vec3类型。。
	//static Vec3<float> color_from_uint(uint32_t color) {
	//	int mask = 0xFF;
	//	float b = (color & mask) / 255.0f;
	//	color >>= 8;
	//	float g = (color & mask) / 255.0f;
	//	color >>= 8;
	//	float r = (color & mask) / 255.0f;
	//	return Vec3<float>(r, g, b);
	//}

	static uint32_t BilinearInterp(
		uint32_t tl, uint32_t tr, 
		uint32_t bl, uint32_t br, 
		int32_t distx, int32_t disty) noexcept {

/*
       dx = (a - c11) / (c21 - c11) , dy = (c - c11) / (c12 - c11)
	c11---- a ---- c21
 dy|		|		|
	|------ c -----	|
	|		|		|
	|		|		|
	c12---- b ---- c22
 
 
	a = c11 + (c21 - c11) * dx;
	b = c12 + (c22 - c12) * dx;
	c = a + (b - a) * dy;
==>
	c = (1 - dx)(1 - dy) * c11 + (1 - dx)dy * c12
	  + (1 - dy)dx * c21 + dxdy * c22
		
*/

		uint32_t f, r;
		int32_t distxy = distx * disty;								//dxdy
		int32_t distxiy = (distx << 8) - distxy;  /* distx * (256 - disty) */   // (1 - dy)dx
 		int32_t distixy = (disty << 8) - distxy;  /* disty * (256 - distx) */   // (1 - dx)dy
		int32_t distixiy = 256 * 256 - (disty << 8) - (distx << 8) + distxy;    // (1 - dx)(1 - dy)
		r = (tl & 0x000000ff) * distixiy + (tr & 0x000000ff) * distxiy			// B
			+ (bl & 0x000000ff) * distixy + (br & 0x000000ff) * distxy;
		f = (tl & 0x0000ff00) * distixiy + (tr & 0x0000ff00) * distxiy			// G
			+ (bl & 0x0000ff00) * distixy + (br & 0x0000ff00) * distxy;
		r |= f & 0xff000000;
		tl >>= 16; tr >>= 16; bl >>= 16; br >>= 16; r >>= 16;
		f = (tl & 0x000000ff) * distixiy + (tr & 0x000000ff) * distxiy			// R
			+ (bl & 0x000000ff) * distixy + (br & 0x000000ff) * distxy;	
		r |= f & 0x00ff0000;	
		f = (tl & 0x0000ff00) * distixiy + (tr & 0x0000ff00) * distxiy			// A 
			+ (bl & 0x0000ff00) * distixy + (br & 0x0000ff00) * distxy;
		r |= f & 0xff000000;
		return r;
	}


	static float qpow(float x, int n) noexcept {
		float res = 1;
		while (n) {
			if (n & 1) {
				res *= x;
			}
			x *= x;
			n >>= 1;
		}
		return res;
	}

	
};




#endif // !PRE_DECLARATION_H
