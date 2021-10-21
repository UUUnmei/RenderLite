#pragma once
#ifndef GRAPHICS_DISCRIPTOR_H
#define GRAPHICS_DISCRIPTOR_H


//显示方式
enum class RenderMode {
	WireFrame,	// 线框模式
	FilledTriangle,		//填充三角形
};

enum class FaceCullMode {
	Front,
	Back	
};

struct GraphicsDiscriptor {
	RenderMode display = RenderMode::FilledTriangle;
	bool MSAA4x = false;
	FaceCullMode fc_mode = FaceCullMode::Back;
};


#endif // !GRAPHICS_DISCRIPTOR_H
