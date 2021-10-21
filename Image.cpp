#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Image.h"

Image::Image(const std::string& filename)
	: data(stbi_load(filename.c_str(), &_width, &_height, &_byte_per_pixel, 0))
{ }


Image::~Image()
{
	if (data)
		stbi_image_free(data);
}

int Image::width()
{
	return _width;
}

int Image::height()
{
	return _height;
}


uint32_t Image::get_pixel(int x, int y)
{
	uint32_t color = 0;
	if (x >= 0 && x < _width && y >= 0 && y < _height) {
		// memcpy 暂不适用
		// 高位 - 低位
		// a  b  g  r   <- memcpy(&color, *data + (x + y * _width) * _byte_per_pixel, sizeof(uint8_t) * _byte_per_pixel);

		memcpy(&color, data + (x + y * _width) * _byte_per_pixel, sizeof(uint8_t) * _byte_per_pixel);
		// abgr -> argb
		color = (color & 0xFF00FF00) | ((color & 0xFF) << 16) | ((color >> 16) & 0xFF);

		//unsigned char* p = data + (x + y * _width) * _byte_per_pixel;
		//unsigned char r = p[0];
		//unsigned char g = p[1];
		//unsigned char b = p[2];
		//unsigned char a = _byte_per_pixel > 3 ? p[3] : 0xFF;
		//color = (a << 24) | (r << 16) | (g << 8) | b;
	}
	return color;
}
