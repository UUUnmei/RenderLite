#pragma once
#ifndef GRAPHICS_DISCRIPTOR_H
#define GRAPHICS_DISCRIPTOR_H


//��ʾ��ʽ
enum class RenderMode {
	WireFrame,	// �߿�ģʽ
	FilledTriangle,		//���������
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
