#include "Object.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cassert>

Object::Object(const std::string& filepath, const std::string& tex_path)
	: vertex_shader(nullptr), pixel_shader(nullptr)
{
	Assimp::Importer imp;
	auto model = imp.ReadFile(filepath,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices 
		//aiProcess_GenNormals				// 如果读的文件里没有法线就自动生成
	);

	const auto pMesh = model->mMeshes[0];
	vertices.reserve(pMesh->mNumVertices);
	normals.reserve(pMesh->mNumVertices);
	texcoords.reserve(pMesh->mNumVertices);
	for (unsigned i = 0; i < pMesh->mNumVertices; ++i) {
		vertices.emplace_back(
			pMesh->mVertices[i].x,
			pMesh->mVertices[i].y,
			pMesh->mVertices[i].z
			);
		normals.emplace_back(			
			pMesh->mNormals[i].x,
			pMesh->mNormals[i].y,
			pMesh->mNormals[i].z
			);
		texcoords.emplace_back(				// 有时没有纹理坐标就不要push了，
			pMesh->mTextureCoords[0][i].x,
			pMesh->mTextureCoords[0][i].y
		);
	}
	indices.reserve(pMesh->mNumFaces * 3);
	for (unsigned i = 0; i < pMesh->mNumFaces; ++i) {
		const auto& face = pMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.emplace_back(face.mIndices[0]);
		indices.emplace_back(face.mIndices[1]);
		indices.emplace_back(face.mIndices[2]);
	}

	texture = nullptr;
	if (tex_path.empty() == false) {
		texture = std::make_unique<Bitmap>(tex_path.c_str());
	}
}

inline static Vec3f color_from_uint(uint32_t color) {
	int mask = 0xFF;
	float b = (color & mask) / 255.0f;
	color >>= 8;
	float g = (color & mask) / 255.0f;
	color >>= 8;
	float r = (color & mask) / 255.0f;
	return Vec3f(r, g, b);
}


Vec3f Object::get_tex(float u, float v, SampleMode m, TextureWrapMode w)
{
	static const Vec3f DEFAULT_COLOR(148.0 / 255, 121.0 / 255, 92.0 / 255); // 没纹理时的默认颜色
	if (!texture) return DEFAULT_COLOR;

	if (w == TextureWrapMode::Clamp) {
		u = Math::clamp(u, 0.0f, 1.0f);
		v = Math::clamp(v, 0.0f, 1.0f);
	}
	else {
		// Repeat
		u = u - std::floor(u);
		v = v - std::floor(v);
	}

	float x = u * texture->GetW() + 0.5f;
	float y = (1 - v) * texture->GetH() + 0.5f;

	uint32_t ret;
	if (m == SampleMode::Single) {
		ret = texture->GetPixel(x, y);
	}
	else {
		// Bilinear 我的朴素实现。。
		//int x1 = std::floor(x);
		//int x2 = std::min(texture->GetW() - 1, (int)std::ceil(x));
		//int y1 = std::floor(y);
		//int y2 = std::min(texture->GetH() - 1, (int)std::ceil(y));

		//Vec3f c11 = color_from_uint(texture->GetPixel(x1, y1));
		//Vec3f c12 = color_from_uint(texture->GetPixel(x1, y2));
		//Vec3f c21 = color_from_uint(texture->GetPixel(x2, y1));
		//Vec3f c22 = color_from_uint(texture->GetPixel(x2, y2));
		//float dx = (float)(x - x1) / (x2 - x1);
		//float dy = (float)(y - y1) / (y2 - y1);
		//Vec3f a = c11 + (c21 - c11) * dx;
		//Vec3f b = c12 + (c22 - c12) * dx;
		//Vec3f c = a + (b - a) * dy;
		//ret = Math::vec_to_color(c);

		// 抄来的，，比上面的快很多
		// skywind3000 RenderHelp
		// 基本想法是把浮点数乘个倍数转为整数计算
		int32_t fx = (int32_t)(x * 0x10000);
		int32_t fy = (int32_t)(y * 0x10000);
		int32_t x1 = Math::clamp(fx >> 16, 0, texture->GetW() - 1);
		int32_t y1 = Math::clamp(fy >> 16, 0, texture->GetH() - 1);
		int32_t x2 = Math::clamp(x1 + 1, 0, texture->GetW() - 1);
		int32_t y2 = Math::clamp(y1 + 1, 0, texture->GetH() - 1);
		int32_t dx = (fx >> 8) & 0xff;
		int32_t dy = (fy >> 8) & 0xff;
		uint32_t c00 = texture->GetPixel(x1, y1);
		uint32_t c01 = texture->GetPixel(x2, y1);
		uint32_t c10 = texture->GetPixel(x1, y2);
		uint32_t c11 = texture->GetPixel(x2, y2);
		ret = Math::BilinearInterp(c00, c01, c10, c11, dx, dy); //也是抄来的，，，移到了math中

	}
	return color_from_uint(ret);
	
}
