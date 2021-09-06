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
	std::function<Vec4f(const Vertex&, const ShadingContext&, V2F&)> vertex_shader; // ����ֵ��mvp��Ķ������꣨δ��λ��������޸�V2F
	std::function<Vec3f(const V2F&)> pixel_shader;		// ����ֵ����ɫ


	std::unique_ptr<Bitmap> texture;  // ����

	std::vector<Vec3f> vertices; // ����
	std::vector<Vec3f> normals;  // ���� 
	std::vector<int> indices;   //��������
	std::vector<Vec2f> texcoords; // ��������

public:
	Object(const std::string& filepath, const std::string& tex_path = {});
	~Object() = default;

	Object(const Object&) = delete;  // ���ɸ���
	Object& operator=(const Object&) = delete;

	Vec3f get_tex(float u, float v);
};



#endif // !OBJECT_H

