#include "Texture2D.h"

Texture2D::Texture2D() : data(nullptr), desc({})
{
}

Texture2D::Texture2D(const std::string& filename, Texture2DDescriptor rdesc)
	: data(std::make_shared<Image>(filename)), desc(rdesc)
{
}

// 只进行浅拷贝，多个纹理可以共用同一个图片
Texture2D::Texture2D(const Texture2D& tex2d) : data(tex2d.data), desc(tex2d.desc)
{ }

Texture2D& Texture2D::operator=(const Texture2D& tex2d)
{
	data = tex2d.data;
	desc = tex2d.desc;
	return *this;
}



inline static Vec3f color_from_uint(uint32_t color) {
	int mask = 0xFF;
	float b = (color & mask) / 255.0f;
	color >>= 8;
	float g = (color & mask) / 255.0f;
	color >>= 8;
	float r = (color & mask) / 255.0f;
	return Vec3f(r, g, b);
}

void Texture2D::set_descriptor(Texture2DDescriptor rhs)
{
	desc = rhs;
}

Vec3f Texture2D::get_tex(float u, float v)
{
	static const Vec3f DEFAULT_COLOR(148.0 / 255, 121.0 / 255, 92.0 / 255); // 没纹理时的默认颜色
	if (!data) return DEFAULT_COLOR;

	if (desc.wrap_mode == WrapMode::Clamp) {
		u = Math::clamp(u, 0.0f, 1.0f);
		v = Math::clamp(v, 0.0f, 1.0f);
	}
	else {
		// Repeat
		u = u - std::floor(u);
		v = v - std::floor(v);
	}

	float x = u * data->width() + 0.5f;
	float y = (1 - v) * data->height() + 0.5f;

	uint32_t ret;
	if (desc.sample_mode == SampleMode::Single) {
		ret = data->get_pixel(x, y);
	}
	else {
		// Bilinear 我的朴素实现。。
		//int x1 = std::floor(x);
		//int x2 = std::min(texture->GetW() - 1, (int)std::ceil(x));
		//int y1 = std::floor(y);
		//int y2 = std::min(texture->GetH() - 1, (int)std::ceil(y));

		//Vec3f c11 = color_from_uint(texture->GetPixel(x1, y1));
		//Vec3f c12 = color_from_uint(texture->GetPixel(x1, y2));
		//Vec3f c21 = color_from_uint(texture->GetPixel(x2, y1));
		//Vec3f c22 = color_from_uint(texture->GetPixel(x2, y2));
		//float dx = (float)(x - x1) / (x2 - x1);
		//float dy = (float)(y - y1) / (y2 - y1);
		//Vec3f a = c11 + (c21 - c11) * dx;
		//Vec3f b = c12 + (c22 - c12) * dx;
		//Vec3f c = a + (b - a) * dy;
		//ret = Math::vec_to_color(c);

		// 抄来的，，比上面的快很多
		// skywind3000 RenderHelp
		// 基本想法是把浮点数乘个倍数转为整数计算
		int32_t fx = (int32_t)(x * 0x10000);
		int32_t fy = (int32_t)(y * 0x10000);
		int32_t x1 = Math::clamp(fx >> 16, 0, data->width() - 1);
		int32_t y1 = Math::clamp(fy >> 16, 0, data->height() - 1);
		int32_t x2 = Math::clamp(x1 + 1, 0, data->width() - 1);
		int32_t y2 = Math::clamp(y1 + 1, 0, data->height() - 1);
		int32_t dx = (fx >> 8) & 0xff;
		int32_t dy = (fy >> 8) & 0xff;
		uint32_t c00 = data->get_pixel(x1, y1);
		uint32_t c01 = data->get_pixel(x2, y1);
		uint32_t c10 = data->get_pixel(x1, y2);
		uint32_t c11 = data->get_pixel(x2, y2);
		ret = Math::BilinearInterp(c00, c01, c10, c11, dx, dy); //也是抄来的，，，移到了math中

	}
	return color_from_uint(ret);
}
