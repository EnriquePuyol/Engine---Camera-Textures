#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleUI.h"
#include "SDL/include/SDL.h"

ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Init()
{
	Load("BakerHouse.fbx");

	return true;
}

update_status ModuleModelLoader::Update()
{

	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::Load(char * path)
{
	if (model)
	{
		DeleteModel();
	}

	bool ok = true;

	scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FixInfacingNormals);

	if (scene == NULL)
		aiGetErrorString();

	numMeshes = scene->mNumMeshes;

	vbo = new unsigned[numMeshes];
	ibo = new unsigned[numMeshes];
	textures = new unsigned[numMeshes];
	materials = new unsigned[numMeshes];
	numVerticesMesh = new unsigned[numMeshes];
	numIndexesMesh = new unsigned[numMeshes];

	GenerateMeshData(scene);
	GenerateMaterialData(scene);

	model = true;
	App->ui->uiConsole->console.AddLog("Model loaded correcty");
	return ok;
}

void ModuleModelLoader::GenerateMeshData(const aiScene* myScene)
{
	for (unsigned i = 0; i < myScene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[i];

		unsigned* buffer = &vbo[i];

		// vertex positions
		glGenBuffers(1, buffer);
		glBindBuffer(GL_ARRAY_BUFFER, *buffer);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * mesh->mNumVertices, mesh->mVertices);

		// textures positions
		float2* texture_coords = (float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices,
			sizeof(float) * 2 * mesh->mNumVertices, GL_MAP_WRITE_BIT);
		for (unsigned j = 0; j < mesh->mNumVertices; ++j)
		{
			texture_coords[j] = float2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// index positions
		unsigned* index = &ibo[i];

		glGenBuffers(1, index);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);

		unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
			sizeof(unsigned)*mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

		for (unsigned k = 0; k < mesh->mNumFaces; ++k)
		{
			assert(mesh->mFaces[k].mNumIndices == 3);

			*(indices++) = mesh->mFaces[k].mIndices[0];
			*(indices++) = mesh->mFaces[k].mIndices[1];
			*(indices++) = mesh->mFaces[k].mIndices[2];
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		textures[i] = mesh->mMaterialIndex;
		numVerticesMesh[i] = mesh->mNumVertices;
		numIndexesMesh[i] = mesh->mNumFaces * 3;
	}

}

void ModuleModelLoader::GenerateMaterialData(const aiScene* myScene)
{
	for (unsigned i = 0; i < myScene->mNumMaterials; ++i)
	{
		const aiMaterial* material = myScene->mMaterials[i];
		unsigned dstMaterial;

		aiString file;
		aiTextureMapping mapping;
		unsigned uvindex = 0;

		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
		{
			dstMaterial = App->textures->Load(file.data, false);
		}

		materials[i] = dstMaterial;
	}
}

void ModuleModelLoader::DeleteModel()
{
	delete vbo;
	delete ibo;
	delete textures;
	delete materials;
	delete numVerticesMesh;
	delete numIndexesMesh;

	model = false;
}
