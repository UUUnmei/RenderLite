#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "Texture2D.h"
#include "Object.h"

#include <vector>
#include <memory>

class SkyBox : public Object
{
	
public:
	SkyBox();
	~SkyBox() = default;
	std::vector<Texture2D> faces;
	void setup(const std::string& dir);

	virtual Vec3f get_tex(float u, float v, float w) override;
	

};

#endif // !SKYBOX_H
