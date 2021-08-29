#include "Object.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cassert>

Object::Object(const std::string& filepath)
{
	Assimp::Importer imp;
	auto model = imp.ReadFile(filepath,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);

	const auto pMesh = model->mMeshes[0];
	vertices.reserve(pMesh->mNumVertices);
	normals.reserve(pMesh->mNumVertices);
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

	indices.reserve(pMesh->mNumFaces * 3);
	for (unsigned i = 0; i < pMesh->mNumFaces; ++i) {
		const auto& face = pMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.emplace_back(face.mIndices[0]);
		indices.emplace_back(face.mIndices[1]);
		indices.emplace_back(face.mIndices[2]);
	}
}
