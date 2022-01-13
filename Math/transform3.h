#pragma once
#ifndef TRANSFORM3_H
#define TRANSFORM3_H

#include "predecl.h"

// ��4*4�����ʾ3ά�任
// ����ϵ
// �õ�ʱ�� matrix * vector

struct Transform3 {

	// ƽ��
	template<typename T>
	static Mat4<T> translate(const Vec3<T>& bias);
	template<typename T>
	static Mat4<T> translate(T x, T y, T z);

	// ����
	template<typename T>
	static Mat4<T> scale(const Vec3<T>& bias);
	template<typename T>
	static Mat4<T> scale(T x, T y, T z);


	// ��ת
	// Ĭ����ʱ��
	static Mat4<float> rotate_x(float rad);
	static Mat4<float> rotate_y(float rad);
	static Mat4<float> rotate_z(float rad);
	static Mat4<float> rotate(const Vec3<float>& axis, float rad); // ָ����ת��

	// view ��ͼ�任 
	static Mat4<float> view(const Vec3<float>& eye, const Vec3<float>& dir, const Vec3<float>& up);

	//  ͶӰ�任
	// ����
	static Mat4<float> orth(float left_x, float right_x, float top_y, float bottom_y, float near_z, float far_z);
	// ͸��
	// ע�����͸��ͶӰ�������ģ�����Ҫ�ٳ˸������� 
	// eq to glm::perspectiveRH_NO
	static Mat4<float> persp(float fovY_rad, float ratio_wh, float near_z, float far_z);

	// �ӿڱ任
	static Mat4<float> viewport(int width, int height);
};


#include "transform3.inl"

#endif // !TRANSFORM3_H
