#include "Object.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cassert>

Object::Object(const std::string& filepath)
	: vertex_shader(nullptr), pixel_shader(nullptr), texture({})
{
	Assimp::Importer imp;
	auto model = imp.ReadFile(filepath,
		aiProcess_OptimizeMeshes |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals				// ��������ļ���û�з��߾��Զ�����
	);

	assert(model != nullptr);
	int vertices_cnt = 0;
	for (int j = 0; j < model->mNumMeshes; ++j) {
		const auto pMesh = model->mMeshes[j];
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
		}
		if (pMesh->HasTextureCoords(0)) {
			// �������������
			for (unsigned i = 0; i < pMesh->mNumVertices; ++i) {
				texcoords.emplace_back(				
					pMesh->mTextureCoords[0][i].x,
					pMesh->mTextureCoords[0][i].y
				);
			}
		}
		else {
			for (unsigned i = 0; i < pMesh->mNumVertices; ++i) {
				texcoords.emplace_back(				
					Vec2f(0, 0)
				);
			}
		}

		for (unsigned i = 0; i < pMesh->mNumFaces; ++i) {
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);

			indices.emplace_back(face.mIndices[0] + vertices_cnt);
			indices.emplace_back(face.mIndices[1] + vertices_cnt);
			indices.emplace_back(face.mIndices[2] + vertices_cnt);
		}
		 // !��ÿ��mesh�����������0��ʼ�ģ�Ҫ�ֶ����¡�����Ȼ����һ���õ�ʱ��Ͳ�����
		// ���߸��򵥴ֱ������ÿ��mesh��Ӧһ��object�ɡ�����
		vertices_cnt += pMesh->mNumVertices;
	}

}

Vec3f Object::get_tex(float u, float v)
{
	static const Vec3f DEFAULT_COLOR(148.0 / 255, 121.0 / 255, 92.0 / 255); // û����ʱ��Ĭ����ɫ
	if (!texture.data) return DEFAULT_COLOR;

	return texture.get_tex(u, v);
}

Vec3f Object::get_tex(float u, float v, float w)
{
	return Vec3f();
}

Object& Object::withTexture2D(Texture2D& tex)
{
	texture = tex;
	return *this;
}
