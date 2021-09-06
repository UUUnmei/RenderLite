#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "ShaderBase.h"
#include "Math/Math.h"
#include <vector>
#include <memory>
#include "bmp.h"

class Object
{
public:
	ShadingContext transform;
	std::function<Vec4f(const Vertex&, const ShadingContext&, V2F&)> vertex_shader; // 返回值是mvp后的顶点坐标（未齐次化）还会修改V2F
	std::function<Vec3f(const V2F&)> pixel_shader;		// 返回值是颜色


	std::unique_ptr<Bitmap> texture;  // 纹理

	std::vector<Vec3f> vertices; // 顶点
	std::vector<Vec3f> normals;  // 发现 
	std::vector<int> indices;   //顶点索引
	std::vector<Vec2f> texcoords; // 纹理坐标

public:
	Object(const std::string& filepath, const std::string& tex_path = {});
	~Object() = default;

	Object(const Object&) = delete;  // 不可复制
	Object& operator=(const Object&) = delete;

	Vec3f get_tex(float u, float v);
};



#endif // !OBJECT_H

