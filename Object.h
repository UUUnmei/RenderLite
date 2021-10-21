#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "GraphicsDiscriptor.h"
#include "ShaderBase.h"
#include "Math/Math.h"
#include <vector>
#include <memory>
#include "Texture2D.h"

class Object
{
public:
	ShadingContext transform;
	std::function<Vec4f(const Vertex&, const ShadingContext&, V2F&)> vertex_shader; // 返回值是mvp后的顶点坐标（未齐次化）还会修改V2F
	std::function<Vec3f(const V2F&)> pixel_shader;		// 返回值是颜色

	int type = 0;
	
	Texture2D texture; // 纹理

	std::vector<Vec3f> vertices; // 顶点
	std::vector<Vec3f> normals;  // 法线
	std::vector<int> indices;   //顶点索引
	std::vector<Vec2f> texcoords; // 纹理坐标

public:
	Object(const std::string& filepath);
	~Object() = default;

	Object(const Object&) = delete;  // 不可复制
	Object& operator=(const Object&) = delete;

	virtual Vec3f get_tex(float u, float v);
	virtual Vec3f get_tex(float u, float v, float w);

	Object& withTexture2D(Texture2D& tex);
};




#endif // !OBJECT_H

