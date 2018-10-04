#include "FrameworkPCH.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const char* modelPath)
	:m_ModelPath(modelPath)
{
}

Model::~Model()
{
}

void Model::OnInit()
{

}