#include "Graphics.h"
#include "Exception.h"


#include <iostream>



Graphics::Graphics(HWND hWnd, int width, int height)
	:width(width), height(height), screen_handle(hWnd)
{
	HDC hDC = GetDC(hWnd);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(hWnd, hDC);

	BITMAPINFO bmpinfo;
	memset(&bmpinfo, 0, sizeof(BITMAPINFO));
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = width;
	bmpinfo.bmiHeader.biHeight = -height;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = height * width * 4;
	//bmpinfo.bmiColors ����̫�������

	screen_hb = CreateDIBSection(screen_dc, &bmpinfo, DIB_RGB_COLORS, (void**)&framebuffer, 0, 0);
	if (screen_hb == NULL) {
		THROW_ERROR("create dib section failed")
	}

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);

	memset(framebuffer, 0, width * height * sizeof(unsigned int));
}


Graphics::~Graphics()
{	
	SelectObject(screen_dc, screen_ob);
	DeleteDC(screen_dc);
	DeleteObject(screen_ob);
	DeleteObject(screen_hb);
}

void Graphics::set_pixel(int x, int y, uint32_t color)
{
	//std::cout << x << ' ' << y << ' ' << color << '\n';
	framebuffer[y * width + x] = color;
}

void Graphics::set_pixel(int idx, uint32_t color)
{
	framebuffer[idx] = color;
}

void Graphics::clear_buffer(uint32_t color)
{
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			framebuffer[i * width + j] = color;
}

void Graphics::draw(void)
{
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
}

// bresenham
// ѧϰ�㷨http://www.idivecat.com/archives/652 
// ʵ�ֲο�������https://stackoverflow.com/questions/10060046/drawing-lines-with-bresenhams-line-algorithm/16405254#16405254
// ��������������˵�Ĳ�һ����ʵ��Ч���Ļ����ζԽ����ϸ��ǲ�����ȷʵ�����⡣�������������
void Graphics::draw_line(const Vec2i& start, const Vec2i& end, uint32_t color)
{
	int dx = end.x - start.x;
	int dy = end.y - start.y;

	int dLong = abs(dx);
	int dShort = abs(dy);

	int offsetLong = dx > 0 ? 1 : -1;
	int offsetShort = dy > 0 ? width : -width;

	if (dLong < dShort)
	{
		std::swap(dShort, dLong);
		std::swap(offsetShort, offsetLong);
	}
	int error = dShort - dLong / 2;
	int index = start.y * width + start.x;
	const int offset[] = { offsetLong, offsetLong + offsetShort };
	const int abs_d[] = { dShort, dShort - dLong };
	for (int i = 0; i <= dLong; ++i)
	{
		set_pixel(index, color);
		const int errorIsTooBig = error >= 0;
		index += offset[errorIsTooBig];
		error += abs_d[errorIsTooBig];
	}

}
