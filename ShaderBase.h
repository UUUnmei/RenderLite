#pragma once
#ifndef SHADER_BASE_H
#define SHADER_BASE_H

#include <functional>
#include "Math/Math.h"

// vertex to fragment
struct V2F {
	Vec3f vtx_view;
	Vec4f vtx_mvp;  // ����Ҫ�ӿڱ任
	Vec3f normal;
	Vec2f texcoord;
	float rhw;		// vtx_mvp��w�ĵ���
	Vec3f color; // ������ɫ
};

// vertex shader �������
struct Vertex {
	Vec3f vtx;
	Vec3f normal;
	Vec2f texcoord;
};

// ��任���󣬸�vertexshader��
struct ShadingContext {
	Mat4f model;
	Mat4f view;
	Mat4f projection;
	// �ӿڱ任������graphics
	//Mat4f it_model;	// == model.inverse().transpose() // Ϊ����������ռ䷨��׼��
	//Mat4f it_view;
	//mvp
	Mat4f mvp;
	Mat4f mv;
	//mv inverse transpose
	Mat4f it_mv;

	// set
	void set_model(const Mat4f& m);
	void set_view(const Mat4f& m);
	void set_projection(const Mat4f& m);
	void gen_mvp(void); // Ԥ���㣬��������������������ˣ�
};


#endif // !SHADER_BASE_H




