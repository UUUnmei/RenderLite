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
	template<typename T>
	static Mat4<float> rotate(const Vec3<T>& axis, float rad); // 指定旋转轴

	// view 视图变换 
	template<typename T>
	static Mat4<T> view(const Vec3<T>& eye, const Vec3<T>& dir, const Vec3<T>& up);

	//  投影变换
	// 正交
	// 两个z应为负数（如果是正的将转为负数，如果是负数就直接用）
	template<typename T>
	static Mat4<float> orth(T left_x, T right_x, T top_y, T bottom_y, T near_z, T far_z); 
	// 透视
	// 两个z应为负数（如果是正的将转为负数，如果是负数就直接用）
	// 注意这个透视投影是完整的，不需要再乘个正交的
	template<typename T>
	static Mat4<float> persp(T fovY_rad, T ratio_wh, T near_z, T far_z);

	// 视口变换
	template<typename T>
	static Mat4<int> viewport(int width, int height);
};


#include "transform3.inl"

#endif // !TRANSFORM3_H
