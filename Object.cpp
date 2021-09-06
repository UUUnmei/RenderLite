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

static Vec3f color_from_uint(uint32_t color) {
	int mask = 0xFF;
	float b = (color & mask) / 255.0f;
	color >>= 8;
	float g = (color & mask) / 255.0f;
	color >>= 8;
	float r = (color & mask) / 255.0f;
	return Vec3f(r, g, b);
}

Vec3f Object::get_tex(float u, float v)
{
	if (!texture) return { };

	int x = u * texture->GetW();
	int y = (1 - v) * texture->GetH();
	uint32_t c = texture->GetPixel(x, y);
	return color_from_uint(c);
	
}
