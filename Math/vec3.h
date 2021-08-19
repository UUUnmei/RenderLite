#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#include "predecl.h"

#include <string>

template<typename T>
struct Vec3 {

	//data
	union 
	{
		T m[3];
		struct { T x, y, z; };
		struct { T u, v, w; };
		struct { T r, g, b; };
	};
	// basic
	Vec3() noexcept;
	Vec3(T x, T y, T z) noexcept;
	Vec3(const Vec3<T>& t) noexcept;
	Vec3<T>& operator=(const Vec3<T>& t) noexcept;
	~Vec3() = default;

	// operator
	const T& operator[] (size_t i) const;
	T& operator[] (size_t i);

	bool operator==(const Vec3<T>& rhs) const noexcept;
	bool operator!=(const Vec3<T>& rhs) const noexcept;
	bool operator<(const Vec3<T>& rhs) const noexcept;

	Vec3<T> operator+() const noexcept;			// 正号
	Vec3<T> operator-() const noexcept;			// 负号
	bool operator!() const noexcept;

	Vec3<T>& operator+=(const Vec3<T>& rhs) noexcept;
	Vec3<T>& operator-=(const Vec3<T>& rhs) noexcept;
	Vec3<T>& operator*=(const Vec3<T>& rhs) noexcept;		// pairwise 不是点积
	Vec3<T>& operator/=(const Vec3<T>& rhs);
	Vec3<T>& operator+=(T rhs) noexcept;
	Vec3<T>& operator-=(T rhs) noexcept;
	Vec3<T>& operator*=(T rhs) noexcept;
	Vec3<T>& operator/=(T rhs) ;

	Vec3<T> operator+(const Vec3<T>& rhs) const noexcept;
	Vec3<T> operator-(const Vec3<T>& rhs) const noexcept;
	Vec3<T> operator*(const Vec3<T>& rhs) const noexcept;				// pairwise 不是点积
	Vec3<T> operator/(const Vec3<T>& rhs) const;
	Vec3<T> operator+(T rhs) const noexcept;
	Vec3<T> operator-(T rhs) const noexcept;
	Vec3<T> operator*(T rhs) const noexcept;
	Vec3<T> operator/(T rhs) const;

	// 还有 T op Vec3<T> 的重载  op包括 + * 

	// general 
	bool is_zero(void) const noexcept;
	std::string to_str(void) const;
	Vec2<T> to_vec2(void) const;
	Vec4<T> to_vec4(void) const;
	float length() const;
	float length_squared() const;
	Vec3<T> abs() const;
	T max() const;
	T abs_max() const;
	T sum() const;
	T abs_sum() const;
	Vec3<T> normalize() const;
	T dot(const Vec3<T>& rhs) const;
	Vec3<T> cross(const Vec3<T>& rhs) const;
	T cross_z(const Vec3<T>& rhs) const;
	
};

using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;


#include "vec3.inl"   // 实际定义

#endif // !VECTOR3_H
