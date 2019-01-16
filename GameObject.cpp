#include "GameObject.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleScene.h"
#include "UI_Hierarchy.h"

#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentLight.h"
#include "ComponentMesh.h"
#include "ModuleFileSystem.h"

GameObject::GameObject(const char name[40])
{
	sprintf_s(this->name, name);
	transform = new ComponentTransform(this);
	material = new ComponentMaterial(this);
	material->defaultMat = true;
	boundingBox = new ComponentBBox(this);

	uID = App->GenerateUUID();
}

GameObject::GameObject(GameObject * goParent)
{
	sprintf_s(this->name, "Empty");
	parent = goParent;

	transform = new ComponentTransform(this);
	material = new ComponentMaterial(this);
	material->defaultMat = true;
	boundingBox = new ComponentBBox(this);

	uID = App->GenerateUUID();
}

GameObject::GameObject(GameObject * gameobject, GameObject * parent)
{
	sprintf_s(name, gameobject->name);
	selected = gameobject->selected;
	this->parent = parent;
	this->transform = new ComponentTransform(gameobject->transform);
	
	for (list<Component*>::iterator it = gameobject->components.begin(); it != gameobject->components.end(); ++it)
	{
		if ((*it)->type == Mesh)
		{
			components.push_back(new ComponentMesh(dynamic_cast<ComponentMesh*>(*it)));
		}
		else if ((*it)->type == Material)
		{
			components.push_back(new ComponentMaterial(dynamic_cast<ComponentMaterial*>(*it)));
		}
		else if ((*it)->type == Camera)
		{
			components.push_back(new ComponentCamera(dynamic_cast<ComponentCamera*>(*it)));
		}
		else if ((*it)->type == Light)
		{
			components.push_back(new ComponentLight(dynamic_cast<ComponentLight*>(*it)));
		}
	}

	for (list<GameObject*>::iterator it = gameobject->childs.begin(); it != gameobject->childs.end(); ++it)
	{
		childs.push_back(new GameObject((*it), this));
	}

	uID = gameobject->uID;
}


GameObject::~GameObject()
{

}

NextPreReturn GameObject::PreUpdate()
{

	for (list<GameObject*>::iterator it = childs.begin(); it != childs.end();)
	{

		if ((*it)->PreUpdate() == GO_DELETED || (*it)->PreUpdate() == GO_CUT)
			childs.erase(it++);
		else
			++it;

	}

	for (list<Component*>::iterator it = components.begin(); it != components.end();)
	{
		if ((*it)->PreUpdate() == COMP_DELETED)
			components.erase(it++);
		else
			++it;
	}

	if (nextPreReturn == GO_PASTE)
	{
		if (nullptr != App->scene->selectedGO)
		{
			childs.push_back(new GameObject(App->scene->cutcopyGO, App->scene->selectedGO));
			App->scene->selectedGO->openNode = true;
		}
		else
			childs.push_back(new GameObject(App->scene->cutcopyGO, App->scene->root));

		nextPreReturn = GO_NONE;
	}

	if (nextPreReturn == GO_DELETED)
	{
		Delete();
		return GO_DELETED;
	}
	else if (nextPreReturn == GO_CUT)
	{
		App->scene->cutcopyGO = new GameObject(App->scene->selectedGO, nullptr);
		App->scene->selectedGO = nullptr;

		nextPreReturn = GO_NONE;
		return GO_CUT;
	}

	return GO_NONE;
}

void GameObject::Update()
{
	boundingBox->Update();

	// Update the childs
	for (list<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		(*it)->Update();
	}
	// Update the components
	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->Update();
	}
}

void GameObject::CleanUp()
{

}

void GameObject::Delete()
{

	for (auto &component : components)
	{
		RELEASE(component);
	}

	for (auto &child : childs)
	{
		RELEASE(child);
	}

	App->scene->selectedGO = nullptr;
}

void GameObject::AddComponent(Type type)
{
	switch (type)
	{
	case Light:
		components.push_back(new ComponentLight(this));
		break;
	case Material:
		components.push_back(new ComponentMaterial(this));
		break;
	case Mesh:
		components.push_back(new ComponentMesh(this));
		break;
	case Camera:
		components.push_back(new ComponentCamera(this));
		break;
	}
}

Component * GameObject::AddComponentType(int type)
{
	if (type == 4)
	{
		ComponentLight* compLight = new ComponentLight(this);
		components.push_back(compLight);
		return compLight;
	}
	else if (type == 2)
	{
		ComponentMaterial* compMat = new ComponentMaterial(this);
		components.push_back(compMat);
		return compMat;
	}
	else if (type == 1)
	{
		ComponentMesh* compMesh = new ComponentMesh(this);
		components.push_back(compMesh);
		return compMesh;
	}
	else if (type == 3)
	{
		ComponentCamera* compCam = new ComponentCamera(this);
		components.push_back(compCam);
		return compCam;
	}
	return new Component();
}

Component* GameObject::GetComponentOfType(Type type)
{
	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->type == type)
		{
			return (*it);
		}
	}

	return NULL;
}

list<Component*> GameObject::GetAllComponentsOfType(Type type)
{
	list<Component*> comps;

	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->type == type)
		{
			comps.push_back((*it));
		}
	}

	return comps;
}

int GameObject::GetNumComponentsOfType(Type type)
{
	int contador = 0;

	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->type == type)
			contador++;
	}

	return contador;
}

bool GameObject::CanAddComponentOfType(Type type)
{
	int limit = 0;

	switch(type)
	{
	case Mesh:
		limit = MAX_MESH_COMP;
		break;
	case Material:
		limit = MAX_MATERIAL_COMP;
		break;
	case Camera:
		limit = MAX_CAMERA_COMP;
		break;
	case Light:
		limit = MAX_LIGHT_COMP;
		break;
	}

	if (limit == -1)
		return true;


	int contador = 0;

	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->type == type)
			contador++;
	}

	if (contador < limit)
		return true;

	return false;
}

void GameObject::Draw()
{
	if (openNode)
	{
		ImGui::SetNextTreeNodeOpen(true);
		openNode = false;
	}

	ImGuiTreeNodeFlags	flags;

	if (childs.size() == 0)
		flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf;
	else
		flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	ImGui::PushID(this);
	ImGui::PushID(&name);

	bool node_open;

	if (nullptr != App->scene->selectedGO && App->scene->selectedGO == this)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		node_open = ImGui::TreeNodeEx(name, flags);

		ImGui::PopStyleColor();
	}
	else
	{
		node_open = ImGui::TreeNodeEx(name, flags);
	}
	// When is clicked but no when the arrow is clicked
	if (ImGui::IsItemClicked() || 
		ImGui::IsItemClicked(1) && nullptr == App->scene->selectedGO ||
		ImGui::IsItemClicked(1) && this != App->scene->selectedGO)
	{
		App->ui->uiHierarchy->isItemClicked = true;

		if ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
		{
			selected = !selected;
			if (selected && this != App->scene->selectedGO)
			{
				App->scene->selectedGO != nullptr ? App->scene->selectedGO->selected = false : 1;
				App->scene->selectedGO = this;
			}
			else if (!this->selected && App->scene->selectedGO == this)
			{
				App->scene->selectedGO = nullptr;
			}
		}
	}
	if (node_open)
	{
		if (childs.size() > 0)
		{
			for (list<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
				(*it)->Draw();
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
	ImGui::PopID();

}

void GameObject::DrawComponents()
{
	if (ImGui::Checkbox(" ", &enable)) {}
	ImGui::SameLine();
	ImGui::InputText("Name", name, ARRAYSIZE(name));
	ImGui::Spacing();
	ImGui::Checkbox("Show Metadata", &showMetadata);

	if (showMetadata)
	{
		ImGui::SeparatorCustom(50, ImGui::GetWindowWidth() - 100);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4, 0.4, 0.4, 1));
		ImGui::Text("uID:");
		ImGui::Text(uID);
		ImGui::PopStyleColor();
	}
	ImGui::Spacing();
	ImGui::Separator();

	transform->Draw(0);
	ImGui::Separator();
	ImGui::Spacing();

	int i = 1;
	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->Draw(i);
		ImGui::Separator();
		ImGui::Spacing();
		i++;
	}

	ImGui::Separator();
	material->Draw(i+1);
	ImGui::Separator();
}

void GameObject::Save(System * system)
{
	system->StartObject();

	system->AddString("uID", uID);
	system->AddString("name", name);

	if (parent != NULL)
		system->AddString("parentUID", parent->uID);
	else
		system->AddString("parentUID", "0");

	system->AddBool("enable", enable);

	system->StartArray("components");
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->Save(system);
	}
	system->EndArray();

	system->StartArray("def_transform");
	transform->Save(system);
	system->EndArray();
	system->StartArray("def_material");
	material->Save(system);
	system->EndArray();

	system->EndObject();

}

void GameObject::Load(System * system, rapidjson::Value & value)
{
	sprintf(uID, system->GetString("uID", value));

	enable = system->GetBool("enable", value);

	rapidjson::Value components = value["components"].GetArray();
	for (rapidjson::Value::ValueIterator it = components.Begin(); it != components.End(); ++it) {
		Component* component = AddComponentType(system->GetComponentType("compType", (*it)));

		if (component != NULL) {
			component->Load(system, (*it));
		}
	}

	rapidjson::Value def_transform = value["def_transform"].GetArray();
	for (rapidjson::Value::ValueIterator it = def_transform.Begin(); it != def_transform.End(); ++it)
	{
		transform = new ComponentTransform(this);
		transform->Load(system, (*it));
	}

	rapidjson::Value def_material = value["def_material"].GetArray();
	for (rapidjson::Value::ValueIterator it = def_material.Begin(); it != def_material.End(); ++it)
	{
		material = new ComponentMaterial(this);
		material->Load(system, (*it));
	}

}
