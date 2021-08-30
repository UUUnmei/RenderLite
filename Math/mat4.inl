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
	: cols{{m00, m10, m20, m30},
		   {m01, m11, m21, m31},
		   {m02, m12, m22, m32},
		   {m03, m13, m23, m33}} { }

//template<typename T>
//inline Mat4<T>::Mat4(const Mat4<T>& t) noexcept
//	:cols{ t.cols[0], t.cols[1], t.cols[2], t.cols[3] } { }
//

template<typename T>
inline Mat4<T>& Mat4<T>::operator=(const Mat4<T>& t) noexcept
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
inline Mat4<T> Mat4<T>::all(T val)
{
	return Mat4<T>(
		val, val, val, val,
		val, val, val, val,
		val, val, val, val,
		val, val, val, val
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
inline Mat4<T> Mat4<T>::from_rows(const Vec4<T>& r0, const Vec4<T>& r1, const Vec4<T>& r2, const Vec4<T>& r3)
{
	return Mat4<T>(
		r0.x, r0.y, r0.z, r0.w,
		r1.x, r1.y, r1.z, r1.w,
		r2.x, r2.y, r2.z, r2.w,
		r3.x, r3.y, r3.z, r3.w
		);
}

template<typename T>
inline T& Mat4<T>::operator()(size_t row, size_t col) 
{
	return cols[col][row];
}

template<typename T>
inline const T& Mat4<T>::operator()(size_t row, size_t col) const 
{
	return cols[col][row];
}

template<typename T>
inline const Vec4<T> Mat4<T>::get_row(size_t idx) const
{
	return Vec4<T>(cols[0][idx], cols[1][idx], cols[2][idx], cols[3][idx]);
}

template<typename T>
inline const Vec4<T> Mat4<T>::get_col(size_t idx) const
{
	return cols[idx];
}

template<typename T>
inline Vec4<T> Mat4<T>::get_row(size_t idx)
{
	return Vec4<T>(cols[0][idx], cols[1][idx], cols[2][idx], cols[3][idx]);
}

template<typename T>
inline Vec4<T> Mat4<T>::get_col(size_t idx)
{
	return cols[idx];
}

template<typename T>
inline Mat4<T> Mat4<T>::transpose(void) const
{
	return from_rows(cols[0], cols[1], cols[2], cols[3]);
}

template<typename T>
inline Mat4<T> Mat4<T>::inverse(void) const
{
	// »ùÓÚ°éËæ¾ØÕó
	T blk00 = cols[2][2] * cols[3][3] - cols[3][2] * cols[2][3];
	T blk02 = cols[1][2] * cols[3][3] - cols[3][2] * cols[1][3];
	T blk03 = cols[1][2] * cols[2][3] - cols[2][2] * cols[1][3];

	T blk04 = cols[2][1] * cols[3][3] - cols[3][1] * cols[2][3];
	T blk06 = cols[1][1] * cols[3][3] - cols[3][1] * cols[1][3];
	T blk07 = cols[1][1] * cols[2][3] - cols[2][1] * cols[1][3];

	T blk08 = cols[2][1] * cols[3][2] - cols[3][1] * cols[2][2];
	T blk10 = cols[1][1] * cols[3][2] - cols[3][1] * cols[1][2];
	T blk11 = cols[1][1] * cols[2][2] - cols[2][1] * cols[1][2];

	T blk12 = cols[2][0] * cols[3][3] - cols[3][0] * cols[2][3];
	T blk14 = cols[1][0] * cols[3][3] - cols[3][0] * cols[1][3];
	T blk15 = cols[1][0] * cols[2][3] - cols[2][0] * cols[1][3];

	T blk16 = cols[2][0] * cols[3][2] - cols[3][0] * cols[2][2];
	T blk18 = cols[1][0] * cols[3][2] - cols[3][0] * cols[1][2];
	T blk19 = cols[1][0] * cols[2][2] - cols[2][0] * cols[1][2];

	T blk20 = cols[2][0] * cols[3][1] - cols[3][0] * cols[2][1];
	T blk22 = cols[1][0] * cols[3][1] - cols[3][0] * cols[1][1];
	T blk23 = cols[1][0] * cols[2][1] - cols[2][0] * cols[1][1];

	Vec4<T> fac0(blk00, blk00, blk02, blk03);
	Vec4<T> fac1(blk04, blk04, blk06, blk07);
	Vec4<T> fac2(blk08, blk08, blk10, blk11);
	Vec4<T> fac3(blk12, blk12, blk14, blk15);
	Vec4<T> fac4(blk16, blk16, blk18, blk19);
	Vec4<T> fac5(blk20, blk20, blk22, blk23);

	Vec4<T> vec0(cols[1][0], cols[0][0], cols[0][0], cols[0][0]);
	Vec4<T> vec1(cols[1][1], cols[0][1], cols[0][1], cols[0][1]);
	Vec4<T> vec2(cols[1][2], cols[0][2], cols[0][2], cols[0][2]);
	Vec4<T> vec3(cols[1][3], cols[0][3], cols[0][3], cols[0][3]);

	Vec4<T> inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
	Vec4<T> inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
	Vec4<T> inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
	Vec4<T> inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

	Vec4<T> sign_a(+1, -1, +1, -1);
	Vec4<T> sign_b(-1, +1, -1, +1);

	Mat4<T> adj = from_cols(inv0 * sign_a, inv1 * sign_b, inv2 * sign_a, inv3 * sign_b);
	float d = cols[0].dot(adj.get_row(0));
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			adj(i, j) /= d;
	return adj;

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
	Vec4<T> c0 = rhs.x * cols[0];
	Vec4<T> c1 = rhs.y * cols[1];
	Vec4<T> c2 = rhs.z * cols[2];
	Vec4<T> c3 = rhs.w * cols[3];

	return c0 + c1 + c2 + c3;
}

template<typename T>
inline Vec4<T> operator*(const Vec4<T>& lhs, const Mat4<T>& rhs) {
	return Vec4<T>(
		lhs.dot(rhs.get_col(0)),
		lhs.dot(rhs.get_col(1)),
		lhs.dot(rhs.get_col(2)),
		lhs.dot(rhs.get_col(3))
		);
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


