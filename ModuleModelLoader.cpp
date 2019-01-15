#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
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
	//Load("BakerHouse.fbx");

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

void ModuleModelLoader::GenerateMeshDataNEW(int id, const aiScene* myScene, MeshData* meshData, const char * path, GameObject* go)
{
	const aiMesh* src_mesh = myScene->mMeshes[id];

	// copies to assign to the mesh
	float3 * verticesAux = new float3[src_mesh->mNumVertices];
	float3 * indicesAux = new float3[src_mesh->mNumFaces];
	float3 * normalsAux = new float3[src_mesh->mNumVertices];

	meshData->type = Vbo;
	meshData->numVertices = src_mesh->mNumVertices;
	meshData->numFaces = src_mesh->mNumFaces;

	unsigned* vboActual = &meshData->vbo;

	glGenBuffers(1, vboActual);
	glBindBuffer(GL_ARRAY_BUFFER, *vboActual);

	// Vertices & texture coords & normals
	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2 + sizeof(float) * 3)*src_mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * src_mesh->mNumVertices, src_mesh->mVertices);

	math::float2* texture_coords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * src_mesh->mNumVertices,
		sizeof(float) * 2 * src_mesh->mNumVertices, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i < src_mesh->mNumVertices; ++i)
	{
		texture_coords[i] = math::float2(src_mesh->mTextureCoords[0][i].x, src_mesh->mTextureCoords[0][i].y);
	}

	meshData->hasNormals = src_mesh->HasNormals();

	if (meshData->hasNormals) 
		glBufferSubData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2)*src_mesh->mNumVertices, sizeof(float) * 3 * src_mesh->mNumVertices, src_mesh->mNormals);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	for (int i = 0; i < src_mesh->mNumVertices; ++i) {
		verticesAux[i] = float3(src_mesh->mVertices[i].x, src_mesh->mVertices[i].y, src_mesh->mVertices[i].z);
		if (src_mesh->HasNormals()) normalsAux[i] = float3(src_mesh->mNormals[i].x, src_mesh->mNormals[i].y, src_mesh->mNormals[i].z);
	}

	// indices
	unsigned* iboActual = &meshData->ibo;

	glGenBuffers(1, iboActual);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *iboActual);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*src_mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
		sizeof(unsigned)*src_mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);



	for (unsigned i = 0; i < src_mesh->mNumFaces; ++i)
	{
		assert(src_mesh->mFaces[i].mNumIndices == 3);

		indicesAux[i] = float3(src_mesh->mFaces[i].mIndices[0], src_mesh->mFaces[i].mIndices[1], src_mesh->mFaces[i].mIndices[2]);

		*(indices++) = src_mesh->mFaces[i].mIndices[0];
		*(indices++) = src_mesh->mFaces[i].mIndices[1];
		*(indices++) = src_mesh->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	int numMaterials = myScene->mNumMaterials;
	//int index = meshComp->my_go->components.size()  - idMesh;
	//ComponentMaterial* compMat = (ComponentMaterial*)meshComp->my_go->components[index - numMaterials + src_mesh->mMaterialIndex];
	ComponentMaterial* compMat;
	if (App->scene->selectedGO != NULL)
		compMat = App->scene->selectedGO->material;
	else
		compMat = go->material;

	meshData->numTexCoords = src_mesh->mNumVertices;
	meshData->materialIndex = compMat->matIndex;
	meshData->numVertices = src_mesh->mNumVertices;
	meshData->numFaces = src_mesh->mNumFaces;
	meshData->numIndexesMesh = src_mesh->mNumFaces * 3;

	meshData->vertices = verticesAux;
	meshData->indices = indicesAux;
	meshData->normals = normalsAux;

	char* log = new char[50];

	sprintf(log, "MESH LOADED CORRECTLY:");
	App->ui->uiConsole->console.AddLog(log);
	sprintf(log, "  >Number of vertices: %u", meshData->numVertices);
	App->ui->uiConsole->console.AddLog(log);
	sprintf(log, "  >Number of faces: %u", meshData->numFaces);
	App->ui->uiConsole->console.AddLog(log);
	sprintf(log, "  >Has normals: %i", meshData->hasNormals);
	App->ui->uiConsole->console.AddLog(log);
}

unsigned ModuleModelLoader::GenerateMaterialDataNEW(int idMaterial, const char * path)
{
	const aiScene* sceneAct = aiImportFile(path, aiProcess_Triangulate);

	int indexMaterial = sceneAct->mMeshes[idMaterial]->mMaterialIndex;
	const aiMaterial* src_material = sceneAct->mMaterials[indexMaterial];
	unsigned dst_material = 0;

	aiString file;
	aiTextureMapping mapping;
	unsigned uvindex = 0;

	if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
	{
		dst_material = App->textures->Load(file.C_Str(), false);
	}

	return dst_material;
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
