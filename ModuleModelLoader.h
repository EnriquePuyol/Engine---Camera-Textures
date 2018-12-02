#ifndef __ModuleModelLoader_h__
#define __ModuleModelLoader_h__

#include "Module.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include "MathGeoLib.h"

#include <GL/glew.h>

class ModuleModelLoader: public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init() override;
	update_status Update() override;

	bool Load(char* path);

	void GenerateMeshData(const aiScene* myScene);
	void GenerateMaterialData(const aiScene* myScene);

	void DeleteModel();

public:
	unsigned* vbo = nullptr;
	unsigned* ibo = nullptr;
	unsigned* textures = nullptr;
	unsigned* materials = nullptr;
	unsigned* numVerticesMesh = nullptr;
	unsigned* numIndexesMesh = nullptr;

	unsigned numMeshes;
	//unsigned numMaterials;

	const aiScene* scene = NULL;

	bool model;

};

#endif