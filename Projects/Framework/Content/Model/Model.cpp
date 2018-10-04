#include "FrameworkPCH.h"
#include "Model.h"

#include "Content/Model/Mesh.h"
#include "PropertyManager.h"

Model::Model(std::string modelPath, unsigned int loadFlags)
	:m_Meshes()
{
	std::string assetPath{};
	PropertyManager::GetInstance().GetString("assetpath", assetPath);

	std::string modelPathStr = assetPath + "/" + modelPath;

	Assimp::Importer importer;
	m_ModelScene = importer.ReadFile(modelPathStr, loadFlags);
	if (!m_ModelScene || m_ModelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_ModelScene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
		return;
	}

	ProcessNode(m_ModelScene->mRootNode);
}

Model::~Model()
{
}

std::vector<Mesh>& Model::GetMeshes()
{
	return m_Meshes;
}

void Model::ProcessNode(aiNode* pNode)
{
	for (unsigned int i=0; i < pNode->mNumMeshes; i++)
	{
		auto pMesh = m_ModelScene->mMeshes[pNode->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(pMesh));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		ProcessNode(pNode->mChildren[i]);
	}
}

Mesh Model::ProcessMesh(aiMesh* pMesh)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		Vertex vertex{};
		glm::vec3 bufferVec{};

		//Load positions
		bufferVec.x = pMesh->mVertices[i].x;
		bufferVec.y = pMesh->mVertices[i].y;
		bufferVec.z = pMesh->mVertices[i].z;
		vertex.Position = bufferVec;

		//Load normals
		bufferVec.x = pMesh->mNormals[i].x;
		bufferVec.y = pMesh->mNormals[i].y;
		bufferVec.z = pMesh->mNormals[i].z;
		vertex.Normal = bufferVec;

		//TexCoords
		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		if (pMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			bufferVec.x = pMesh->mTextureCoords[0][i].x;
			bufferVec.y = pMesh->mTextureCoords[0][i].y;
			vertex.TexCoords = bufferVec;
		}

		//Tangent
		//BiTangent
		//...

		vertices.push_back(vertex);
	}

	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	{
		aiFace face = pMesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// process materials
	aiMaterial* material = m_ModelScene->mMaterials[pMesh->mMaterialIndex];

	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* pMat, aiTextureType type, TextureType typeID)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < pMat->GetTextureCount(type); i++)
	{
		aiString str;
		pMat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++)
		{
			if (std::strcmp(m_TexturesLoaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_TexturesLoaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}

			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Texture texture(str.C_Str());
				texture.type = typeID;
				texture.path = str.C_Str();
				textures.push_back(texture);
				m_TexturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
	}
	return textures;
}