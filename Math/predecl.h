#pragma once
#ifndef PRE_DECLARATION_H
#define PRE_DECLARATION_H

#include <cmath>
#include <algorithm>
#include <random>

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
	static Vec3<T> lerp(const Vec3<T>& a, const Vec3<T>& b, float t) noexcept {
		return a + (b - a) * t;
	}




	// 矢量转整数颜色
	template<typename T>
	static uint32_t vec_to_color(const Vec4<T>& color) {
		uint32_t r = (uint32_t)clamp((int)(color.r * 255.0f), 0, 255);
		uint32_t g = (uint32_t)clamp((int)(color.g * 255.0f), 0, 255);
		uint32_t b = (uint32_t)clamp((int)(color.b * 255.0f), 0, 255);
		uint32_t a = (uint32_t)clamp((int)(color.a * 255.0f), 0, 255);
		return (r << 16) | (g << 8) | b | (a << 24);
	}

	// 矢量转换整数颜色
	template<typename T>
	static uint32_t vec_to_color(const Vec3<T>& color) {
		return vec_to_color(color.to_vec4());
	}

};




#endif // !PRE_DECLARATION_H
