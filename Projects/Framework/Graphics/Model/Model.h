#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h"

class Mesh;
class Model : public BaseObject
{
public:
	Model(std::string modelPath, unsigned int loadFlags = aiProcess_Triangulate | aiProcess_FlipUVs);
	virtual ~Model();

	std::vector<Mesh>& GetMeshes();
private:

	void ProcessNode(aiNode* pNode);
	Mesh ProcessMesh(aiMesh* pMesh);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* pMat, aiTextureType type, TextureType typeID);

	std::vector<Texture> m_TexturesLoaded;
	std::vector<Mesh> m_Meshes;
	std::string m_LoadDirectory;
	const aiScene* m_ModelScene;
};