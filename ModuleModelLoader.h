#ifndef __ModuleModelLoader_h__
#define __ModuleModelLoader_h__

#include "Module.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>

#include <GL/glew.h>

class ModuleModelLoader: public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init();
	update_status Update();

	void GenerateMeshData(aiMesh* mesh);
	void GenerateMaterialData(aiMaterial* material);

};

#endif