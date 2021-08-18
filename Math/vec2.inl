#include "predecl.h"
#include "vec2.h"


template<typename T>
inline Vec2<T>::Vec2() noexcept
	: x(T()), y(T()) { }

template<typename T>
inline Vec2<T>::Vec2(T x, T y) noexcept
	: x(x), y(y) { }

template<typename T>
inline Vec2<T>::Vec2(const std::pair<T, T>& t) noexcept
	: x(t.first), y(t.second) { }


template<typename T>
inline Vec2<T>::Vec2(const Vec2<T>& t)  noexcept
	: x(t.x), y(t.y) { }


template<typename T>
inline Vec2<T>& Vec2<T>::operator=(const Vec2& t) noexcept
{
	if (this == &t) return *this;
	x = t.x;
	y = t.y;
	return *this;
}

template<typename T>
inline const T& Vec2<T>::operator[](size_t i) const
{
	// assert(i < 3);
	return m[i];
}

template<typename T>
inline T& Vec2<T>::operator[](size_t i)
{
	// assert(i < 3);
	return m[i];
}


template<typename T>
inline std::string Vec2<T>::to_str() const
{
	return "["
		+ std::to_string(x) + ", "
		+ std::to_string(y) + ", "
		+ "]";
}