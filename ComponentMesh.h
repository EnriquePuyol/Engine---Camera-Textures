#ifndef __ComponentMesh_h__
#define __ComponentMesh_h__

#include "Component.h"
#include "MathGeoLib.h"

enum meshDataType {
	Vbo,
	Vao
};

struct MeshData 
{
	meshDataType type;
	unsigned vbo;
	unsigned ibo;
	unsigned numIndexesMesh;
	unsigned materialIndex;
	unsigned numVertices;
	unsigned numFaces;
	unsigned vao;
	unsigned numTexCoords;
	unsigned* buffer = new unsigned[10];
	bool hasNormals = false;
	float3 * vertices;
	float3 * indices;
	float3 * normals;
};

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* parent);
	ComponentMesh(ComponentMesh* component);
	~ComponentMesh();

	PreComponentReturn PreUpdate() override;
	void Update() override;
	void CleanUp() override;
	void Draw(int id) override;
	void LoadMesh(char* path);
	void CheckOtherMeshes(char* path);

public:
	int id;

	MeshData meshData;
	char path[40] = "";
};

#endif // !__ComponentMesh_h__