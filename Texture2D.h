#pragma once
#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H


#include <memory>
#include "Math/Math.h"
#include "Texture2DDescriptor.h"
#include "Image.h"


class Texture2D
{

	

public:
	Texture2D();
	Texture2D(const std::string& filename, Texture2DDescriptor rdesc = {});
	Texture2D(const Texture2D& tex2d);
	Texture2D& operator=(const Texture2D& tex2d);
	~Texture2D() = default;
	
	void set_descriptor(Texture2DDescriptor rhs);
	Vec3f get_tex(float u, float v);


	std::shared_ptr<Image> data;
	Texture2DDescriptor desc;
};


#endif
