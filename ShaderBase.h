#pragma once
#ifndef SHADER_BASE_H
#define SHADER_BASE_H

#include <functional>
#include "Math/Math.h"

// vertex to fragment
struct V2F {
	Vec3f vtx_view;
	Vec4f vtx_mvp;  // 还需要视口变换
	Vec3f normal;
	Vec2f texcoord;
	float rhw;		// vtx_mvp中w的倒数
	Vec3f color; // 纹理颜色
};

// vertex shader 输入参数
struct Vertex {
	Vec3f vtx;
	Vec3f normal;
	Vec2f texcoord;
};

// 存变换矩阵，给vertexshader用
struct ShadingContext {
	Mat4f model;
	Mat4f view;
	Mat4f projection;
	// 视口变换矩阵在graphics
	//Mat4f it_model;	// == model.inverse().transpose() // 为了算摄像机空间法线准备
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
	void gen_mvp(void); // 预计算，设置完上面三个后别忘了！
};


#endif // !SHADER_BASE_H




