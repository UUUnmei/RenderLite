#pragma once
#ifndef GRAPHICS_DISCRIPTOR_H
#define GRAPHICS_DISCRIPTOR_H

//��ʾ��ʽ
enum class RenderMode {
	WireFrame,	// �߿�ģʽ
	FilledTriangle,		//���������
};

// ���������ʽ
enum class SampleMode {
	Single,
	Bilinear,  // ˫���Բ�ֵ
};

// ���� �������� ����0-1��Χʱ����Ϊ
enum class TextureWrapMode {
	Repeat,		//ֻҪС������
	Clamp,		// �ضϵ�0-1��Χ
};

struct GraphicsDiscriptor {
	RenderMode display = RenderMode::FilledTriangle;
	bool MSAA4x = false;
	SampleMode sample = SampleMode::Single;
	TextureWrapMode texwrap = TextureWrapMode::Clamp;
};


#endif // !GRAPHICS_DISCRIPTOR_H
