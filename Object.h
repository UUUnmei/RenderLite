#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "Math/Math.h"
#include <vector>
#include <memory>
#include "bmp.h"

class Object
{

public:
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

