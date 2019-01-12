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
}

ComponentMesh::ComponentMesh(ComponentMesh * component)
{
	type = Mesh;
	active = component->active;
	owner = component->owner;
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
	ImGui::PopID();
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

		App->scene->selectedGO->material->material = App->modelLoader->GenerateMaterialDataNEW(i, path);

		if (i == 0)
		{
			id = i;
			sprintf_s(this->path, path);

			App->modelLoader->GenerateMeshDataNEW(i, scene, &meshData, path);
		}
		else
		{
			ComponentMesh* compMesh = new ComponentMesh(App->scene->selectedGO);
			compMesh->id = i;
			sprintf_s(compMesh->path, path);

			App->modelLoader->GenerateMeshDataNEW(i, scene, &compMesh->meshData, path);

			App->scene->selectedGO->components.push_back(compMesh);

		}
	}
}

void ComponentMesh::CheckOtherMeshes(char* path)
{
	// Checks if this model is already loaded in this gameobject
	for (list<Component*>::iterator it = App->scene->selectedGO->components.begin(); it != App->scene->selectedGO->components.end(); ++it)
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
