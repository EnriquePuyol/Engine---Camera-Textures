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

#include "ComponentMesh.h"

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
	void GenerateMeshDataNEW(int id, const aiScene* myScene, MeshData* meshData, const char* path, GameObject* go = NULL);
	unsigned GenerateMaterialDataNEW(int idMaterial, const char* path);

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