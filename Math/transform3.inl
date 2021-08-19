#pragma once
#include "predecl.h"
#include "transform3.h"



template<typename T>
inline Mat4<T> Transform3::translate(const Vec3<T>& bias)
{
	return Transform3::translate(bias.x, bias.y, bias.z);
}

template<typename T>
inline Mat4<T> Transform3::translate(T x, T y, T z)
{
	return Mat4<T>(
		T(1), 0, 0, x,
		0, T(1), 0, y,
		0, 0, T(1), z,
		0, 0, 0, T(1)
		);
}

template<typename T>
inline Mat4<T> Transform3::scale(const Vec3<T>& bias)
{
	return Transform3::scale(bias.x, bias.y, bias.z);
}

template<typename T>
inline Mat4<T> Transform3::scale(T x, T y, T z)
{
	return Mat4<T>(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, T(1)
		);
}

inline Mat4<float> Transform3::rotate_x(float rad)
{
	float c = std::cos(rad), s = std::sin(rad);
	return Mat4<float>(
		1, 0,  0, 0, 
		0, c, -s, 0,
		0, s,  c, 0,
		0, 0,  0, 1
		);

}

inline Mat4<float> Transform3::rotate_y(float rad)
{
	float c = std::cos(rad), s = std::sin(rad);
	return Mat4<float>(
		c,  0, s, 0,
		0,  1, 0, 0,
		-s, 0, c, 0,
		0,  0, 0, 1
		);
}

inline Mat4<float> Transform3::rotate_z(float rad)
{
	float c = std::cos(rad), s = std::sin(rad);
	return Mat4<float>(
		c, -s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
}


// Rodrigues' Rotation Formula
// 这里直接写展开后的形式
inline Mat4<float> Transform3::rotate(const Vec3<float>& axis, float rad)
{
	Mat4<float> ret;
	const Vec3<float> k = axis.normalize();
	const float s = std::sin(rad), c = std::cos(rad);
	float x = k.x;
	float y = k.y;
	float z = k.z;

	ret.cols[0][0] = x * x + (1 - x * x) * c;
	ret.cols[0][1] = x * y * (1 - c) - z * s;
	ret.cols[0][2] = x * z * (1 - c) + y * s;
	ret.cols[0][3] = 0;

	ret.cols[1][0] = y * x * (1 - c) + z * s;
	ret.cols[1][1] = y * y + (1 - y * y) * c;
	ret.cols[1][2] = y * z * (1 - c) - x * s;
	ret.cols[1][3] = 0;

	ret.cols[2][0] = z * x * (1 - c) - y * s;
	ret.cols[2][1] = z * y * (1 - c) + x * s;
	ret.cols[2][2] = z * z + (1 - z * z) * c;
	ret.cols[2][3] = 0;

	ret.cols[3][0] = 0;
	ret.cols[3][1] = 0;
	ret.cols[3][2] = 0;
	ret.cols[3][3] = 1;

	return ret;

}


inline Mat4<float> Transform3::view(const Vec3<float>& eye, const Vec3<float>& dir, const Vec3<float>& up)
{
	const Vec3<float> g = dir.normalize();
	const Vec3<float> t = up.normalize();
	const Vec3<float> a = g.cross(t).normalize();

	return Mat4<float>(
		a.x, a.y, a.z, 0,
		t.x, t.y, t.z, 0,
		-g.x, -g.y, -g.z, 0,
		0, 0, 0, 1
		)
		* Mat4<float>(
			1, 0, 0, -eye.x,
			0, 1, 0, -eye.y,
			0, 0, 1, -eye.z,
			0, 0, 0, 1
		);
}

inline Mat4<float> Transform3::orth(float left_x, float right_x, float top_y, float bottom_y, float near_z, float far_z)
{
	near_z = near_z > 0 ? -near_z : near_z;
	far_z = far_z > 0 ? -far_z : far_z;

	float ixx = 1.0 / (right_x - left_x);
	float iyy = 1.0 / (top_y - bottom_y);
	float izz = 1.0 / (near_z - far_z);
	return Mat4<float>(
		2 * ixx, 0, 0, -(right_x + left_x) * 1.0 * ixx,
		0, 2 * iyy, 0, -(top_y + bottom_y) * 1.0 * iyy,
		0, 0, 2 * izz, -(near_z + far_z) * 1.0 * izz,
		0, 0, 0, 1
		);
}



inline Mat4<float> Transform3::persp(float fovY_rad, float ratio_wh, float near_z, float far_z)
{

	float n = near_z > 0 ? -near_z : near_z;
	float f = far_z > 0 ? -far_z : far_z;
	float t = fabs(near_z) * std::tan(fovY_rad / 2.0);
	float b = -t;
	float r = t * ratio_wh;
	float l = -r;

	return Mat4<float>(
		2 * n / (r - l), 0, (l + r) / (l - r), 0,
		0, 2 * n / (t - b), (b + t) / (b - t), 0,
		0, 0, (n + f) / (n - f), 2 * f * n / (f - n),
		0, 0, 1, 0
		);
}

inline Mat4<float> Transform3::viewport(int width, int height)
{
	return Mat4<float>(
		width / 2, 0, 0, width / 2,
		0, -height / 2, 0, height / 2,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
}
