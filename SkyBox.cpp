#include "SkyBox.h"
#include "ChiliWin.h"

#include <filesystem>



SkyBox::SkyBox() : Object("obj/cube.obj")
{
	type = 1;
}

// 传天空贴图所在路径就行
// 自动找特定名字的图
// TOP 0
// BOTTOM 1
// LEFT 2
// RIGHT 3
// FRONT 4
// BACK 5
void SkyBox::setup(const std::string& dir)
{
	using namespace std::filesystem;
	faces.resize(6);

	path str(dir);
	if (!exists(str))
		throw std::runtime_error("天空盒路径错误");

	static const char* face_name[] = { "top.jpg", "bottom.jpg", "left.jpg", "right.jpg", "front.jpg", "back.jpg" };

	for (int i = 0; i < 6; ++i) {
		path t = str.append(face_name[i]);
		std::string s = t.string();
		faces[i] = Texture2D(s);
		str.remove_filename();
	}
	

	OutputDebugString("成功载入天空盒");
}

Vec3f SkyBox::get_tex(float x, float y, float z)
{

	float absX = fabs(x);
	float absY = fabs(y);
	float absZ = fabs(z);
	float ma, sc, tc;
	int faceIndex;
	ma = std::max(std::max(absX, absY), absZ);
	if (ma == absX) {
		if (x > 0) {
			faceIndex = 3;   // right
			sc = z;
			tc = y;
		}
		else {
			faceIndex = 2;   // left
			sc = -z;
			tc = y;
		}
	}
	else if (ma == absY) {
		if (y > 0) {
			faceIndex = 0;   // top
			sc = x;
			tc = z;
		}
		else {
			faceIndex = 1;   // bottom
			sc = x;
			tc = -z;
		}
	}
	else {
		if (z > 0) {
			faceIndex = 5;  // back
			sc = -x;
			tc = y;
		}
		else {
			faceIndex = 4;    // front
			sc = x;
			tc = y;
		}
	}
	Vec2f uv((sc / ma + 1) / 2, (tc / ma + 1) / 2);
	return faces[faceIndex].get_tex(uv.u, uv.v);
}

