#pragma once
#ifndef TEXTURE_2DDESCRIPTOR_H
#define TEXTURE_2DDESCRIPTOR_H


// 纹理采样方式
enum class SampleMode {
	Single,
	Bilinear,  // 双线性插值
};

// 决定 纹理坐标 超出0-1范围时的行为
enum class WrapMode {
	Repeat,		//只要小数部分
	Clamp,		// 截断到0-1范围
};


struct Texture2DDescriptor {
	SampleMode sample_mode = SampleMode::Single;
	WrapMode wrap_mode = WrapMode::Repeat;
};
#endif // !TEXTURE_2D_H