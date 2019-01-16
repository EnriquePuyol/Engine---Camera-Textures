#ifndef __ComponentMesh_h__
#define __ComponentMesh_h__

#include "Component.h"
#include "MathGeoLib.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>

struct MeshData 
{
	unsigned vbo;
	unsigned ibo;
	unsigned numIndexesMesh;
	unsigned materialIndex;
	unsigned numVertices;
	unsigned numFaces;
	unsigned vao;
	unsigned numTexCoords;
	unsigned* buffer  = new unsigned[10];
	bool hasNormals   = false;
	float* colours	  = nullptr;
	float3* vertices	  = nullptr;
	unsigned* indices = nullptr;
	float* normals    = nullptr;
	float* uvs		  = nullptr;

	unsigned normalsOffset = 0u;
	unsigned texturesOffset = 0u;
	unsigned vertexSize = 0u;
};

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* parent);
	ComponentMesh(GameObject* parent, char* path);
	ComponentMesh(ComponentMesh* component);
	~ComponentMesh();

	PreComponentReturn PreUpdate() override;
	void Update() override;
	void CleanUp() override;
	void Draw(int id) override;
	void ShowMetadata() override;

	void LoadMesh(char* path);
	void CheckOtherMeshes(char* path);
	void GenerateMeshData(const aiScene* myScene);

	void Save(System* system) override;
	void Load(System* system, rapidjson::Value& value) override;

public:
	int id;
	bool hasTexture = true;

	int	numIndices = 0;
	int	materialIndex = 0;
	int numVert = 0;

	MeshData meshData;
	char path[100] = "";
};

#endif // !__ComponentMesh_h__