#include "ModuleModelLoader.h"

ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Init()
{
	unsigned int flags;
	flags |= aiProcess_Triangulate;
	flags |= aiProcess_FixInfacingNormals;
	const aiScene* scene = aiImportFile("BakerHouse.fbx", flags);
	
	if (scene == NULL)
		aiGetErrorString();

	for(unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		GenerateMeshData(scene->mMeshes[i]);
	}

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		GenerateMaterialData(scene->mMaterials[i]);
	}

	return true;
}

update_status ModuleModelLoader::Update()
{

	return UPDATE_CONTINUE;
}

void ModuleModelLoader::GenerateMeshData(aiMesh* mesh)
{
	GLuint buffer;

	// vertex positions
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (mesh->mNumVertices * 5), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (mesh->mNumVertices * 3), mesh->mVertices);
	// uvs
	//glBufferSubData(GL_ARRAY_BUFFER, (mesh->mNumVertices * 3), mesh->mNumVertices * sizeof(float) * 2, mesh->mVertices);

	// texture coords
	//glMapBufferRange(GL_ARRAY_BUFFER, 0, 2, GL_MAP_READ_BIT);
}

void ModuleModelLoader::GenerateMaterialData(aiMaterial * material)
{

}
