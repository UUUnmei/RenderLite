#pragma once
#ifndef MATRIX4_H
#define MATRIX4_H

#include "predecl.h"




template<typename T>
struct Mat4 {
	// data
	Vec4<T> cols[4];   // 直到实际用了一次我才意识到这东西是行主序，，，表面上叫col不能改变它实际上是row的事实，呜呜


	// basic
	Mat4() noexcept;
	Mat4(
		T m00, T m01, T m02, T m03,
		T m10, T m11, T m12, T m13,
		T m20, T m21, T m22, T m23,
		T m30, T m31, T m32, T m33
		) noexcept;
	//explicit Mat4(const Mat4<T>& t) noexcept;
	Mat4<T>& operator=(const Mat4<T>& t) noexcept;
	~Mat4() = default;

	// static construct
	static Mat4<T> identity();
	static Mat4<T> all(T val);
	static Mat4<T> from_cols(
		const Vec4<T>& c0,
		const Vec4<T>& c1,
		const Vec4<T>& c2,
		const Vec4<T>& c3
		);
	static Mat4<T> from_rows(
		const Vec4<T>& r0,
		const Vec4<T>& r1,
		const Vec4<T>& r2,
		const Vec4<T>& r3
	);


	// general
	T& operator()(size_t row, size_t col);
	const T& operator()(size_t row, size_t col) const;
	const Vec4<T> get_row(size_t idx) const;
	const Vec4<T> get_col(size_t idx) const;
	Vec4<T> get_row(size_t idx);
	Vec4<T> get_col(size_t idx);
	Mat4<T> transpose(void) const;

	Mat4<T>& operator*=(const Mat4<T>& rhs) const;

	Mat4<T> operator*(const Mat4<T>& rhs) const;
	Vec4<T> operator*(const Vec4<T>& rhs) const;
	Mat4<T> operator+(const Mat4<T>& rhs) const;
	Mat4<T> operator-(const Mat4<T>& rhs) const;
	

};



using Mat4f = Mat4<float>;
using Mat4d = Mat4<double>;


#include "mat4.inl"

#endif // !MATRIX4_H
