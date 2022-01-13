#include "predecl.h"
#include "vec4.h"



template<typename T>
inline Vec4<T> Vec4<T>::homogenize() const
{
	if (!w) return Vec4<T>(x, y, z, w);
	return Vec4<T>(x / w, y / w, z / w, 1);
}

template<typename T>
inline Vec4<T>::Vec4() noexcept
	: x(T()), y(T()), z(T()), w(T()) { }

template<typename T>
inline Vec4<T>::Vec4(T x) noexcept
	: Vec4(x, x, x, x) { }

template<typename T>
inline Vec4<T>::Vec4(T x, T y, T z, T w) noexcept
	: x(x), y(y), z(z), w(w) { }

template<typename T>
inline Vec4<T>::Vec4(const Vec4<T>& t)  noexcept
	: x(t.x), y(t.y), z(t.z), w(t.w) { }



template<typename T>
inline Vec4<T>& Vec4<T>::operator=(const Vec4<T>& t) noexcept
{
	if (this == &t) return *this;
	x = t.x;
	y = t.y;
	z = t.z;
	w = t.w;
	return *this;
}

template<typename T>
inline const T& Vec4<T>::operator[](size_t i) const
{
	// assert(i < 4);
	return m[i];
}

template<typename T>
inline T& Vec4<T>::operator[](size_t i)
{
	// assert(i < 4);
	return m[i];
}

template<typename T>
inline bool Vec4<T>::is_zero(void) const noexcept
{
	return !x && !y && !z && !w;
}


template<typename T>
inline bool Vec4<T>::operator==(const Vec4<T>& rhs) const noexcept
{
	return x == rhs.x
		&& y == rhs.y
		&& z == rhs.z
		&& w == rhs.w;
}



template<typename T>
inline bool Vec4<T>::operator!=(const Vec4<T>& rhs) const noexcept
{
	return x != rhs.x
		|| y != rhs.y
		|| z != rhs.z
		|| w != rhs.w;
}




template<typename T>
inline bool Vec4<T>::operator<(const Vec4<T>& rhs) const noexcept
{
	return x < rhs.x
		&& y < rhs.y
		&& z < rhs.z
		&& w < rhs.w;
}

template<typename T>
inline Vec4<T> Vec4<T>::operator+() noexcept
{
	return Vec4<T>(x, y, z, w);
}

template<typename T>
inline Vec4<T> Vec4<T>::operator-() noexcept
{
	return Vec4<T>(-x, -y, -z, -w);
}

template<typename T>
inline bool Vec4<T>::operator!() noexcept
{
	return is_zero();
}

template<typename T>
inline Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& rhs) noexcept
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

template<typename T>
inline Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& rhs) noexcept
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

template<typename T>
inline Vec4<T>& Vec4<T>::operator*=(const Vec4<T>& rhs) noexcept
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;
	return *this;
}

template<typename T>
inline Vec4<T>& Vec4<T>::operator/=(const Vec4<T>& rhs)
{
	// assert(rhs.is_zero() == false);
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	w /= rhs.w;
	return *this;
}

template<typename T>
inline Vec4<T>& Vec4<T>::operator+=(T rhs) noexcept
{
	x += rhs;
	y += rhs;
	z += rhs;
	w += rhs;
	return *this;
}

template<typename T>
inline Vec4<T>& Vec4<T>::operator-=(T rhs) noexcept
{
	x -= rhs;
	y -= rhs;
	z -= rhs;
	w -= rhs;
	return *this;
}

template<typename T>
inline Vec4<T>& Vec4<T>::operator*=(T rhs) noexcept
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;
	return *this;
}

template<typename T>
inline Vec4<T>& Vec4<T>::operator/=(T rhs)
{
	// assert(rhs != 0);
	x /= rhs;
	y /= rhs;
	z /= rhs;
	w /= rhs;
	return *this;
}

template<typename T>
inline Vec4<T> Vec4<T>::operator+(const Vec4<T>& rhs) const noexcept
{
	return Vec4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

template<typename T>
inline Vec4<T> Vec4<T>::operator-(const Vec4<T>& rhs) const noexcept
{
	return Vec4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

template<typename T>
inline Vec4<T> Vec4<T>::operator*(const Vec4<T>& rhs) const noexcept
{
	return Vec4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
}

template<typename T>
inline Vec4<T> Vec4<T>::operator/(const Vec4<T>& rhs) const
{
	// // assert(rhs.is_zero() == false);
	return Vec4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
}

template<typename T>
inline Vec4<T> Vec4<T>::operator+(T rhs) const noexcept
{
	return Vec4<T>(x + rhs, y + rhs, z + rhs, w + rhs);
}

template<typename T>
inline Vec4<T> Vec4<T>::operator-(T rhs) const noexcept
{
	return Vec4<T>(x - rhs, y - rhs, z - rhs, w - rhs);
}

template<typename T>
inline Vec4<T> Vec4<T>::operator*(T rhs) const noexcept
{
	return Vec4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
}

template<typename T>
inline Vec4<T> Vec4<T>::operator/(T rhs) const
{
	// assert(rhs != 0);
	return Vec4<T>(x / rhs, y / rhs, z / rhs, w / rhs);
}


template<typename T>
inline Vec4<T> operator+(T lhs, const Vec4<T>& rhs) noexcept
{
	return rhs + lhs;
}

template<typename T>
inline Vec4<T> operator*(T lhs, const Vec4<T>& rhs) noexcept
{
	return rhs * lhs;
}

template<typename T>
inline Vec2<T> Vec4<T>::to_vec2(void) const
{
	return Vec2<T>(x, y);
}

template<typename T>
inline Vec3<T> Vec4<T>::to_vec3(void) const
{
	return Vec3<T>(x, y, z);
}

template<typename T>
inline std::string Vec4<T>::to_str() const
{
	return "["
		+ std::to_string(x) + ", "
		+ std::to_string(y) + ", "
		+ std::to_string(z) + ", "
		+ std::to_string(w)
		+ "]";
}

template<typename T>
inline float Vec4<T>::length() const
{
	return std::sqrt(length_squared());
}

template<typename T>
inline float Vec4<T>::length_squared() const
{
	return x * x + y * y + z * z + w * w;
}

template<typename T>
inline T Vec4<T>::max() const
{
	return std::max({ x, y, z, w });
}

template<typename T>
inline Vec4<T> Vec4<T>::abs() const
{
	return Vec4<T>(
		x > 0 ? x : -x,
		y > 0 ? y : -y,
		z > 0 ? z : -z,
		w > 0 ? w : -w);
}

template<typename T>
inline T Vec4<T>::abs_max() const
{
	return abs().max();
}

template<typename T>
inline Vec4<T> Vec4<T>::normalize() const
{
	if (is_zero() == true)
		return Vec4<T>(x, y, z, w);

	return *this / length();
}

template<typename T>
inline T Vec4<T>::dot(const Vec4<T>& rhs) const
{
	return (*this * rhs).sum();
}

template<typename T>
inline T Vec4<T>::sum() const
{
	return x + y + z + w;
}

template<typename T>
inline T Vec4<T>::abs_sum() const
{
	return abs().sum();
}

