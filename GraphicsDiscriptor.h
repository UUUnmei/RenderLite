#pragma once
#ifndef GRAPHICS_DISCRIPTOR_H
#define GRAPHICS_DISCRIPTOR_H

//显示方式
enum class RenderMode {
	WireFrame,	// 线框模式
	FilledTriangle,		//填充三角形
};

// 纹理采样方式
enum class SampleMode {
	Single,
	Bilinear,  // 双线性插值
};

// 决定 纹理坐标 超出0-1范围时的行为
enum class TextureWrapMode {
	Repeat,		//只要小数部分
	Clamp,		// 截断到0-1范围
};

struct GraphicsDiscriptor {
	RenderMode display = RenderMode::FilledTriangle;
	bool MSAA4x = false;
	SampleMode sample = SampleMode::Single;
	TextureWrapMode texwrap = TextureWrapMode::Clamp;
};


#endif // !GRAPHICS_DISCRIPTOR_H
