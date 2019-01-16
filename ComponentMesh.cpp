#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "ModuleModelLoader.h"
#include "ModuleUI.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	type = Mesh;
	App->renderer->meshes.push_back(this);

	uID = App->GenerateUUID();
}

ComponentMesh::ComponentMesh(GameObject * parent, char * path) : Component(parent)
{
	type = Mesh;
	sprintf(this->path, path);
	App->renderer->meshes.push_back(this);

	uID = App->GenerateUUID();
}

ComponentMesh::ComponentMesh(ComponentMesh * component)
{
	type = Mesh;
	active = component->active;
	owner = component->owner;
	App->renderer->meshes.push_back(this);

	uID = component->uID;
}

ComponentMesh::~ComponentMesh()
{
}

PreComponentReturn ComponentMesh::PreUpdate()
{
	// Si borramos el componente quitamos la mesh del Render
	if (nextPreReturn == COMP_DELETED)
	{
		for (list<ComponentMesh*>::iterator it = App->renderer->meshes.begin(); it != App->renderer->meshes.end(); )
		{
			if ((*it) != this)
				++it;
			else
				App->renderer->meshes.erase(it++);
		}
	}

	return nextPreReturn;
}

void ComponentMesh::Update()
{

}

void ComponentMesh::CleanUp()
{

}

void ComponentMesh::Draw(int id)
{
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("MESH").x / 2);
	ImGui::Text("MESH");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("     X     ").x / 2);
	ImGui::PushID(id);
	if (ImGui::Button("X"))
	{
		Delete();
	}
	ImGui::PopID();

	ImGui::Spacing();

	ImGui::PushID(id);
	ImGui::Text("Path:");
	ImGui::SameLine();
	ImGui::InputText("", path, ARRAYSIZE(path));
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		LoadMesh(path);
	}

	if (owner->showMetadata)
		ShowMetadata();

	ImGui::PopID();
}

void ComponentMesh::ShowMetadata()
{
	ImGui::SeparatorCustom(50, ImGui::GetWindowWidth() - 100);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4, 0.4, 0.4, 1));
	ImGui::Text("uID:");
	ImGui::Text(uID);
	ImGui::PopStyleColor();
}

void ComponentMesh::LoadMesh(char* path)
{
	CheckOtherMeshes(path);

	const aiScene* scene = aiImportFile(path, aiProcess_Triangulate);
	
	if (scene == NULL)
	{
		char* error = new char[50];
		sprintf(error, "Model not found: ");
		strcat(error, path);
		App->ui->uiConsole->console.AddLog(error);
		return;
	}

	for (int i = 0; i < scene->mNumMeshes; ++i) {

		//App->modelLoader->GenerateMaterialDataNEW(i, path);
		owner->material->matIndex = App->modelLoader->GenerateMaterialDataNEW(i, path);

		if (i == 0)
		{
			id = i;
			sprintf_s(this->path, path);

			//App->modelLoader->GenerateMeshDataNEW(i, scene, &meshData, path, owner);
			GenerateMeshData(scene);
		}
		else
		{
			ComponentMesh* compMesh = new ComponentMesh(owner);
			compMesh->id = i;
			sprintf_s(compMesh->path, path);

			//App->modelLoader->GenerateMeshDataNEW(i, scene, &compMesh->meshData, path, owner);
			compMesh->GenerateMeshData(scene);
			owner->components.push_back(compMesh);

		}
	}
	owner->boundingBox->SetAABB(owner->GetAllComponentsOfType(Mesh));
}

void ComponentMesh::CheckOtherMeshes(char* path)
{
	// Checks if this model is already loaded in this gameobject
	for (list<Component*>::iterator it = owner->components.begin(); it != owner->components.end(); ++it)
	{
		if ((*it)->type == Mesh)
		{
			if (strcmp(dynamic_cast<ComponentMesh*>(*it)->path, path) == 0 && (*it) != this)
			{
				(*it)->nextPreReturn = COMP_DELETED;
			}
		}
	}
}

void ComponentMesh::GenerateMeshData(const aiScene* myScene)
{
	const aiMesh* src_mesh = myScene->mMeshes[id];

	meshData.numVertices = src_mesh->mNumVertices;
	meshData.vertices = new float3[meshData.numVertices];
	memcpy(meshData.vertices, src_mesh->mVertices, sizeof(float) * meshData.numVertices * 3);

	if (src_mesh->HasFaces())
	{
		meshData.numIndexesMesh = src_mesh->mNumFaces * 3;
		meshData.indices = new unsigned[meshData.numIndexesMesh];
		for (unsigned i = 0u; i < src_mesh->mNumFaces; ++i)
		{
			memcpy(&meshData.indices[i * 3], src_mesh->mFaces[i].mIndices, 3 * sizeof(unsigned));
		}
	}

	if (src_mesh->HasTextureCoords(0))
	{
		meshData.uvs = new float[meshData.numVertices * 2];
		int uvsCounter = 0;
		for (unsigned i = 0u; i < meshData.numVertices; ++i)
		{
			meshData.uvs[uvsCounter] = src_mesh->mTextureCoords[0][i].x;
			++uvsCounter;
			meshData.uvs[uvsCounter] = src_mesh->mTextureCoords[0][i].y;
			++uvsCounter;
		}
	}

	if (src_mesh->HasNormals())
	{
		meshData.normals = new float[meshData.numVertices * 3];
		memcpy(meshData.normals, src_mesh->mNormals, sizeof(float)*meshData.numVertices * 3);
	}

	if (src_mesh->HasVertexColors(0))
	{
		meshData.colours = new float[meshData.numVertices * 3];
		memcpy(meshData.colours, src_mesh->mColors, sizeof(float)*meshData.numVertices * 3);
	}

	glGenVertexArrays(1, &meshData.vao);
	glBindVertexArray(meshData.vao);

	glGenBuffers(1, &meshData.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, meshData.vbo);


	unsigned offset = sizeof(math::float3);

	if (meshData.normals != nullptr) {
		meshData.normalsOffset = offset;
		offset += sizeof(math::float3);
	}

	if (meshData.uvs != nullptr) {
		meshData.texturesOffset = offset;
		offset += sizeof(math::float2);
	}

	meshData.vertexSize = offset;

	glBufferData(GL_ARRAY_BUFFER, meshData.vertexSize * meshData.numVertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * meshData.numVertices, meshData.vertices);

	if (meshData.normals != nullptr)
	{
		glBufferSubData(GL_ARRAY_BUFFER, meshData.normalsOffset * meshData.numVertices, sizeof(float) * 3 * meshData.numVertices, meshData.normals);
	}

	if (meshData.uvs != nullptr)
	{
		glBufferSubData(GL_ARRAY_BUFFER, meshData.texturesOffset * meshData.numVertices, sizeof(float2) * meshData.numVertices, meshData.uvs);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glGenBuffers(1, &meshData.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * meshData.numIndexesMesh, meshData.indices, GL_STATIC_DRAW);

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	if (meshData.texturesOffset != 0)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(meshData.texturesOffset * meshData.numVertices));
	}

	if (meshData.normalsOffset != 0)
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(meshData.normalsOffset * meshData.numVertices));
	}

	// vao off
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// vbo off
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->numIndices = src_mesh->mNumFaces * 3;
	this->materialIndex = src_mesh->mMaterialIndex;
	this->numVert = src_mesh->mNumVertices;
}

void ComponentMesh::Save(System * system)
{
	system->StartObject();

	system->AddComponentType("compType", type);
	system->AddString("ownerUID", owner->uID);
	system->AddString("uID", uID);

	system->AddInt("id", id);
	system->AddInt("numIndices", numIndices);
	system->AddInt("materialIndex", materialIndex);
	system->AddInt("numVert", numVert);
	system->AddString("path", path);

	system->EndObject();
}
