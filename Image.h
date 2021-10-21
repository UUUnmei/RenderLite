#pragma once
#ifndef IMAGE_H
#define IMAGE_H




#include <memory>
#include <string>

class Image
{

	unsigned char* data;
	int _width;
	int _height;
	int _byte_per_pixel;

public:
	Image(const std::string& filename);
	Image(const Image& img) = delete;   //²»ÔÊÐí¿½±´
	Image& operator=(const Image& img) = delete;
	~Image();

	int width();
	int height();

	uint32_t get_pixel(int x, int y);

};


#endif // !IMAGE_H
