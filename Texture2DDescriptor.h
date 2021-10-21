#pragma once
#ifndef TEXTURE_2DDESCRIPTOR_H
#define TEXTURE_2DDESCRIPTOR_H


// ���������ʽ
enum class SampleMode {
	Single,
	Bilinear,  // ˫���Բ�ֵ
};

// ���� �������� ����0-1��Χʱ����Ϊ
enum class WrapMode {
	Repeat,		//ֻҪС������
	Clamp,		// �ضϵ�0-1��Χ
};


struct Texture2DDescriptor {
	SampleMode sample_mode = SampleMode::Single;
	WrapMode wrap_mode = WrapMode::Repeat;
};
#endif // !TEXTURE_2D_H