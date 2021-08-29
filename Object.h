#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "Math/Math.h"
#include <vector>



class Object
{
public:
	std::vector<Vec3f> vertices; // 顶点
	std::vector<Vec3f> normals;  // 发现 
	std::vector<int> indices;   //顶点索引

public:
	Object(const std::string& filepath);
	~Object() = default;

	Object(const Object&) = delete;  // 不可复制
	Object& operator=(const Object&) = delete;

};



#endif // !OBJECT_H

