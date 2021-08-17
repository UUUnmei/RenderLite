#pragma once
#ifndef VECTOR4_H
#define VECTOR4_H

// 还要调整，4维的其实不需要那么多操作

#include "predecl.h"

#include <string>

template<typename T>
struct Vec4 {
	//data
	union
	{
		T m[4];
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
	};

	// ! unique   齐次化
	Vec4<T> homogenize() const;

	// basic
	Vec4() noexcept;
	Vec4(T x, T y, T z, T w) noexcept;
	explicit Vec4(const Vec4& t) noexcept;
	Vec4<T>& operator=(const Vec4& t) noexcept;
	~Vec4() = default;

	// operator
	const T& operator[] (size_t i) const;
	T& operator[] (size_t i);

	bool operator==(const Vec4<T>&rhs) noexcept;
	bool operator!=(const Vec4<T>&rhs) noexcept;
	bool operator<(const Vec4<T>&rhs) noexcept;

	Vec4<T> operator+() noexcept;			// 正号
	Vec4<T> operator-() noexcept;			// 负号
	bool operator!() noexcept;

	Vec4<T>& operator+=(const Vec4<T>&rhs) noexcept;
	Vec4<T>& operator-=(const Vec4<T>&rhs) noexcept;
	Vec4<T>& operator*=(const Vec4<T>&rhs) noexcept;		// pairwise 不是点积
	Vec4<T>& operator/=(const Vec4<T>&rhs);
	Vec4<T>& operator+=(T rhs) noexcept;
	Vec4<T>& operator-=(T rhs) noexcept;
	Vec4<T>& operator*=(T rhs) noexcept;
	Vec4<T>& operator/=(T rhs);

	Vec4<T> operator+(const Vec4<T>&rhs) noexcept;
	Vec4<T> operator-(const Vec4<T>&rhs) noexcept;
	Vec4<T> operator*(const Vec4<T>&rhs) noexcept;				// pairwise 不是点积
	Vec4<T> operator/(const Vec4<T>&rhs);
	Vec4<T> operator+(T rhs) noexcept;
	Vec4<T> operator-(T rhs) noexcept;
	Vec4<T> operator*(T rhs) noexcept;
	Vec4<T> operator/(T rhs);

	// 还有 T op Vec4<T> 的重载  op包括 + * 

	// general 
	bool is_zero(void) const noexcept;
	std::string to_str(void) const;
	Vec2<T> to_vec2(void) const;
	Vec3<T> to_vec3(void) const;
	float length() const;
	float length_squared() const;
	Vec4<T> abs() const;
	T max() const;
	T abs_max() const;
	T sum() const;
	T abs_sum() const;
	Vec4<T> normalize() const;
	T dot(const Vec4<T>& rhs) const;
	//Vec4<T> cross(const Vec4<T>& rhs) const;
	//float cross_z(const Vec4<T>& rhs) const;
};

using Vec4i = Vec4<int>;
using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;

#include "vec4.inl"

#endif // !VECTOR4_H
