#pragma once
#ifndef VECTOR2_H
#define VECTOR2_H

#include "predecl.h"

#include <string>
template<typename T>
struct Vec2 {
	//data
	union
	{
		T m[2];
		struct { T x, y; };
		struct { T u, v; };
	};
	// basic
	Vec2() noexcept;
	Vec2(T x, T y) noexcept;
	explicit Vec2(const std::pair<T, T>& t) noexcept;
	Vec2(const Vec2& t) noexcept;
	Vec2<T>& operator=(const Vec2& t) noexcept;
	~Vec2() = default;

	// operator
	bool operator<(const Vec2<T>& rhs) const noexcept;
	const T& operator[] (size_t i) const;
	T& operator[] (size_t i);
	Vec2<T> operator+(const Vec2<T>& rhs) const noexcept;
	Vec2<T> operator-(const Vec2<T>& rhs) const noexcept;
	Vec2<T> operator*(T rhs) const;
	Vec2<T>& operator*=(T rhs);

	// general
	std::string to_str(void) const;
	T cross_z(const Vec2<T>& rhs) const;
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

#include "vec2.inl"   // 实际定义

#endif // !VECTOR2_H
