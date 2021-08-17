#pragma once

#include "predecl.h"
#include "mat4.h"

template<typename T>
inline Mat4<T>::Mat4() noexcept
	: Mat4(identity()) { }

template<typename T>
inline Mat4<T>::Mat4(
	T m00, T m01, T m02, T m03, 
	T m10, T m11, T m12, T m13,
	T m20, T m21, T m22, T m23, 
	T m30, T m31, T m32, T m33) noexcept
	: cols{{m00, m01, m02, m03},
		   {m10, m11, m12, m13},
		   {m20, m21, m22, m23},
		   {m30, m31, m32, m33}} { }

template<typename T>
inline Mat4<T>::Mat4(const Mat4& t) noexcept
	:cols(t.cols[0], t.cols[1], t.cols[2], t.cols[3]) { }


template<typename T>
inline Mat4<T>& Mat4<T>::operator=(const Mat4& t) noexcept
{
	if (this == &t) return *this;
	for (int i = 0; i < 4; ++i)
		cols[i] = t.cols[i];
	return *this;
}

template<typename T>
inline Mat4<T> Mat4<T>::identity()
{
	return Mat4<T>(
		T(1), 0, 0, 0,
		0, T(1), 0, 0,
		0, 0, T(1), 0,
		0, 0, 0, T(1)
		);
}

template<typename T>
inline Mat4<T> Mat4<T>::from_cols(const Vec4<T>& c0, const Vec4<T>& c1, const Vec4<T>& c2, const Vec4<T>& c3)
{
	return Mat4<T>(
		c0.x, c1.x, c2.x, c3.x,
		c0.y, c1.y, c2.y, c3.y,
		c0.z, c1.z, c2.z, c3.z,
		c0.w, c1.w, c2.w, c3.w
		);
}

template<typename T>
inline T& Mat4<T>::operator()(size_t row, size_t col) 
{
	return cols[row][col];
}

template<typename T>
inline const T& Mat4<T>::operator()(size_t row, size_t col) const 
{
	return cols[row][col];
}

template<typename T>
inline Vec4<T> Mat4<T>::get_row(size_t idx) const
{
	return Vec4<T>(cols[0][idx], cols[1][idx], cols[2][idx], cols[3][idx]);
}

template<typename T>
inline Vec4<T> Mat4<T>::get_col(size_t idx) const
{
	return cols[idx];
}

template<typename T>
inline Mat4<T>& Mat4<T>::operator*=(const Mat4<T>& rhs) const
{
	return *this = *this * rhs;
}

// matrix = matrix * matrix
template<typename T>
inline Mat4<T> Mat4<T>::operator*(const Mat4<T>& rhs) const
{
	Mat4<T> ret;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			ret(i, j) = get_row(i).dot(rhs.get_col(j));
		}
	}
	return ret;
}

// vector = matrix * vector
template<typename T>
inline Vec4<T> Mat4<T>::operator*(const Vec4<T>& rhs) const
{
	Vec4<T> c0 = cols[0] * rhs.x;
	Vec4<T> c1 = cols[1] * rhs.y;
	Vec4<T> c2 = cols[2] * rhs.z;
	Vec4<T> c3 = cols[3] * rhs.w;

	return c0 + c1 + c2 + c3;
}

template<typename T>
inline Mat4<T> Mat4<T>::operator+(const Mat4<T>& rhs) const
{
	return Mat4<T>::from_cols(
		cols[0] + rhs.cols[0],
		cols[1] + rhs.cols[1],
		cols[2] + rhs.cols[2],
		cols[3] + rhs.cols[3]
	);
}

template<typename T>
inline Mat4<T> Mat4<T>::operator-(const Mat4<T>& rhs) const
{
	return Mat4<T>::from_cols(
		cols[0] - rhs.cols[0],
		cols[1] - rhs.cols[1],
		cols[2] - rhs.cols[2],
		cols[3] - rhs.cols[3]
		);
}


