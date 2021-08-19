#include "predecl.h"


template<typename T>
inline Vec3<T>::Vec3() noexcept
	: x(T()), y(T()), z(T()) { }

template<typename T>
inline Vec3<T>::Vec3(T x, T y, T z) noexcept
	: x(x), y(y), z(z) { }

template<typename T>
inline Vec3<T>::Vec3(const Vec3<T>& t)  noexcept
	: x(t.x), y(t.y), z(t.z) { }



template<typename T>
inline Vec3<T>& Vec3<T>::operator=(const Vec3<T>& t) noexcept
{
	if (this == &t) return *this;
	x = t.x;
	y = t.y;
	z = t.z;
	return *this;
}

template<typename T>
inline const T& Vec3<T>::operator[](size_t i) const 
{
	// assert(i < 3);
	return m[i];
}

template<typename T>
inline T& Vec3<T>::operator[](size_t i)
{
	// assert(i < 3);
	return m[i];
}

template<typename T>
inline bool Vec3<T>::is_zero(void) const noexcept
{
	return !x && !y && !z;
}


template<typename T>
inline bool Vec3<T>::operator==(const Vec3<T>& rhs) const noexcept
{
	return x == rhs.x
		&& y == rhs.y
		&& z == rhs.z;
}



template<typename T>
inline bool Vec3<T>::operator!=(const Vec3<T>& rhs) const noexcept
{
	return x != rhs.x
		|| y != rhs.y
		|| z != rhs.z;
}


template<typename T>
inline bool Vec3<T>::operator<(const Vec3<T>& rhs) const noexcept
{
	return x < rhs.x
		&& y < rhs.y
		&& z < rhs.z;
}

template<typename T>
inline Vec3<T> Vec3<T>::operator+() const noexcept
{
	return Vec3<T>(x, y, z);
}

template<typename T>
inline Vec3<T> Vec3<T>::operator-() const noexcept
{
	return Vec3<T>(-x, -y, -z);
}

template<typename T>
inline bool Vec3<T>::operator!() const noexcept
{
	return is_zero();
}

template<typename T>
inline Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& rhs) noexcept
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

template<typename T>
inline Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& rhs) noexcept
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

template<typename T>
inline Vec3<T>& Vec3<T>::operator*=(const Vec3<T>& rhs) noexcept
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}

template<typename T>
inline Vec3<T>& Vec3<T>::operator/=(const Vec3<T>& rhs)
{
	// assert(rhs.is_zero() == false);
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	return *this;
}

template<typename T>
inline Vec3<T>& Vec3<T>::operator+=(T rhs) noexcept
{
	x += rhs;
	y += rhs;
	z += rhs;
	return *this;
}

template<typename T>
inline Vec3<T>& Vec3<T>::operator-=(T rhs) noexcept
{
	x -= rhs;
	y -= rhs;
	z -= rhs;
	return *this;
}

template<typename T>
inline Vec3<T>& Vec3<T>::operator*=(T rhs) noexcept
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

template<typename T>
inline Vec3<T>& Vec3<T>::operator/=(T rhs)
{
	// assert(rhs != 0);
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}

template<typename T>
inline Vec3<T> Vec3<T>::operator+(const Vec3<T>& rhs) const noexcept
{
	return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
}

template<typename T>
inline Vec3<T> Vec3<T>::operator-(const Vec3<T>& rhs) const noexcept
{
	return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
}

template<typename T>
inline Vec3<T> Vec3<T>::operator*(const Vec3<T>& rhs) const noexcept
{
	return Vec3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
}

template<typename T>
inline Vec3<T> Vec3<T>::operator/(const Vec3<T>& rhs) const
{
	// // assert(rhs.is_zero() == false);
	return Vec3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
}

template<typename T>
inline Vec3<T> Vec3<T>::operator+(T rhs) const noexcept
{
	return Vec3<T>(x + rhs, y + rhs, z + rhs);
}

template<typename T>
inline Vec3<T> Vec3<T>::operator-(T rhs) const noexcept
{
	return Vec3<T>(x - rhs, y - rhs, z - rhs);
}

template<typename T>
inline Vec3<T> Vec3<T>::operator*(T rhs) const noexcept
{
	return Vec3<T>(x * rhs, y * rhs, z * rhs);
}

template<typename T>
inline Vec3<T> Vec3<T>::operator/(T rhs) const
{
	// assert(rhs != 0);
	return Vec3<T>(x / rhs, y / rhs, z / rhs);
}


template<typename T>
inline Vec3<T> operator+(T lhs, const Vec3<T>& rhs) noexcept
{
	return rhs + lhs;
}

template<typename T>
inline Vec3<T> operator*(T lhs, const Vec3<T>& rhs) noexcept
{
	return rhs * lhs;
}

template<typename T>
inline Vec2<T> Vec3<T>::to_vec2(void) const
{
	return Vec2<T>(x, y);
}

template<typename T>
inline Vec4<T> Vec3<T>::to_vec4(void) const 
{
	return Vec4<T>(x, y, z, 1);
}

template<typename T>
inline std::string Vec3<T>::to_str() const
{
	return "["
		+ std::to_string(x) + ", "
		+ std::to_string(y) + ", "
		+ std::to_string(z)
		+ "]";
}

template<typename T>
inline float Vec3<T>::length() const
{
	return std::sqrt(length_squared());
}

template<typename T>
inline float Vec3<T>::length_squared() const
{
	return x * x + y * y + z * z;
}

template<typename T>
inline T Vec3<T>::max() const
{
	return std::max({ x, y, z });
}

template<typename T>
inline Vec3<T> Vec3<T>::abs() const
{
	return Vec3<T>(
		x > 0 ? x : -x,
		y > 0 ? y : -y,
		z > 0 ? z : -z);
}

template<typename T>
inline T Vec3<T>::abs_max() const
{
	return abs().max();
}

template<typename T>
inline Vec3<T> Vec3<T>::normalize() const
{
	if (is_zero() == true)
		return Vec3<T>(x, y, z);

	return *this / length();
}

template<typename T>
inline T Vec3<T>::sum() const
{
	return x + y + z;
}

template<typename T>
inline T Vec3<T>::abs_sum() const
{
	return abs().sum();
}

template<typename T>
inline T Vec3<T>::dot(const Vec3<T>& rhs) const
{
	return (*this * rhs).sum();
}

template<typename T>
inline Vec3<T> Vec3<T>::cross(const Vec3<T>& rhs) const
{
	return Vec3<T>(
		y * rhs.z - z * rhs.y,
		z * rhs.x - x * rhs.z,
		x * rhs.y - y * rhs.x);
}

// 有时只想要叉乘后的z值
template<typename T>
inline T Vec3<T>::cross_z(const Vec3<T>& rhs) const
{
	return x * rhs.y - y * rhs.x;
}