#pragma once
#ifndef TRANSFORM3_H
#define TRANSFORM3_H

#include "predecl.h"

// 用4*4矩阵表示3维变换
// 右手系
// 用的时候 matrix * vector

struct Transform3 {

	// 平移
	template<typename T>
	static Mat4<T> translate(const Vec3<T>& bias);
	template<typename T>
	static Mat4<T> translate(T x, T y, T z);

	// 缩放
	template<typename T>
	static Mat4<T> scale(const Vec3<T>& bias);
	template<typename T>
	static Mat4<T> scale(T x, T y, T z);


	// 旋转
	// 默认逆时针
	static Mat4<float> rotate_x(float rad);
	static Mat4<float> rotate_y(float rad);
	static Mat4<float> rotate_z(float rad);
	static Mat4<float> rotate(const Vec3<float>& axis, float rad); // 指定旋转轴

	// view 视图变换 
	static Mat4<float> view(const Vec3<float>& eye, const Vec3<float>& dir, const Vec3<float>& up);

	//  投影变换
	// 正交
	static Mat4<float> orth(float left_x, float right_x, float top_y, float bottom_y, float near_z, float far_z);
	// 透视
	// 注意这个透视投影是完整的，不需要再乘个正交的 
	// eq to glm::perspectiveRH_NO
	static Mat4<float> persp(float fovY_rad, float ratio_wh, float near_z, float far_z);

	// 视口变换
	static Mat4<float> viewport(int width, int height);
};


#include "transform3.inl"

#endif // !TRANSFORM3_H
