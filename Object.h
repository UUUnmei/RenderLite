#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "Math/Math.h"
#include <vector>



class Object
{
public:
	std::vector<Vec3f> vertices; // ����
	std::vector<Vec3f> normals;  // ���� 
	std::vector<int> indices;   //��������

public:
	Object(const std::string& filepath);
	~Object() = default;

	Object(const Object&) = delete;  // ���ɸ���
	Object& operator=(const Object&) = delete;

};



#endif // !OBJECT_H

