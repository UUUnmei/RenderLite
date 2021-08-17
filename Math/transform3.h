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
	template<typename T>
	static Mat4<float> rotate(const Vec3<T>& axis, float rad); // ָ����ת��

	// view ��ͼ�任 
	template<typename T>
	static Mat4<T> view(const Vec3<T>& eye, const Vec3<T>& dir, const Vec3<T>& up);

	//  ͶӰ�任
	// ����
	// ����zӦΪ��������������Ľ�תΪ����������Ǹ�����ֱ���ã�
	template<typename T>
	static Mat4<float> orth(T left_x, T right_x, T top_y, T bottom_y, T near_z, T far_z); 
	// ͸��
	// ����zӦΪ��������������Ľ�תΪ����������Ǹ�����ֱ���ã�
	// ע�����͸��ͶӰ�������ģ�����Ҫ�ٳ˸�������
	template<typename T>
	static Mat4<float> persp(T fovY_rad, T ratio_wh, T near_z, T far_z);

	// �ӿڱ任
	template<typename T>
	static Mat4<int> viewport(int width, int height);
};


#include "transform3.inl"

#endif // !TRANSFORM3_H
